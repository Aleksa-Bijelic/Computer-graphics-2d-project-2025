#include "../Header/Pipe.h"
#include "../Header/SharedGeometry.h"
#include <GL/glew.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Pipe::Pipe() : pipeVAO(0), pipeVBO(0), dropVAO(0), dropVBO(0), 
    pipeX(0.22f), pipeY(0.015f), pipeWidth(0.02f), pipeHeight(0.97f),
    pipeTop(0.5f), pipeBottom(-0.47f), 
    basinBottom(-0.85f), basinHeight(0.4f),
    timeSinceLastDrop(0.0) {
    
    drops.resize(5);
    for (auto& drop : drops) {
        drop.active = false;
        drop.y = pipeBottom;
        drop.velocity = 0.0f;
    }
}

Pipe::~Pipe() {
}

void Pipe::initBuffers() {
    SharedGeometry* geom = SharedGeometry::getInstance();
    pipeVAO = geom->getQuadVAO();
    dropVAO = geom->getCircleVAO();
}

void Pipe::render(unsigned int shader) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(shader);
    
    glUniform2f(glGetUniformLocation(shader, "uPos"), pipeX, pipeY);
    glUniform2f(glGetUniformLocation(shader, "uScale"), pipeWidth, pipeHeight);
    glUniform3f(glGetUniformLocation(shader, "color"), 0.65f, 0.65f, 0.7f);
    glUniform1f(glGetUniformLocation(shader, "alpha"), 0.85f);
    
    glBindVertexArray(pipeVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    // Kapi vode
    glUniform3f(glGetUniformLocation(shader, "color"), 0.5f, 0.7f, 0.9f);
    glUniform1f(glGetUniformLocation(shader, "alpha"), 0.7f);
    
    SharedGeometry* geom = SharedGeometry::getInstance();
    const int verticesPerDrop = geom->getCircleVertexCount();
    
    for (int i = 0; i < (int)drops.size(); i++) {
        if (drops[i].active) {
            glUniform2f(glGetUniformLocation(shader, "uPos"), pipeX, drops[i].y);
            glUniform2f(glGetUniformLocation(shader, "uScale"), 1.0f, 1.0f);
            
            glBindVertexArray(dropVAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, verticesPerDrop);
            glBindVertexArray(0);
        }
    }
    
    glUseProgram(0);
    glDisable(GL_BLEND);
}

void Pipe::update(float delta, bool isWorking, bool isMaintaining, float waterLevel) {
    if (isWorking) {
        double currentDropInterval;
        if (isMaintaining) {
            currentDropInterval = 0.6;
        } else {
            currentDropInterval = 0.3;
        }
        
        timeSinceLastDrop += delta;
        if (timeSinceLastDrop >= currentDropInterval) {
            spawnDrop();
            timeSinceLastDrop = 0.0;
        }
    }
    
    float currentWaterTop = basinBottom + (basinHeight * waterLevel);
    
    for (int i = 0; i < (int)drops.size(); i++) {
        if (drops[i].active) {
            drops[i].velocity += gravity * delta;
            
            drops[i].y -= drops[i].velocity * delta;
            
            if (drops[i].y <= currentWaterTop) {
                drops[i].active = false;
            }
        }
    }
}

void Pipe::spawnDrop() {
    for (auto& drop : drops) {
        if (!drop.active) {
            drop.active = true;
            drop.y = pipeBottom;
            drop.velocity = 0.0f;
            return;
        }
    }
}
