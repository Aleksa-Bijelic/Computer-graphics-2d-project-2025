#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>

#include "../Header/Util.h"
#include "../Header/AirConditioner.h"
#include "../Header/Basin.h"
#include "../Header/Pipe.h"
#include "../Header/SharedGeometry.h"

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        AirConditioner* ac = static_cast<AirConditioner*>(glfwGetWindowUserPointer(window));
        if (!ac) return;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        int fbW, fbH;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        float ndcX = (float)(x / fbW) * 2.0f - 1.0f;
        float ndcY = 1.0f - (float)(y / fbH) * 2.0f;
        float aspect = (float)fbW / (float)fbH;
        ac->handleClickNDC(ndcX, ndcY, aspect);
    }
}

int main()
{
    if (!glfwInit()) return endProgram("GLFW nije uspeo da se inicijalizuje.");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Klima Uredjaj", monitor, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    SharedGeometry::getInstance();

    unsigned int basicShader = createShader("basic.vert", "basic.frag");

    AirConditioner* ac = new AirConditioner(window, basicShader);
    glfwSetWindowUserPointer(window, ac);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    Basin* basin = new Basin();
    basin->initBuffers();

    Pipe* pipe = new Pipe();
    pipe->initBuffers();

    GLFWcursor* customCursor = loadImageToCursor("Resources/remote-control.png");
    if (customCursor != NULL) {
        glfwSetCursor(window, customCursor);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.8f, 0.6f, 1.0f);

    auto lastTime = std::chrono::high_resolution_clock::now();
    const double frameTime = 1.0 / 75.0;

    auto lastTempChangeTime = std::chrono::high_resolution_clock::now();
    const double tempChangeInterval = 0.1;

    while (!glfwWindowShouldClose(window))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;

        if (elapsed.count() >= frameTime)
        {
            lastTime = currentTime;

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            std::chrono::duration<double> timeSinceLastTempChange = currentTime - lastTempChangeTime;

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && ac->getIsOn()) 
            {
                if (timeSinceLastTempChange.count() >= tempChangeInterval) 
                {
                    ac->increaseDesiredTemp();
                    lastTempChangeTime = currentTime;
                }
            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && ac->getIsOn()) 
            {
                if (timeSinceLastTempChange.count() >= tempChangeInterval) 
                {
                    ac->decreaseDesiredTemp();
                    lastTempChangeTime = currentTime;
                }
            }

            static bool spacePressed = false;
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
            {
                if (!spacePressed) 
                {
                    basin->emptyWater();
                    ac->setBasinFull(false);
                    spacePressed = true;
                }
            } 
            else 
            {
                spacePressed = false;
            }

            bool isWorking = ac->isActivelyWorking();
            bool isMaintaining = ac->isMaintainingTemp();
            basin->updateWaterLevel((float)elapsed.count(), isWorking || isMaintaining, isMaintaining);
            
            pipe->update((float)elapsed.count(), isWorking || isMaintaining, isMaintaining, basin->getWaterLevel());
            
            if (basin->isFull()) {
                ac->setBasinFull(true);
            }

            glClear(GL_COLOR_BUFFER_BIT);

            basin->render(basicShader);

            pipe->render(basicShader);

            ac->render();

            glfwSwapBuffers(window);
        }

        glfwPollEvents();
    }

    if (customCursor != NULL) 
    {
        glfwDestroyCursor(customCursor);
    }

    delete ac;
    delete basin;
    delete pipe;
    if (basicShader) glDeleteProgram(basicShader);
    
    SharedGeometry::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}