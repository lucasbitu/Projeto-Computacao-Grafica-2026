#include "../include/cenario.h"
#include "../include/texturas_util.h"
#include "../include/camera.h"
#include <math.h>

bool portaFrenteAberta = false; 
bool portaQ1Aberta = false;
bool portaQ2Aberta = false;
bool portaSuiteAberta = false;

const float DOOR_W = 1.6f;
/* Altura : largura = 2 : 1 (aberturas na parede e malha em drawDoors) */
const float DOOR_H = 2.0f * DOOR_W;

float calcularDistancia(float x1, float z1, float x2, float z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2));
}

/* Uma luz de teto por cômodo — posições alinhadas à luminária em drawCeilingLampFixtures */
static const int NUM_LUZES_TETO = 6;
static const float LUZ_TETO_X[NUM_LUZES_TETO] = {
    -6.5f, 0.5f, 7.0f, 0.5f, -6.5f, 6.5f
};
static const float LUZ_TETO_Z[NUM_LUZES_TETO] = {
    -5.0f, -5.0f, -5.0f, 5.5f, 5.5f, 7.0f
};
/* Teto interior em drawHouse: y = 4.0; fonte ligeiramente abaixo do plafon (sem sombras no GL fixo) */
static const float LUZ_TETO_Y = 3.88f;

void updateRoomLightPositions(void) {
    for (int i = 0; i < NUM_LUZES_TETO; i++) {
        GLfloat p[] = { LUZ_TETO_X[i], LUZ_TETO_Y, LUZ_TETO_Z[i], 1.0f };
        glLightfv(GL_LIGHT1 + i, GL_POSITION, p);
    }
}

static void drawOneCeilingLamp(float cx, float cz) {
    const float yBottom = 3.91f;
    const float yTop = 3.995f;
    const float hx = 0.32f;
    const float hz = 0.42f;
    const float x0 = cx - hx;
    const float x1 = cx + hx;
    const float z0 = cz - hz;
    const float z1 = cz + hz;

    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(x0, yBottom, z0);
        glVertex3f(x1, yBottom, z0);
        glVertex3f(x1, yBottom, z1);
        glVertex3f(x0, yBottom, z1);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(x0, yTop, z0);
        glVertex3f(x0, yTop, z1);
        glVertex3f(x1, yTop, z1);
        glVertex3f(x1, yTop, z0);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(x0, yBottom, z0);
        glVertex3f(x0, yBottom, z1);
        glVertex3f(x0, yTop, z1);
        glVertex3f(x0, yTop, z0);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(x1, yBottom, z1);
        glVertex3f(x1, yBottom, z0);
        glVertex3f(x1, yTop, z0);
        glVertex3f(x1, yTop, z1);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(x0, yBottom, z0);
        glVertex3f(x1, yBottom, z0);
        glVertex3f(x1, yTop, z0);
        glVertex3f(x0, yTop, z0);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x1, yBottom, z1);
        glVertex3f(x0, yBottom, z1);
        glVertex3f(x0, yTop, z1);
        glVertex3f(x1, yTop, z1);
    glEnd();
}

void drawCeilingLampFixtures(void) {
    glDisable(GL_TEXTURE_2D);
    GLfloat dif[] = { 0.9f, 0.9f, 0.88f, 1.0f };
    GLfloat emi[] = { 0.12f, 0.12f, 0.1f, 1.0f };
    GLfloat spec[] = { 0.35f, 0.35f, 0.35f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 24.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emi);

    for (int i = 0; i < NUM_LUZES_TETO; i++)
        drawOneCeilingLamp(LUZ_TETO_X[i], LUZ_TETO_Z[i]);

    GLfloat noEmi[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmi);
    glEnable(GL_TEXTURE_2D);
}

void drawSkybox() {
    glDisable(GL_LIGHTING); 

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texCeu);
    glColor3f(1.0f, 1.0f, 1.0f); 

    glPushMatrix();
        glTranslatef(camX, 0.0f, camZ); 
        
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        GLUquadric* quadric = gluNewQuadric();
        gluQuadricTexture(quadric, GL_TRUE);        
        gluQuadricOrientation(quadric, GLU_INSIDE); 
        
        glDepthMask(GL_FALSE);
        gluSphere(quadric, 300.0, 50, 50); 
        glDepthMask(GL_TRUE);
        
        gluDeleteQuadric(quadric);
    glPopMatrix();

    glEnable(GL_LIGHTING); 
}

void drawGround() {
    glDisable(GL_LIGHTING); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBindTexture(GL_TEXTURE_2D, texGramado);

    const float y = -0.01f;
    const float size = 500.0f;

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f,   0.0f);   glVertex3f(-size, y, -size);
        glTexCoord2f(0.0f,   500.0f); glVertex3f(-size, y,  size);
        glTexCoord2f(500.0f, 500.0f); glVertex3f( size, y,  size);
        glTexCoord2f(500.0f, 0.0f);   glVertex3f( size, y, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_LIGHTING); 
}
