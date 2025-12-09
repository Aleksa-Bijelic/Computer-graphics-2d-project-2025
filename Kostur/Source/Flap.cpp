#include "../Header/Flap.h"
#include "../Header/SharedGeometry.h"
#include <GL/glew.h>

Flap::Flap() : VAO(0), VBO(0), 
    blackPosX(0), blackPosY(0), blackScaleX(0), blackScaleY(0),
    grayPosX(0), grayPosY(0), grayScaleX(0), grayScaleY(0) {
}

Flap::~Flap() {
}

void Flap::initBuffers() {
    SharedGeometry* geom = SharedGeometry::getInstance();
    VAO = geom->getQuadVAO();
}

void Flap::update(float acBottom, float flapThickness, float openFactor, float coverFactor) {
    float left = -0.35f;
    float right = 0.35f;
    float width = right - left;
    
    float currentBlackHeight = flapThickness * openFactor;
    float blackTop = acBottom + 0.06f;
    float blackBottom = blackTop - currentBlackHeight;
    
    blackPosX = (left + right) / 2.0f;
    blackPosY = (blackTop + blackBottom) / 2.0f;
    blackScaleX = width;
    blackScaleY = currentBlackHeight;
    
    float currentGrayHeight = flapThickness * coverFactor;
    float grayTop = blackBottom;
    float grayBottom = grayTop - currentGrayHeight;
    
    grayPosX = (left + right) / 2.0f;
    grayPosY = (grayTop + grayBottom) / 2.0f;
    grayScaleX = width;
    grayScaleY = currentGrayHeight;
}

void Flap::render(bool drawBlack, bool drawGray, unsigned int shader) {
    glUseProgram(shader);
    
    if (drawBlack) {
        glUniform2f(glGetUniformLocation(shader, "uPos"), blackPosX, blackPosY);
        glUniform2f(glGetUniformLocation(shader, "uScale"), blackScaleX, blackScaleY);
        
        glUniform3f(glGetUniformLocation(shader, "color"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(shader, "alpha"), 1.0f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }
    
    if (drawGray) {
        glUniform2f(glGetUniformLocation(shader, "uPos"), grayPosX, grayPosY);
        glUniform2f(glGetUniformLocation(shader, "uScale"), grayScaleX, grayScaleY);
        
        glUniform3f(glGetUniformLocation(shader, "color"), 0.65f, 0.65f, 0.65f);
        glUniform1f(glGetUniformLocation(shader, "alpha"), 1.0f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }
    
    glUseProgram(0);
}
