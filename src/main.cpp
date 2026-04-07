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

    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    /* Luz solar (direcional) — iluminação geral exterior */
    GLfloat sol_difusa[]   = { 0.45f, 0.45f, 0.45f, 1.0f };
    GLfloat sol_ambiente[] = { 0.18f, 0.18f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sol_difusa);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sol_ambiente);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Um ponto de luz no teto por cômodo (atenuação limita o alcance a cada divisão) */
    GLfloat luz_comodo_dif[] = { 0.75f, 0.75f, 0.72f, 1.0f };
    GLfloat luz_comodo_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    for (int i = 1; i <= 6; i++) {
        GLenum L = GL_LIGHT0 + i;
        glLightfv(L, GL_DIFFUSE, luz_comodo_dif);
        glLightfv(L, GL_AMBIENT, luz_comodo_amb);
        glLightfv(L, GL_SPECULAR, light_specular);
        glLightf(L, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(L, GL_LINEAR_ATTENUATION, 0.07f);
        glLightf(L, GL_QUADRATIC_ATTENUATION, 0.04f);
        glEnable(L);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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
    texMarmore2 = loadTexture("texturas/marmore_2.jpg");
    texFogaoFrente = loadTexture("texturas/fogao_frente.png");
    texFogaoCima = loadTexture("texturas/fogao_cima.png");
    texGuardaRoupa = loadTexture("texturas/guarda_roupa_frente.png");
    texMesaTampo = loadTexture("texturas/oak-cladding-1300-mm-architextures.jpg");
    texMesaPe = loadTexture("texturas/mesa_pe.jpg");
    texCadeiras = loadTexture("texturas/cadeiras.jpg");
    texSofa = loadTexture("texturas/sofa.jpg");
    texMesaTv = loadTexture("texturas/mesa_tv.jpg");
    texConteudoTv = loadTexture("texturas/conteudo_tv.jpg");
    texCabeceiraCama = loadTexture("texturas/cabeceira_cama.jpg");
    texBaseCama = loadTexture("texturas/base_cama.jpg");
    texColchaoCama = loadTexture("texturas/colchao_cama.jpg");

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

    GLfloat sol_posicao[] = { 0.0f, 15.0f, 30.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sol_posicao);

    updateRoomLightPositions();

    drawGround();
    drawHouse();
    drawInternalWalls();
    drawCeilingLampFixtures();
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

// A FUNÇÃO IDLE GARANTE QUE A ANIMAÇÃO CORRE SOZINHA
void idle() {
    if (tourAtivo) {
        updateTour();
        glutPostRedisplay();
    }
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
    
    // Regista a função idle para o Tour Virtual
    glutIdleFunc(idle); 

    glutMainLoop(); 
    return 0;
}