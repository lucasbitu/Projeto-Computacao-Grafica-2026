#ifndef CENARIO_H
#define CENARIO_H

#include <GL/glut.h>

extern bool portaFrenteAberta; 
extern bool portaQ1Aberta;
extern bool portaQ2Aberta;
extern bool portaSuiteAberta;

extern const float DOOR_H;
extern const float DOOR_W;

float calcularDistancia(float x1, float z1, float x2, float z2);

void drawSkybox();
void drawGround();
void drawInternalWalls();
void drawHouse();
void updateRoomLightPositions(void);
void drawCeilingLampFixtures(void);
void drawWindows();
void drawDoorModel();
void drawDoors();

#endif