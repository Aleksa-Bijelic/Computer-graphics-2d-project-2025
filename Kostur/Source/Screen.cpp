#include "../Header/Screen.h"
#include <GL/glew.h>
#include <cmath>
#include <string>

Screen::Screen(GLFWwindow* window) : VAO(0), VBO(0), segmentHorizVAO(0), segmentHorizVBO(0), segmentVertVAO(0), segmentVertVBO(0), textureVAO(0), textureVBO(0), centerX(0.0f), centerY(0.0f), width(0.0f), height(0.0f), aspect(1.0f) {
    int fbW = 1, fbH = 1;
    if (window) glfwGetFramebufferSize(window, &fbW, &fbH);
    aspect = (float)fbW / (float)fbH;
}

Screen::~Screen() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (segmentHorizVAO) glDeleteVertexArrays(1, &segmentHorizVAO);
    if (segmentHorizVBO) glDeleteBuffers(1, &segmentHorizVBO);
    if (segmentVertVAO) glDeleteVertexArrays(1, &segmentVertVAO);
    if (segmentVertVBO) glDeleteBuffers(1, &segmentVertVBO);
    if (textureVAO) glDeleteVertexArrays(1, &textureVAO);
    if (textureVBO) glDeleteBuffers(1, &textureVBO);
}

