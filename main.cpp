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
GLuint texPorta; 

// NOVAS TEXTURAS DA COZINHA
GLuint texGeladeira;
GLuint texMarmore;
GLuint texFogaoFrente;
GLuint texFogaoCima;

// Posição inicial da nossa câmera no mundo 3D
float camX = 0.0f; 
float camY = 2.0f; 
float camZ = 5.0f; 

// Direção do olhar
float camYaw = 0.0f;
float camPitch = 0.0f;

// ==========================================
// ESTADO DAS PORTAS E LÓGICA DE DISTÂNCIA
// ==========================================
bool portaFrenteAberta = false; 
bool portaQ1Aberta = false;
bool portaQ2Aberta = false;
bool portaSuiteAberta = false;

float calcularDistancia(float x1, float z1, float x2, float z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2));
}

static const float CAM_PITCH_LIMIT = 1.55f;

static const float DOOR_H = 2.2f;
static const float DOOR_W = 1.6f;

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

    // CARREGANDO AS TEXTURAS DOS MÓVEIS
    texGeladeira = loadTexture("texturas/geladeira.jpg");
    texMarmore = loadTexture("texturas/marmore.jpg");
    texFogaoFrente = loadTexture("texturas/fogao_frente.png");
    texFogaoCima = loadTexture("texturas/fogao_cima.png");
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

