#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Lamp {
public:
    Lamp(GLFWwindow* window);
    ~Lamp();
    void render(bool isOn);
    void handleClickNDC(float ndcX, float ndcY, float aspect, bool &toggled);

private:
    unsigned int VAO, VBO;
    unsigned int shader;
    float centerX, centerY, radiusY;
    unsigned int vertexCount;
};