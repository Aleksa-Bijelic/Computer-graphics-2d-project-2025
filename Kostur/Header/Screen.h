#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Screen {
public:
    Screen(GLFWwindow* window);
    ~Screen();
    void initBuffers(float centerX, float centerY, float width, float height);
    void render(bool isOn, unsigned int shader);
    void renderWithTemperature(bool isOn, unsigned int shader, int temperature);
    void renderWithIcon(bool isOn, unsigned int shader, unsigned int textureShader, unsigned int iconTexture);

private:
    unsigned int VAO, VBO;
    unsigned int segmentHorizVAO, segmentHorizVBO;
    unsigned int segmentVertVAO, segmentVertVBO;
    unsigned int textureVAO, textureVBO;
    float centerX, centerY;
    float width, height;
    float aspect;
    
    void drawDigit(int digit, float x, float y, float digitWidth, float digitHeight, unsigned int shader);
    void drawSegment(float x1, float y1, float x2, float y2, unsigned int shader);
};
