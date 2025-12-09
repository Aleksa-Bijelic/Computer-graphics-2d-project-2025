#pragma once
#include <GL/glew.h>

class Flap {
public:
    Flap();
    ~Flap();
    void initBuffers();
    void update(float acBottom, float flapThickness, float openFactor, float coverFactor);
    void render(bool drawBlack, bool drawGray, unsigned int shader);

private:
    unsigned int VAO, VBO;
    
    float blackPosX, blackPosY, blackScaleX, blackScaleY;
    float grayPosX, grayPosY, grayScaleX, grayScaleY;
};