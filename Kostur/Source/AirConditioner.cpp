#include "../Header/AirConditioner.h"
#include "../Header/Util.h"
#include "../Header/Lamp.h"
#include "../Header/Flap.h"
#include "../Header/Screen.h"
#include "../Header/SharedGeometry.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AirConditioner::AirConditioner(GLFWwindow* window, unsigned int basicShader) : basicShader(basicShader), isOn(false), lamp(nullptr), flap(nullptr), screen1(nullptr), screen2(nullptr), screen3(nullptr), lastTime(0.0), desiredTemp(24), measuredTemp(30), lastTempAdjustTime(0.0), basinFull(false) {
    textureShader = createShader("texture.vert", "texture.frag");

    std::cout << "Ucitavanje ikonica..." << std::endl;
    fireTexture = loadImageToTexture("Resources/fire.png");
    std::cout << "Fire texture ID: " << fireTexture << std::endl;
    snowflakeTexture = loadImageToTexture("Resources/snowflake.png");
    std::cout << "Snowflake texture ID: " << snowflakeTexture << std::endl;
    checkmarkTexture = loadImageToTexture("Resources/checkmark.png");
    std::cout << "Checkmark texture ID: " << checkmarkTexture << std::endl;

    SharedGeometry* geom = SharedGeometry::getInstance();
    VAO = geom->getQuadVAO();

    lamp = new Lamp(window);
    flap = new Flap();
    flap->initBuffers();

    screen1 = new Screen(window);
    screen2 = new Screen(window);
    screen3 = new Screen(window);

    float screenWidth = 0.14f;
    float screenHeight = 0.10f;
    float screenY = 0.68f;

    screen1->initBuffers(-0.3f, screenY, screenWidth, screenHeight);
    screen2->initBuffers(-0.15f, screenY, screenWidth, screenHeight);
    screen3->initBuffers(0.0f, screenY, screenWidth, screenHeight);

    flapOpen = 0.0f;
    flapCoverOpen = 0.0f;
    flapSpeed = 1.2f;
    flapThickness = 0.06f;

    lastTime = glfwGetTime();
}

AirConditioner::~AirConditioner() {
    if (textureShader) glDeleteProgram(textureShader);
    if (fireTexture) glDeleteTextures(1, &fireTexture);
    if (snowflakeTexture) glDeleteTextures(1, &snowflakeTexture);
    if (checkmarkTexture) glDeleteTextures(1, &checkmarkTexture);
    if (lamp) { delete lamp; lamp = nullptr; }
    if (flap) { delete flap; flap = nullptr; }
    if (screen1) { delete screen1; screen1 = nullptr; }
    if (screen2) { delete screen2; screen2 = nullptr; }
    if (screen3) { delete screen3; screen3 = nullptr; }
}

void AirConditioner::turnOn() {
    isOn = true;
}

void AirConditioner::handleClickNDC(float ndcX, float ndcY, float aspect) {
    bool wasOn = isOn;
    
    if (!basinFull || isOn) {
        if (lamp) lamp->handleClickNDC(ndcX, ndcY, aspect, isOn);
    }
    
    if (wasOn != isOn) {
        lastTempAdjustTime = glfwGetTime();
    }
}

void AirConditioner::turnOffDueToFullBasin() {
    isOn = false;
}

void AirConditioner::setBasinFull(bool full) {
    basinFull = full;
    if (full && isOn) {
        turnOffDueToFullBasin();
    }
}

void AirConditioner::increaseDesiredTemp() {
    if (desiredTemp < 40) {
        desiredTemp++;
    }
}

void AirConditioner::decreaseDesiredTemp() {
    if (desiredTemp > -10) {
        desiredTemp--;
    }
}

bool AirConditioner::getIsOn() const {
    return isOn;
}

bool AirConditioner::isActivelyWorking() const {
    return isOn && (desiredTemp != measuredTemp);
}

bool AirConditioner::isMaintainingTemp() const {
    return isOn && (desiredTemp == measuredTemp);
}

void AirConditioner::render() {
    double current = glfwGetTime();
    double delta = current - lastTime;
    lastTime = current;

    if (isOn) 
    {
        if (flapOpen < 1.0f) 
        {
            flapOpen += flapSpeed * (float)delta;
            if (flapOpen > 1.0f) flapOpen = 1.0f;
        }
        else 
        {
            flapCoverOpen += flapSpeed * (float)delta;
            if (flapCoverOpen > 1.0f) flapCoverOpen = 1.0f;
        }

        const double tempAdjustInterval = 5.0;
        if (current - lastTempAdjustTime >= tempAdjustInterval) 
        {
            if (measuredTemp < desiredTemp) 
            {
                measuredTemp++;
            } 
            else if (measuredTemp > desiredTemp) 
            {
                measuredTemp--;
            }
            lastTempAdjustTime = current;
        }
    }
    else 
    {
        if (flapCoverOpen > 0.0f) 
        {
            flapCoverOpen -= flapSpeed * (float)delta;
            if (flapCoverOpen < 0.0f) flapCoverOpen = 0.0f;
        }
        else 
        {
            flapOpen -= flapSpeed * (float)delta;
            if (flapOpen < 0.0f) flapOpen = 0.0f;
        }
    }

    float acBottom = 0.5f;

    glUseProgram(basicShader);
    
    glUniform2f(glGetUniformLocation(basicShader, "uPos"), 0.0f, 0.65f);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), 0.8f, 0.3f);
    glUniform3f(glGetUniformLocation(basicShader, "color"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(basicShader, "alpha"), 1.0f);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);

    if (flap) 
    {
        flap->update(acBottom, flapThickness, flapOpen, flapCoverOpen);
        flap->render(flapOpen > 0.0f, flapCoverOpen > 0.0f, basicShader);
    }

    if (screen1) screen1->renderWithTemperature(isOn, basicShader, desiredTemp);
    if (screen2) screen2->renderWithTemperature(isOn, basicShader, measuredTemp);
    
    if (screen3) 
    {
        unsigned int iconToShow = checkmarkTexture;
        
        if (desiredTemp > measuredTemp) 
        {
            iconToShow = fireTexture;
        } 
        else if (desiredTemp < measuredTemp) 
        {
            iconToShow = snowflakeTexture;
        }
        
        screen3->renderWithIcon(isOn, basicShader, textureShader, iconToShow);
    }

    glUseProgram(basicShader);
    
    glUniform2f(glGetUniformLocation(basicShader, "uPos"), 0.385f, 0.525f);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), 1.0f, 1.0f);
    
    if (isOn) {
        glUniform3f(glGetUniformLocation(basicShader, "color"), 1.0f, 0.0f, 0.0f);
    } else {
        glUniform3f(glGetUniformLocation(basicShader, "color"), 0.4f, 0.08f, 0.05f);
    }
    glUniform1f(glGetUniformLocation(basicShader, "alpha"), 1.0f);
    
    if (lamp) lamp->render(isOn);
    glUseProgram(0);
}