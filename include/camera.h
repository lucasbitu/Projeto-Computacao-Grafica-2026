#ifndef CAMERA_H
#define CAMERA_H

extern float camX; 
extern float camY; 
extern float camZ; 
extern float camYaw;
extern float camPitch;

// Variáveis para o Tour Virtual
extern bool tourAtivo;
void updateTour();

void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);

#endif