void Screen::initBuffers(float centerX, float centerY, float width, float height) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->width = width;
    this->height = height;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float verts[] = {
        -0.5f,  0.5f,  // gornji levi
         0.5f,  0.5f,  // gornji desni
         0.5f, -0.5f,  // donji desni
        -0.5f, -0.5f   // donji levi
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Horizontalna linija (centar u 0,0, dužina 1)
    glGenVertexArrays(1, &segmentHorizVAO);
    glGenBuffers(1, &segmentHorizVBO);
    glBindVertexArray(segmentHorizVAO);
    glBindBuffer(GL_ARRAY_BUFFER, segmentHorizVBO);
    
    float horizVerts[] = {
        -0.5f, 0.0f,
         0.5f, 0.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(horizVerts), horizVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Vertikalna linija (centar u 0,0, dužina 1)
    glGenVertexArrays(1, &segmentVertVAO);
    glGenBuffers(1, &segmentVertVBO);
    glBindVertexArray(segmentVertVAO);
    glBindBuffer(GL_ARRAY_BUFFER, segmentVertVBO);
    
    float vertVerts[] = {
        0.0f, -0.5f,
        0.0f,  0.5f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertVerts), vertVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &textureVAO);
    glGenBuffers(1, &textureVBO);
    glBindVertexArray(textureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);

    float textureVerts[] = {
        -0.5f,  0.5f,     0.0f, 1.0f,  // gornji levi
         0.5f,  0.5f,     1.0f, 1.0f,  // gornji desni
         0.5f, -0.5f,     1.0f, 0.0f,  // donji desni
        -0.5f, -0.5f,     0.0f, 0.0f   // donji levi
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(textureVerts), textureVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Screen::render(bool isOn, unsigned int shader) {
    glUseProgram(shader);
    
    float scaleX = width / aspect;
    float scaleY = height;
    glUniform2f(glGetUniformLocation(shader, "uPos"), centerX, centerY);
    glUniform2f(glGetUniformLocation(shader, "uScale"), scaleX, scaleY);
    
    glUniform3f(glGetUniformLocation(shader, "color"), 0.0f, 0.0f, 0.0f);
    glUniform1f(glGetUniformLocation(shader, "alpha"), 1.0f);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Screen::drawSegment(float x1, float y1, float x2, float y2, unsigned int shader) {
    float centerX = (x1 + x2) / 2.0f;
    float centerY = (y1 + y2) / 2.0f;
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);
    
    glUniform2f(glGetUniformLocation(shader, "uPos"), centerX, centerY);
    
    glLineWidth(3.0f);
    
    if (fabs(dy) < 0.001f) {
        glUniform2f(glGetUniformLocation(shader, "uScale"), length, 1.0f);
        glBindVertexArray(segmentHorizVAO);
    } else {
        glUniform2f(glGetUniformLocation(shader, "uScale"), 1.0f, length);
        glBindVertexArray(segmentVertVAO);
    }
    
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Screen::drawDigit(int digit, float x, float y, float digitWidth, float digitHeight, unsigned int shader) {
    // 7-segmentni displej
    // Segmenti:
    //     a
    //   f   b
    //     g
    //   e   c
    //     d
    
    float w = digitWidth / aspect;
    float h = digitHeight;
    float gap = h * 0.05f;
    
    float left = x - w / 2.0f;
    float right = x + w / 2.0f;
    float top = y + h / 2.0f;
    float middle = y;
    float bottom = y - h / 2.0f;
    
    // Definicija segmenata za svaku cifru (a, b, c, d, e, f, g)
    bool segments[10][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}  // 9
    };
    
    if (digit < 0 || digit > 9) return;
    
    glUseProgram(shader);
    glUniform3f(glGetUniformLocation(shader, "color"), 0.0f, 1.0f, 0.0f);
    glUniform1f(glGetUniformLocation(shader, "alpha"), 1.0f);
    
    if (segments[digit][0]) drawSegment(left + gap, top, right - gap, top, shader);           // a (gornji)
    if (segments[digit][1]) drawSegment(right, top - gap, right, middle + gap, shader);       // b (gornji desni)
    if (segments[digit][2]) drawSegment(right, middle - gap, right, bottom + gap, shader);    // c (donji desni)
    if (segments[digit][3]) drawSegment(left + gap, bottom, right - gap, bottom, shader);     // d (donji)
    if (segments[digit][4]) drawSegment(left, middle - gap, left, bottom + gap, shader);      // e (donji levi)
    if (segments[digit][5]) drawSegment(left, top - gap, left, middle + gap, shader);         // f (gornji levi)
    if (segments[digit][6]) drawSegment(left + gap, middle, right - gap, middle, shader);     // g (srednji)
}

void Screen::renderWithTemperature(bool isOn, unsigned int shader, int temperature) {
    render(isOn, shader);
    
    if (!isOn) return;
    
    std::string tempStr = std::to_string(temperature);
    
    float digitWidth = height * 0.25f;
    float digitHeight = height * 0.4f;
    float digitSpacing = digitWidth / aspect * 1.6f;
    
    int numDigits = (int)tempStr.length();
    float startX = centerX - (numDigits - 1) * digitSpacing / 2.0f;
    
    for (int i = 0; i < numDigits; i++) {
        if (tempStr[i] == '-') {
            glUseProgram(shader);
            glUniform3f(glGetUniformLocation(shader, "color"), 0.0f, 1.0f, 0.0f);
            glUniform1f(glGetUniformLocation(shader, "alpha"), 1.0f);
            float x = startX + i * digitSpacing;
            float w = digitWidth / aspect * 0.5f;
            drawSegment(x - w, centerY, x + w, centerY, shader);
        } else {
            int digit = tempStr[i] - '0';
            drawDigit(digit, startX + i * digitSpacing, centerY, digitWidth, digitHeight, shader);
        }
    }
    
    glUseProgram(0);
}

void Screen::renderWithIcon(bool isOn, unsigned int shader, unsigned int textureShader, unsigned int iconTexture) {
    render(isOn, shader);
    
    if (!isOn) return;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(textureShader);
    
    float iconScale = height * 0.7f;
    glUniform2f(glGetUniformLocation(textureShader, "uPos"), centerX, centerY);
    glUniform2f(glGetUniformLocation(textureShader, "uScale"), iconScale / aspect, iconScale);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, iconTexture);
    
    int texLoc = glGetUniformLocation(textureShader, "textureSampler");
    if (texLoc != -1) {
        glUniform1i(texLoc, 0);
    }
    
    glBindVertexArray(textureVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    
    glDisable(GL_BLEND);
}
