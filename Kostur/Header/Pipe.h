#pragma once
#include <GL/glew.h>
#include <vector>

struct WaterDrop {
    float y;
    float velocity;
    bool active;
};

class Pipe {
public:
    Pipe();
    ~Pipe();
    void initBuffers();
    void render(unsigned int shader);
    void update(float delta, bool isWorking, bool isMaintaining, float waterLevel);
    
private:
    unsigned int pipeVAO, pipeVBO;
    unsigned int dropVAO, dropVBO;
    
    float pipeX, pipeY;
    float pipeWidth, pipeHeight;
    float pipeTop, pipeBottom;
    
    float basinBottom;
    float basinHeight;
    
    std::vector<WaterDrop> drops;
    double timeSinceLastDrop;
    const float gravity = 2.5f;
    
    void spawnDrop();
};
