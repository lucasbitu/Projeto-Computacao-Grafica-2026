#include <GL/glut.h> 
#include <stdlib.h>  
#include <math.h>    
#include <iostream> 

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

// Variáveis para guardar os IDs das texturas
GLuint texPiso;
GLuint texParede;
GLuint texGramado;
GLuint texTelhado;
GLuint texGesso; 

// Posição inicial da nossa câmera no mundo 3D
float camX = 0.0f; 
float camY = 2.0f; 
float camZ = 5.0f; 

// Direção do olhar
float camYaw = 0.0f;
float camPitch = 0.0f;

static const float CAM_PITCH_LIMIT = 1.55f; 

// Função para carregar a textura 
GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) { 
        if (nrChannels == 3) { 
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); 
        } else if (nrChannels == 4) { 
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data); 
        }
        stbi_image_free(data);
    } else { 
        std::cerr << "Falha ao carregar textura: " << filename << std::endl; 
    }
    
    return textureID;
}

void init(void) {
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); 
    glEnable(GL_DEPTH_TEST); 
    glShadeModel(GL_SMOOTH); 

    // LUZ 0
    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // LUZ 1 (Sol)
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
}

void reshape(int w, int h) {
    if (h == 0) h = 1; 
    float aspect = (float)w / (float)h; 

    glViewport(0, 0, w, h); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(60.0, aspect, 0.1, 100.0); 
    glMatrixMode(GL_MODELVIEW); 
}

void drawGround() {
    glDisable(GL_LIGHTING); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBindTexture(GL_TEXTURE_2D, texGramado);

    const float y = -0.01f;
    const float size = 200.0f;

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f,   0.0f);   glVertex3f(-size, y, -size);
        glTexCoord2f(0.0f,   200.0f); glVertex3f(-size, y,  size);
        glTexCoord2f(200.0f, 200.0f); glVertex3f( size, y,  size);
        glTexCoord2f(200.0f, 0.0f);   glVertex3f( size, y, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_LIGHTING); 
}