// ==========================================
// NOVO: Cubo Inteligente para Móveis Texturizados
// ==========================================
void drawTexturedCube(GLuint texFrente, GLuint texLados, GLuint texTopo) {
    // FRENTE (Z = 0.5)
    if (texFrente > 0) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texFrente); }
    else glDisable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glEnd();

    // LADOS (Trás, Esquerda, Direita, Baixo)
    if (texLados > 0) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texLados); }
    else glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        // Trás
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        // Esquerda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        // Direita
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        // Baixo
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();

    // TOPO (Y = 0.5)
    if (texTopo > 0) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texTopo); }
    else glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();
    
    // Garante que a textura volte a ficar ligada pro resto da cena
    glEnable(GL_TEXTURE_2D);
}
// ==========================================


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

    const float q1a = -0.2f, q1b = q1a + DOOR_W;
    const float q2a = 6.2f, q2b = q2a + DOOR_W;
    const float bathXa = 5.2f, bathXb = bathXa + DOOR_W;
    const float muretaTopoY = 1.7f;
    const float balcaoY = muretaTopoY + 0.03f;
    const float bzCoz = -0.55f;
    const float bzSala = 0.42f;

    glBegin(GL_QUADS);
        // 1. PAREDE CENTRAL (Z = 0)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f);
        glTexCoord2f(2.15f, 0.0f); glVertex3f(q1a, 0.0f, 0.0f);
        glTexCoord2f(2.15f, 2.0f); glVertex3f(q1a, 4.0f, 0.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f, 0.0f);

        glTexCoord2f(2.15f, 1.1f); glVertex3f(q1a, DOOR_H, 0.0f);
        glTexCoord2f(3.38f, 1.1f); glVertex3f(q1b, DOOR_H, 0.0f);
        glTexCoord2f(3.38f, 2.0f); glVertex3f(q1b, 4.0f, 0.0f);
        glTexCoord2f(2.15f, 2.0f); glVertex3f(q1a, 4.0f, 0.0f);

        glTexCoord2f(3.38f, 0.0f); glVertex3f(q1b, 0.0f, 0.0f);
        glTexCoord2f(7.08f, 0.0f); glVertex3f(q2a, 0.0f, 0.0f);
        glTexCoord2f(7.08f, 2.0f); glVertex3f(q2a, 4.0f, 0.0f);
        glTexCoord2f(3.38f, 2.0f); glVertex3f(q1b, 4.0f, 0.0f);

        glTexCoord2f(7.08f, 1.1f); glVertex3f(q2a, DOOR_H, 0.0f);
        glTexCoord2f(8.31f, 1.1f); glVertex3f(q2b, DOOR_H, 0.0f);
        glTexCoord2f(8.31f, 2.0f); glVertex3f(q2b, 4.0f, 0.0f);
        glTexCoord2f(7.08f, 2.0f); glVertex3f(q2a, 4.0f, 0.0f);

        glTexCoord2f(8.31f, 0.0f); glVertex3f(q2b, 0.0f, 0.0f);
        glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
        glTexCoord2f(10.0f, 2.0f); glVertex3f(10.0f, 4.0f, 0.0f);
        glTexCoord2f(8.31f, 2.0f); glVertex3f(q2b, 4.0f, 0.0f);

        // 2. DIVISÓRIA SALA / SUÍTE
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f,  10.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(3.0f, 0.0f,   4.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(3.0f, 4.0f,   4.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(3.0f, 4.0f,  10.0f); 

        // 3. DIVISÓRIA COZINHA / QUARTO 1
        glNormal3f(1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -10.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(-3.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f,   0.0f); 

        // 4. DIVISÓRIA QUARTO 1 / QUARTO 2
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(4.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(4.0f, 0.0f, -10.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(4.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(4.0f, 4.0f,   0.0f); 

        // 5. DIVISÓRIA BANHEIROS / SUÍTE
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 3.0f, 0.0f, 4.0f);
        glTexCoord2f(1.1f, 0.0f); glVertex3f(bathXa, 0.0f, 4.0f);
        glTexCoord2f(1.1f, 2.0f); glVertex3f(bathXa, 4.0f, 4.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f( 3.0f, 4.0f, 4.0f);

        glTexCoord2f(1.1f, 1.1f); glVertex3f(bathXa, DOOR_H, 4.0f);
        glTexCoord2f(1.9f, 1.1f); glVertex3f(bathXb, DOOR_H, 4.0f);
        glTexCoord2f(1.9f, 2.0f); glVertex3f(bathXb, 4.0f, 4.0f);
        glTexCoord2f(1.1f, 2.0f); glVertex3f(bathXa, 4.0f, 4.0f);

        glTexCoord2f(1.9f, 0.0f); glVertex3f(bathXb, 0.0f, 4.0f);
        glTexCoord2f(3.5f, 0.0f); glVertex3f(10.0f, 0.0f, 4.0f);
        glTexCoord2f(3.5f, 2.0f); glVertex3f(10.0f, 4.0f, 4.0f);
        glTexCoord2f(1.9f, 2.0f); glVertex3f(bathXb, 4.0f, 4.0f);

        // 6. MURETA COZINHA
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f); 
        glTexCoord2f(10.0f, 0.0f); glVertex3f( -6.0f, 0.0f, 0.0f); 
        glTexCoord2f(10.0f, 2.0f); glVertex3f( -6.0f, 1.7f, 0.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 1.7f, 0.0f); 

        // Tampo do balcão
        glBindTexture(GL_TEXTURE_2D, texPiso);
        GLfloat mat_balcao[] = { 0.88f, 0.86f, 0.82f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_balcao);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, balcaoY, bzCoz);
        glTexCoord2f(3.0f, 0.0f); glVertex3f(-3.0f, balcaoY, bzCoz);
        glTexCoord2f(3.0f, 1.0f); glVertex3f(-3.0f, balcaoY, bzSala);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, balcaoY, bzSala);
        glBindTexture(GL_TEXTURE_2D, texParede);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);
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
    const float frontDoorXa = -6.4f - 0.5f * DOOR_W;
    const float frontDoorXb = frontDoorXa + DOOR_W;

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f); 

        glNormal3f(1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f,  10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f,  10.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(10.0f, 0.0f,  10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f(10.0f, 4.0f,  10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(10.0f, 4.0f, -10.0f);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
        glTexCoord2f(0.56f, 0.0f); glVertex3f(frontDoorXa, 0.0f, 10.0f);
        glTexCoord2f(0.56f, 2.0f); glVertex3f(frontDoorXa, 4.0f, 10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f);

        glTexCoord2f(0.96f, 0.0f); glVertex3f(frontDoorXb, 0.0f, 10.0f);
        glTexCoord2f(4.0f,  0.0f); glVertex3f( 10.0f, 0.0f, 10.0f);
        glTexCoord2f(4.0f,  2.0f); glVertex3f( 10.0f, 4.0f, 10.0f);
        glTexCoord2f(0.96f, 2.0f); glVertex3f(frontDoorXb, 4.0f, 10.0f);

        glTexCoord2f(0.56f, 1.1f); glVertex3f(frontDoorXa, DOOR_H, 10.0f);
        glTexCoord2f(0.96f, 1.1f); glVertex3f(frontDoorXb, DOOR_H, 10.0f);
        glTexCoord2f(0.96f, 2.0f); glVertex3f(frontDoorXb, 4.0f, 10.0f);
        glTexCoord2f(0.56f, 2.0f); glVertex3f(frontDoorXa, 4.0f, 10.0f);
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
        glNormal3f(-0.707f, 0.707f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-overX, baseRoofY,  overZ); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-overX, baseRoofY, -overZ); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f(  0.0f,     picoY, -overZ); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(  0.0f,     picoY,  overZ); 

        glNormal3f(0.707f, 0.707f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f( overX, baseRoofY, -overZ); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f( overX, baseRoofY,  overZ); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f(  0.0f,     picoY,  overZ); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(  0.0f,     picoY, -overZ); 
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texParede); 
    glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.0f, -1.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, 10.0f); 
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, 10.0f); 

        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f); 
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, -10.0f); 
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawDoorModel() {
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f); 
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f); 
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f); 

        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.1f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.1f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.9f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.9f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.9f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.9f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.1f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 0.1f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();
}

