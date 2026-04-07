#include <GL/glut.h> 
#include <stdlib.h>  
#include <iostream> 
#include <math.h>

// A TUA ARQUITETURA MODULAR
#include "../include/texturas_util.h"
#include "../include/camera.h"
#include "../include/moveis.h"
#include "../include/cenario.h"

void init(void) {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); 
    glEnable(GL_DEPTH_TEST); 
    glShadeModel(GL_SMOOTH); 

    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat sol_difusa[] = { 0.7f, 0.7f, 0.7f, 1.0f }; 
    GLfloat sol_ambiente[] = { 0.6f, 0.6f, 0.6f, 1.0f };

    glLightfv(GL_LIGHT1, GL_DIFFUSE, sol_difusa);
    glLightfv(GL_LIGHT1, GL_AMBIENT, sol_ambiente); 
    glEnable(GL_LIGHT1); 

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_TEXTURE_2D); 
    
    texPiso = loadTexture("texturas/piso.jpg");     
    texParede = loadTexture("texturas/paredes.jpg");
    texGramado = loadTexture("texturas/grama.jpg");
    texTelhado = loadTexture("texturas/telhado.jpg");
    texGesso = loadTexture("texturas/gesso.jpg");
    texPorta = loadTexture("texturas/porta.jpg"); 

    texGeladeira = loadTexture("texturas/geladeira.jpg");
    texMarmore = loadTexture("texturas/marmore.jpg");
    texFogaoFrente = loadTexture("texturas/fogao_frente.png");
    texFogaoCima = loadTexture("texturas/fogao_cima.png");
    texGuardaRoupa = loadTexture("texturas/guarda_roupa1.png"); 
    
    texCeu = loadTexture("texturas/ceu.jpg"); 
}

void reshape(int w, int h) {
    if (h == 0) h = 1; 
    float aspect = (float)w / (float)h; 

    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    
    gluPerspective(60.0, aspect, 0.1, 500.0); 
    
    glMatrixMode(GL_MODELVIEW); 
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity(); 

    float cosP = cosf(camPitch);
    float dirX = sinf(camYaw) * cosP;
    float dirY = sinf(camPitch);
    float dirZ = -cosf(camYaw) * cosP;

    gluLookAt(camX, camY, camZ,                      
              camX + dirX, camY + dirY, camZ + dirZ, 
              0.0, 1.0, 0.0);                        

    // Renderiza o mundo
    drawSkybox();

    GLfloat light_position[] = { 0.0f, 3.5f, -3.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat sol_posicao[] = { 0.0f, 15.0f, 30.0f, 0.0f }; 
    glLightfv(GL_LIGHT1, GL_POSITION, sol_posicao);

    drawGround();
    drawHouse();
    drawInternalWalls();
    drawDoors(); 
    
    // Renderiza os Móveis
    drawDiningTable(); 
    drawBedroomsFurniture();
    drawKitchenFurniture();
    drawBathroomFurniture();
    drawLivingRoomFurniture(); 
    
    // Renderiza a Transparência
    drawWindows();

    glutSwapBuffers(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(800, 600); 
    glutCreateWindow("Projeto CG - Ambiente Simulado"); 

    init(); 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop(); 
    return 0;
}