void drawInternalWalls() {
    glBindTexture(GL_TEXTURE_2D, texParede); 
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    glBegin(GL_QUADS);
        // 1. PAREDE CENTRAL (Z = 0)
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 0.0f); 
        glTexCoord2f(10.0f, 0.0f); glVertex3f( 10.0f, 0.0f, 0.0f); 
        glTexCoord2f(10.0f, 2.0f); glVertex3f( 10.0f, 4.0f, 0.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 0.0f); 

        // 2. DIVISÓRIA SALA / ÁREA DA SUÍTE E BANHEIROS (X = 3)
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f,  10.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(3.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(3.0f, 4.0f,   0.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(3.0f, 4.0f,  10.0f); 

        // 3. DIVISÓRIA COZINHA / QUARTO 1 (X = -3)
        glNormal3f(1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -10.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(-3.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f,   0.0f); 

        // 4. DIVISÓRIA QUARTO 1 / QUARTO 2 (X = 4)
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(4.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(4.0f, 0.0f, -10.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(4.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(4.0f, 4.0f,   0.0f); 

        // 5. DIVISÓRIA BANHEIROS / SUÍTE (Z = 4)
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 3.0f, 0.0f, 4.0f); 
        glTexCoord2f(3.5f, 0.0f); glVertex3f(10.0f, 0.0f, 4.0f); 
        glTexCoord2f(3.5f, 2.0f); glVertex3f(10.0f, 4.0f, 4.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f( 3.0f, 4.0f, 4.0f); 

        // 6. DIVISÓRIA BANHEIRO SOCIAL / BANHEIRO DA SUÍTE (X = 6)
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(6.0f, 0.0f, 4.0f); 
        glTexCoord2f(2.0f, 0.0f); glVertex3f(6.0f, 0.0f, 0.0f); 
        glTexCoord2f(2.0f, 2.0f); glVertex3f(6.0f, 4.0f, 0.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(6.0f, 4.0f, 4.0f); 

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0); 
}

void drawHouse() {
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    // 1. O CHÃO
    glBindTexture(GL_TEXTURE_2D, texPiso); 
    glBegin(GL_QUADS); 
        glNormal3f(0.0f, 1.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f); 
    glEnd();

    // 2. AS PAREDES EXTERNAS
    glBindTexture(GL_TEXTURE_2D, texParede); 

    glBegin(GL_QUADS);
        // Parede do Fundo (Z = -10)
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f); 

        // Parede Esquerda (X = -10)
        glNormal3f(1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f,  10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f,  10.0f);

        // Parede Direita (X = 10)
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(10.0f, 0.0f,  10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f(10.0f, 4.0f,  10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(10.0f, 4.0f, -10.0f);

        // --- Parede da Frente com Porta Mais à Esquerda (Z = 10) ---
        glNormal3f(0.0f, 0.0f, -1.0f); 
        
        // Pedaço Esquerdo (X de -10 até -7.0)
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
        glTexCoord2f(0.6f, 0.0f); glVertex3f( -7.0f, 0.0f, 10.0f);
        glTexCoord2f(0.6f, 2.0f); glVertex3f( -7.0f, 4.0f, 10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f);

        // Pedaço Direito (X de -5.8 até 10)
        glTexCoord2f(0.84f, 0.0f); glVertex3f( -5.8f, 0.0f, 10.0f);
        glTexCoord2f(4.0f,  0.0f); glVertex3f( 10.0f, 0.0f, 10.0f);
        glTexCoord2f(4.0f,  2.0f); glVertex3f( 10.0f, 4.0f, 10.0f);
        glTexCoord2f(0.84f, 2.0f); glVertex3f( -5.8f, 4.0f, 10.0f);

        // Pedaço de Cima da porta (Y de 2.2 até 4.0, X de -7.0 até -5.8)
        glTexCoord2f(0.6f,  1.1f); glVertex3f( -7.0f, 2.2f, 10.0f);
        glTexCoord2f(0.84f, 1.1f); glVertex3f( -5.8f, 2.2f, 10.0f);
        glTexCoord2f(0.84f, 2.0f); glVertex3f( -5.8f, 4.0f, 10.0f);
        glTexCoord2f(0.6f,  2.0f); glVertex3f( -7.0f, 4.0f, 10.0f);
    glEnd();

    // 3. O TETO INTERNO
    glBindTexture(GL_TEXTURE_2D, texGesso); 
    
    GLfloat material_teto[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_teto);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 4.0f,  10.0f); 
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 4.0f,  10.0f); 
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 4.0f, -10.0f); 
    glEnd();

    // 4. ESTRUTURA DO TELHADO EXTERNO
    glBindTexture(GL_TEXTURE_2D, texTelhado); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    float picoY = 8.0f;
    float baseRoofY = 3.6f;
    float overX = 11.0f;    
    float overZ = 11.0f;    

    glBegin(GL_QUADS);
        // Água Esquerda
        glNormal3f(-0.707f, 0.707f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-overX, baseRoofY,  overZ); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-overX, baseRoofY, -overZ); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f(  0.0f,     picoY, -overZ); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(  0.0f,     picoY,  overZ); 

        // Água Direita
        glNormal3f(0.707f, 0.707f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f( overX, baseRoofY, -overZ); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f( overX, baseRoofY,  overZ); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f(  0.0f,     picoY,  overZ); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(  0.0f,     picoY, -overZ); 
    glEnd();

    // OS OITÕES
    glBindTexture(GL_TEXTURE_2D, texParede); 
    
    glBegin(GL_TRIANGLES);
        // Oitão Frontal (Z = 10)
        glNormal3f(0.0f, 0.0f, -1.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, 10.0f); 
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, 10.0f); 

        // Oitão Traseiro (Z = -10)
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f); 
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, -10.0f); 
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawTable() {
    glPushMatrix(); 
    glTranslatef(0.0f, 0.5f, -3.0f); 

    glPushMatrix(); 
        GLfloat tampo_difuso[] = { 0.6f, 0.4f, 0.2f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tampo_difuso);
        glScalef(2.0f, 0.1f, 1.0f);  
        glutSolidCube(1.0); 
    glPopMatrix(); 

    GLfloat perna_difusa[] = { 0.4f, 0.2f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, perna_difusa);

    glPushMatrix();
        glTranslatef(-0.9f, -0.45f, 0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.9f, -0.45f, 0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.9f, -0.45f, -0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.9f, -0.45f, -0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix(); 
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

    GLfloat light_position[] = { 0.0f, 3.5f, -3.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat sol_posicao[] = { 0.0f, 15.0f, 30.0f, 0.0f }; 
    glLightfv(GL_LIGHT1, GL_POSITION, sol_posicao);

    drawGround();
    drawHouse();
    drawInternalWalls(); 
    drawTable();

    glutSwapBuffers(); 
}

void keyboard(unsigned char key, int x, int y) {
    float velocidade = 0.5f; 

    float forwardX = sinf(camYaw);
    float forwardZ = -cosf(camYaw);
    float rightX = cosf(camYaw);
    float rightZ = sinf(camYaw);
    
    switch (key) {
        case 'w':
        case 'W':
            camX += forwardX * velocidade;
            camZ += forwardZ * velocidade;
            break;
        case 's':
        case 'S':
            camX -= forwardX * velocidade;
            camZ -= forwardZ * velocidade;
            break;
        case 'a':
        case 'A':
            camX -= rightX * velocidade;
            camZ -= rightZ * velocidade;
            break;
        case 'd':
        case 'D':
            camX += rightX * velocidade;
            camZ += rightZ * velocidade;
            break;
        case 'e': 
        case 'E':
            camY += velocidade;
            break;
        case 'q': 
        case 'Q':
            camY -= velocidade;
            if (camY < 0.5f) camY = 0.5f; 
            break;
        case 27: 
            exit(0); 
            break;             
    }
    
    glutPostRedisplay(); 
}

void specialKeys(int key, int x, int y) {
    (void)x;
    (void)y;
    float velocidadeOlhar = 0.04f; 

    switch (key) {
        case GLUT_KEY_LEFT:
            camYaw -= velocidadeOlhar;
            break;
        case GLUT_KEY_RIGHT:
            camYaw += velocidadeOlhar;
            break;
        case GLUT_KEY_UP:
            camPitch = fminf(camPitch + velocidadeOlhar, CAM_PITCH_LIMIT);
            break;
        case GLUT_KEY_DOWN:
            camPitch = fmaxf(camPitch - velocidadeOlhar, -CAM_PITCH_LIMIT);
            break;
        default:
            return;
    }

    glutPostRedisplay();
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