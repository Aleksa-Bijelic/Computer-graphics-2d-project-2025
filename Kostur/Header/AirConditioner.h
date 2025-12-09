#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Lamp;
class Flap;
class Screen;

class AirConditioner {
public:
    AirConditioner(GLFWwindow* window, unsigned int basicShader);
    ~AirConditioner();
    void render();
    void turnOn();      
    void handleClickNDC(float ndcX, float ndcY, float aspect);
    void increaseDesiredTemp();
    void decreaseDesiredTemp();
    bool getIsOn() const;
    bool isActivelyWorking() const;
    bool isMaintainingTemp() const;
    void turnOffDueToFullBasin();
    void setBasinFull(bool full);

private:
unsigned int basicShader;
unsigned int textureShader;
unsigned int VAO, VBO;

Lamp* lamp;
Flap* flap;
Screen* screen1;
Screen* screen2;
Screen* screen3;

unsigned int fireTexture;
unsigned int snowflakeTexture;
unsigned int checkmarkTexture;

    bool isOn;

    double lastTime;
    float flapOpen;
    float flapCoverOpen;
    float flapSpeed;
    float flapThickness;

    int desiredTemp;
    int measuredTemp;
    double lastTempAdjustTime;
    
    bool basinFull;
};