#include "../include/camera.h"
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h> 

float camX = 0.0f; 
float camY = 2.0f; 
float camZ = 5.0f; 
float camYaw = 0.0f;
float camPitch = 0.0f;
const float CAM_PITCH_LIMIT = 1.55f;

// O extern permite comunicar com o cenário
extern bool portaFrenteAberta; 
extern bool portaQ1Aberta;
extern bool portaQ2Aberta;
extern bool portaSuiteAberta;
extern const float DOOR_W;
extern float calcularDistancia(float x1, float z1, float x2, float z2);

void keyboard(unsigned char key, int x, int y) {
    float velocidade = 0.5f; 

    float forwardX = sinf(camYaw);
    float forwardZ = -cosf(camYaw);
    float rightX = cosf(camYaw);
    float rightZ = sinf(camYaw);
    
    switch (key) {
        case 'w': case 'W': camX += forwardX * velocidade; camZ += forwardZ * velocidade; break;
        case 's': case 'S': camX -= forwardX * velocidade; camZ -= forwardZ * velocidade; break;
        case 'a': case 'A': camX -= rightX * velocidade; camZ -= rightZ * velocidade; break;
        case 'd': case 'D': camX += rightX * velocidade; camZ += rightZ * velocidade; break;
        case 'e': case 'E': camY += velocidade; break;
        case 'q': case 'Q': camY -= velocidade; if (camY < 0.5f) camY = 0.5f; break;
        case 'o': case 'O':
            if (calcularDistancia(camX, camZ, (-6.4f - 0.5f * DOOR_W), 10.0f) < 3.0f) portaFrenteAberta = !portaFrenteAberta;
            else if (calcularDistancia(camX, camZ, -0.2f, 0.0f) < 3.0f) portaQ1Aberta = !portaQ1Aberta;
            else if (calcularDistancia(camX, camZ, 6.2f, 0.0f) < 3.0f) portaQ2Aberta = !portaQ2Aberta;
            else if (calcularDistancia(camX, camZ, 5.2f, 4.0f) < 3.0f) portaSuiteAberta = !portaSuiteAberta;
            break;
        case 27: 
            exit(0); 
            break;             
    }
    glutPostRedisplay(); 
}

void specialKeys(int key, int x, int y) {
    (void)x; (void)y;
    float velocidadeOlhar = 0.04f; 

    switch (key) {
        case GLUT_KEY_LEFT: camYaw -= velocidadeOlhar; break;
        case GLUT_KEY_RIGHT: camYaw += velocidadeOlhar; break;
        case GLUT_KEY_UP: camPitch = fminf(camPitch + velocidadeOlhar, CAM_PITCH_LIMIT); break;
        case GLUT_KEY_DOWN: camPitch = fmaxf(camPitch - velocidadeOlhar, -CAM_PITCH_LIMIT); break;
        default: return;
    }
    glutPostRedisplay();
}