void drawDoors() {
    glBindTexture(GL_TEXTURE_2D, texPorta); 
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    glPushMatrix();
        const float frontDoorXa = -6.4f - 0.5f * DOOR_W;
        glTranslatef(frontDoorXa, 0.0f, 10.0f);
        glRotatef(portaFrenteAberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f);
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel(); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.2f, 0.0f, 0.0f); 
        glRotatef(portaQ1Aberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f); 
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel(); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(6.2f, 0.0f, 0.0f); 
        glRotatef(portaQ2Aberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f); 
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel(); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.2f, 0.0f, 4.0f); 
        glRotatef(portaSuiteAberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f); 
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel(); 
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0); 
}

void drawTable() {
    glPushMatrix(); 
    glTranslatef(-4.5f, 0.5f, 3.0f); 

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

static void drawBed(float cx, float cz) {
    GLfloat colchao[] = { 0.52f, 0.5f, 0.62f, 1.0f };
    GLfloat cabeceira[] = { 0.38f, 0.24f, 0.14f, 1.0f };

    const float compX = 2.0f;
    const float largZ = 2.25f;

    glPushMatrix();
    glTranslatef(cx, 0.14f, cz);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colchao);
    glScalef(compX, 0.28f, largZ);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx, 0.52f, cz - 0.5f * largZ - 0.06f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cabeceira);
    glScalef(2.05f, 0.9f, 0.12f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

static void drawStudyDesk(float tx, float tz) {
    GLfloat tampo[] = { 0.52f, 0.36f, 0.2f, 1.0f };
    GLfloat perna[] = { 0.32f, 0.18f, 0.1f, 1.0f };

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);

    glPushMatrix();
    glTranslatef(0.0f, 0.76f, 0.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tampo);
    glScalef(1.35f, 0.07f, 0.58f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, perna);
    glPushMatrix();
    glTranslatef(-0.52f, 0.38f, 0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.52f, 0.38f, 0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.52f, 0.38f, -0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.52f, 0.38f, -0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

static void drawChair(float tx, float tz, float rotYDeg) {
    GLfloat assento[] = { 0.25f, 0.22f, 0.35f, 1.0f };
    GLfloat encosto[] = { 0.22f, 0.2f, 0.32f, 1.0f };

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, assento);
    glPushMatrix();
    glTranslatef(0.0f, 0.44f, 0.0f);
    glScalef(0.46f, 0.08f, 0.42f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, encosto);
    glPushMatrix();
    glTranslatef(0.0f, 0.72f, -0.18f);
    glScalef(0.44f, 0.48f, 0.09f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, assento);
    glPushMatrix();
    glTranslatef(-0.17f, 0.22f, -0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f, 0.22f, -0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.17f, 0.22f, 0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f, 0.22f, 0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawBedroomsFurniture() {
    /* Quarto 1: X -3..4, Z -10..0 — cama no fundo; mesa perto da porta (Z≈0) */
    drawBed(0.35f, -8.85f);
    drawStudyDesk(2.45f, -2.75f);
    drawChair(2.45f, -1.85f, 180.0f);

    /* Quarto 2: X 4..10, Z -10..0 */
    drawBed(7.0f, -8.85f);
    drawStudyDesk(8.4f, -2.75f);
    drawChair(8.4f, -1.85f, 180.0f);
}

// ==========================================
// MÓVEIS DA COZINHA ATUALIZADOS (Com Texturas)
// ==========================================
void drawKitchenFurniture() {
    // --- 1. GELADEIRA ---
    // Usamos um material branco base para a textura aparecer com as cores reais
    GLfloat mat_branca[] = { 0.8f, 0.8f, 0.8f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branca);

    glPushMatrix();
        glTranslatef(-9.0f, 1.0f, -9.2f); 
        glScalef(0.8f, 2.0f, 0.8f);
        // Usa a textura da geladeira só na frente. Os lados ficam com o material branco.
        drawTexturedCube(texGeladeira, 0, 0);
    glPopMatrix();

    // --- 2. BANCADA DA PIA ---
    GLfloat mat_neutro[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_neutro);
    
    glPushMatrix();
        glTranslatef(-5.0f, 0.45f, -9.2f); 
        glScalef(2.5f, 0.9f, 0.8f);
        // Usa a textura de mármore em todos os lados (frente, lados, topo)
        drawTexturedCube(texMarmore, texMarmore, texMarmore);
    glPopMatrix();

    // --- 3. FOGÃO ---
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branca);
    
    glPushMatrix();
        glTranslatef(-7.5f, 0.45f, -9.2f); 
        glScalef(0.8f, 0.9f, 0.8f);
        // Frente: fogao_frente; topo: fogao_cima; laterais sem textura (material branco).
        drawTexturedCube(texFogaoFrente, 0, texFogaoCima);
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
    drawDoors(); 
    drawTable();
    drawBedroomsFurniture();
    drawKitchenFurniture();

    glutSwapBuffers(); 
}

void keyboard(unsigned char key, int x, int y) {
    float velocidade = 0.5f; 

    float forwardX = sinf(camYaw);
    float forwardZ = -cosf(camYaw);
    float rightX = cosf(camYaw);
    float rightZ = sinf(camYaw);
    
    switch (key) {
        case 'w': case 'W':
            camX += forwardX * velocidade;
            camZ += forwardZ * velocidade;
            break;
        case 's': case 'S':
            camX -= forwardX * velocidade;
            camZ -= forwardZ * velocidade;
            break;
        case 'a': case 'A':
            camX -= rightX * velocidade;
            camZ -= rightZ * velocidade;
            break;
        case 'd': case 'D':
            camX += rightX * velocidade;
            camZ += rightZ * velocidade;
            break;
        case 'e': case 'E':
            camY += velocidade;
            break;
        case 'q': case 'Q':
            camY -= velocidade;
            if (camY < 0.5f) camY = 0.5f; 
            break;
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