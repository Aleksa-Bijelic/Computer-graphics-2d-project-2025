#include "../Header/SharedGeometry.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SharedGeometry* SharedGeometry::instance = nullptr;

SharedGeometry::SharedGeometry() : quadVAO(0), quadVBO(0), circleVAO(0), circleVBO(0), circleVertexCount(0) {
    float quadVertices[] = {
        -0.5f,  0.5f,  // gornje levo
         0.5f,  0.5f,  // gornje desno
         0.5f, -0.5f,  // donje desno
        -0.5f, -0.5f   // donje levo
    };
    
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    const int segments = 16;
    const float dropRadius = 0.012f;
    
    std::vector<float> circleVertices;
    circleVertices.push_back(0.0f);
    circleVertices.push_back(0.0f);
    
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * M_PI / segments;
        circleVertices.push_back(dropRadius * cos(angle));
        circleVertices.push_back(dropRadius * sin(angle));
    }
    
    circleVertexCount = static_cast<unsigned int>(circleVertices.size() / 2);
    
    glGenVertexArrays(1, &circleVAO);
    glGenBuffers(1, &circleVBO);
    glBindVertexArray(circleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SharedGeometry::~SharedGeometry() {
    if (quadVAO) glDeleteVertexArrays(1, &quadVAO);
    if (quadVBO) glDeleteBuffers(1, &quadVBO);
    if (circleVAO) glDeleteVertexArrays(1, &circleVAO);
    if (circleVBO) glDeleteBuffers(1, &circleVBO);
}

SharedGeometry* SharedGeometry::getInstance() {
    if (instance == nullptr) {
        instance = new SharedGeometry();
    }
    return instance;
}

void SharedGeometry::cleanup() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}
