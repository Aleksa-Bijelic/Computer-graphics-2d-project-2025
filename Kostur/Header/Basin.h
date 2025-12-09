#pragma once
#include <GL/glew.h>

class Basin {
public:
    Basin();
    ~Basin();
    void initBuffers();
    void render(unsigned int shader);
    void updateWaterLevel(float delta, bool isFilling, bool isMaintaining);
    void emptyWater();
    bool isFull() const;
    float getWaterLevel() const;

private:
    unsigned int VAO, VBO;
    
    float waterLevel;
    float basinCenterX, basinCenterY;
    float basinWidth, basinHeight;
    float thickness;
};
