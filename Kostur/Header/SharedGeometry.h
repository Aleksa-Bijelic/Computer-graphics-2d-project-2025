#ifndef SHARED_GEOMETRY_H
#define SHARED_GEOMETRY_H

#include <GL/glew.h>

class SharedGeometry {
private:
    static SharedGeometry* instance;
    
    unsigned int quadVAO;
    unsigned int quadVBO;
    unsigned int circleVAO;
    unsigned int circleVBO;
    unsigned int circleVertexCount;
    
    SharedGeometry();
    ~SharedGeometry();
    
public:
    static SharedGeometry* getInstance();
    static void cleanup();
    
    unsigned int getQuadVAO() const { return quadVAO; }
    unsigned int getCircleVAO() const { return circleVAO; }
    unsigned int getCircleVertexCount() const { return circleVertexCount; }
    
    SharedGeometry(const SharedGeometry&) = delete;
    SharedGeometry& operator=(const SharedGeometry&) = delete;
};

#endif
