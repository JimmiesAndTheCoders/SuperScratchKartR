#include "shadow.h"
#include "rlgl.h"
#include <math.h>

Shadow::Shadow() {
    shadowTexture = { 0 }; 
}

Shadow::~Shadow() {}

void Shadow::Draw(Vector3 position, float radius, float opacity) const {
    if (radius <= 0.0f) return;
    float y = position.y + 0.02f; 
    int segments = 32;
    
    rlCheckRenderBatchLimit(segments * 3);
    rlDisableTexture();
    rlDisableBackfaceCulling();
    
    rlBegin(RL_TRIANGLES);
        rlColor4f(0.0f, 0.0f, 0.0f, opacity);
        
        for (int i = 0; i < segments; i++) {
            float angle1 = (float)i * (360.0f / segments) * DEG2RAD;
            float angle2 = (float)(i + 1) * (360.0f / segments) * DEG2RAD;
            
            rlVertex3f(position.x, y, position.z);
            rlVertex3f(position.x + sinf(angle1) * radius, y, position.z + cosf(angle1) * radius);
            rlVertex3f(position.x + sinf(angle2) * radius, y, position.z + cosf(angle2) * radius);
        }
    rlEnd();
    rlEnableBackfaceCulling();
    rlSetTexture(0); 
}