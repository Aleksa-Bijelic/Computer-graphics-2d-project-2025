#include "../Header/Lamp.h"
#include "../Header/Util.h"
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Lamp::Lamp(GLFWwindow* window) : VAO(0), VBO(0), shader(0), centerX(0.385f), centerY(0.525f), radiusY(0.015f), vertexCount(0) {
    int segments = 32;
    int fbW = 1, fbH = 1;
    if (window) glfwGetFramebufferSize(window, &fbW, &fbH);
    float aspect = (float)fbW / (float)fbH;
    float xRadius = radiusY / aspect;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector<float> verts;
    verts.push_back(0.0f);
    verts.push_back(0.0f);
    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * M_PI / segments;
        verts.push_back(xRadius * cos(angle));
        verts.push_back(radiusY * sin(angle));
    }
    vertexCount = static_cast<unsigned int>(verts.size() / 2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Lamp::~Lamp() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
}

void Lamp::render(bool isOn) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    glBindVertexArray(0);
}

void Lamp::handleClickNDC(float ndcX, float ndcY, float aspect, bool &toggled) {
    float dx = (ndcX - centerX) / (1.0f / aspect);
    float dy = (ndcY - centerY);
    float dist2 = dx*dx + dy*dy;
    if (dist2 <= radiusY * radiusY) {
        toggled = !toggled;
    }
}
