#include "../Header/Basin.h"
#include "../Header/SharedGeometry.h"
#include <GL/glew.h>

Basin::Basin() : VAO(0), VBO(0), waterLevel(0.0f),
    basinCenterX(0.0f), basinCenterY(-0.65f), 
    basinWidth(0.56f), basinHeight(0.4f), thickness(0.02f) {
}

Basin::~Basin() {
}

void Basin::initBuffers() {
    SharedGeometry* geom = SharedGeometry::getInstance();
    VAO = geom->getQuadVAO();
}

void Basin::render(unsigned int shader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(shader);
    
    // Providna unutrasnjost lavora
    glUniform2f(glGetUniformLocation(shader, "uPos"), basinCenterX, basinCenterY);
    glUniform2f(glGetUniformLocation(shader, "uScale"), basinWidth, basinHeight);
    glUniform3f(glGetUniformLocation(shader, "color"), 0.9f, 0.9f, 0.95f);
    glUniform1f(glGetUniformLocation(shader, "alpha"), 0.3f);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    // Voda
    if (waterLevel > 0.0f) {
        float currentWaterHeight = basinHeight * waterLevel;
        float waterCenterY = basinCenterY - (basinHeight - currentWaterHeight) / 2.0f;
        
        glUniform2f(glGetUniformLocation(shader, "uPos"), basinCenterX, waterCenterY);
        glUniform2f(glGetUniformLocation(shader, "uScale"), basinWidth, currentWaterHeight);
        glUniform3f(glGetUniformLocation(shader, "color"), 0.5f, 0.7f, 0.9f);
        glUniform1f(glGetUniformLocation(shader, "alpha"), 0.7f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }
    
	// Ivice lavora
    glUniform3f(glGetUniformLocation(shader, "color"), 0.7f, 0.7f, 0.75f);
    glUniform1f(glGetUniformLocation(shader, "alpha"), 1.0f);
    
    // Dno
    float bottomY = basinCenterY - basinHeight / 2.0f - thickness / 2.0f;
    glUniform2f(glGetUniformLocation(shader, "uPos"), basinCenterX, bottomY);
    glUniform2f(glGetUniformLocation(shader, "uScale"), basinWidth + 2 * thickness, thickness);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    // Leva strana
    float leftX = basinCenterX - basinWidth / 2.0f - thickness / 2.0f;
    glUniform2f(glGetUniformLocation(shader, "uPos"), leftX, basinCenterY);
    glUniform2f(glGetUniformLocation(shader, "uScale"), thickness, basinHeight);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    // Desna strana
    float rightX = basinCenterX + basinWidth / 2.0f + thickness / 2.0f;
    glUniform2f(glGetUniformLocation(shader, "uPos"), rightX, basinCenterY);
    glUniform2f(glGetUniformLocation(shader, "uScale"), thickness, basinHeight);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    glUseProgram(0);
    glDisable(GL_BLEND);
}

void Basin::updateWaterLevel(float delta, bool isFilling, bool isMaintaining) {
    if (isFilling && waterLevel < 1.0f) {
        float fillRate;
        
        if (isMaintaining) {
            fillRate = 1.0f / 60.0f;
        } else {
            fillRate = 1.0f / 40.0f;
        }
        
        waterLevel += delta * fillRate;
        if (waterLevel > 1.0f) waterLevel = 1.0f;
    }
}

void Basin::emptyWater() {
    waterLevel = 0.0f;
}

bool Basin::isFull() const {
    return waterLevel >= 1.0f;
}

float Basin::getWaterLevel() const {
    return waterLevel;
}
