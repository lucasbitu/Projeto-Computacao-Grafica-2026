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

void drawInternalWalls() {
    glBindTexture(GL_TEXTURE_2D, texParede); 
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    const float q1a = -0.2f, q1b = q1a + DOOR_W;
    const float q2a = 6.2f, q2b = q2a + DOOR_W;
    const float bathXa = 5.2f, bathXb = bathXa + DOOR_W;
    const float muretaTopoY = 1.32f;
    /* Tampo no mesmo nível do topo da mureta (sem folga vertical) */
    const float balcaoY = muretaTopoY;
    const float muretaTexV = muretaTopoY * (2.0f / 1.7f);
    const float bzCoz = -0.55f;
    const float bzSala = 0.42f;

    glBegin(GL_QUADS);
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

        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f,  10.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(3.0f, 0.0f,   4.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(3.0f, 4.0f,   4.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(3.0f, 4.0f,  10.0f); 

        glNormal3f(1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -10.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(-3.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f,   0.0f); 

        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(4.0f, 0.0f,   0.0f); 
        glTexCoord2f(5.0f, 0.0f); glVertex3f(4.0f, 0.0f, -10.0f); 
        glTexCoord2f(5.0f, 2.0f); glVertex3f(4.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(4.0f, 4.0f,   0.0f); 

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

        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f); 
        glTexCoord2f(10.0f, 0.0f); glVertex3f( -6.0f, 0.0f, 0.0f); 
        glTexCoord2f(10.0f, muretaTexV); glVertex3f( -6.0f, muretaTopoY, 0.0f); 
        glTexCoord2f(0.0f, muretaTexV); glVertex3f(-3.0f, muretaTopoY, 0.0f); 

        glBindTexture(GL_TEXTURE_2D, texPiso);
        GLfloat mat_balcao[] = { 0.88f, 0.86f, 0.82f, 1.0f };
        GLfloat amb_balcao[] = { 0.36f, 0.35f, 0.33f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb_balcao);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_balcao);
        /* CCW visto de +Y para normal geométrica (0,1,0) coincidir com a iluminação */
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, balcaoY, bzCoz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, balcaoY, bzSala);
        glTexCoord2f(3.0f, 1.0f); glVertex3f(-3.0f, balcaoY, bzSala);
        glTexCoord2f(3.0f, 0.0f); glVertex3f(-3.0f, balcaoY, bzCoz);
        glBindTexture(GL_TEXTURE_2D, texParede);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0); 
}

void drawHouse() {
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_piso_amb[] = { 0.32f, 0.32f, 0.34f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_piso_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    glBindTexture(GL_TEXTURE_2D, texPiso); 
    glBegin(GL_QUADS); 
        glNormal3f(0.0f, 1.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f); 
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texParede);
    /* Fachadas com iluminação: sol + LIGHT7 + luzes pontuais (TWO_SIDE em main). Textura modula o resultado. */
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    /* Ambiente um pouco alto nas fachadas: paredes laterais quase paralelas ao sol ficam menos “murchas” */
    GLfloat mat_parede_ext_amb[] = { 0.30f, 0.30f, 0.32f, 1.0f };
    GLfloat mat_parede_ext_dif[] = { 0.88f, 0.88f, 0.90f, 1.0f };
    GLfloat mat_parede_no_spec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_parede_ext_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_parede_ext_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_parede_no_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

    const float frontDoorXa = -6.4f - 0.5f * DOOR_W;
    const float frontDoorXb = frontDoorXa + DOOR_W;
    const float picoY = 8.0f;
    const float winY0 = 1.2f;
    const float winY1 = 2.8f;
    const float backWin1Xa = -8.5f, backWin1Xb = -5.5f;
    const float backWin3Xa = -1.0f, backWin3Xb = 2.0f;
    const float backWin2Xa = 5.5f, backWin2Xb = 8.5f;
    const float frontWinXa = -3.0f, frontWinXb = 0.0f;
    auto frontU = [](float x) { return (x + 10.0f) / 5.0f; };
    auto frontV = [](float y) { return y / 2.0f; };

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 0.6f); glVertex3f( 10.0f, winY0, -10.0f);
        glTexCoord2f(0.0f, 0.6f); glVertex3f(-10.0f, winY0, -10.0f);

        glTexCoord2f(0.0f, 1.4f); glVertex3f(-10.0f, winY1, -10.0f);
        glTexCoord2f(4.0f, 1.4f); glVertex3f( 10.0f, winY1, -10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f);

        glTexCoord2f(0.0f, 0.6f);  glVertex3f(-10.0f, winY0, -10.0f);
        glTexCoord2f(0.3f, 0.6f);  glVertex3f(backWin1Xa, winY0, -10.0f);
        glTexCoord2f(0.3f, 1.4f);  glVertex3f(backWin1Xa, winY1, -10.0f);
        glTexCoord2f(0.0f, 1.4f);  glVertex3f(-10.0f, winY1, -10.0f);

        glTexCoord2f(0.9f, 0.6f);  glVertex3f(backWin1Xb, winY0, -10.0f);
        glTexCoord2f(1.8f, 0.6f);  glVertex3f(backWin3Xa, winY0, -10.0f);
        glTexCoord2f(1.8f, 1.4f);  glVertex3f(backWin3Xa, winY1, -10.0f);
        glTexCoord2f(0.9f, 1.4f);  glVertex3f(backWin1Xb, winY1, -10.0f);

        glTexCoord2f(2.4f, 0.6f);  glVertex3f(backWin3Xb, winY0, -10.0f);
        glTexCoord2f(3.1f, 0.6f);  glVertex3f(backWin2Xa, winY0, -10.0f);
        glTexCoord2f(3.1f, 1.4f);  glVertex3f(backWin2Xa, winY1, -10.0f);
        glTexCoord2f(2.4f, 1.4f);  glVertex3f(backWin3Xb, winY1, -10.0f);

        glTexCoord2f(3.7f, 0.6f);  glVertex3f(backWin2Xb, winY0, -10.0f);
        glTexCoord2f(4.0f, 0.6f);  glVertex3f(10.0f, winY0, -10.0f);
        glTexCoord2f(4.0f, 1.4f);  glVertex3f(10.0f, winY1, -10.0f);
        glTexCoord2f(3.7f, 1.4f);  glVertex3f(backWin2Xb, winY1, -10.0f);

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
        glTexCoord2f(frontU(-10.0f), frontV(0.0f)); glVertex3f(-10.0f, 0.0f, 10.0f);
        glTexCoord2f(frontU(frontDoorXa), frontV(0.0f)); glVertex3f(frontDoorXa, 0.0f, 10.0f);
        glTexCoord2f(frontU(frontDoorXa), frontV(4.0f)); glVertex3f(frontDoorXa, 4.0f, 10.0f);
        glTexCoord2f(frontU(-10.0f), frontV(4.0f)); glVertex3f(-10.0f, 4.0f, 10.0f);

        glTexCoord2f(frontU(frontDoorXb), frontV(0.0f)); glVertex3f(frontDoorXb, 0.0f, 10.0f);
        glTexCoord2f(frontU(frontWinXa), frontV(0.0f)); glVertex3f(frontWinXa, 0.0f, 10.0f);
        glTexCoord2f(frontU(frontWinXa), frontV(4.0f)); glVertex3f(frontWinXa, 4.0f, 10.0f);
        glTexCoord2f(frontU(frontDoorXb), frontV(4.0f)); glVertex3f(frontDoorXb, 4.0f, 10.0f);

        glTexCoord2f(frontU(frontWinXb), frontV(0.0f)); glVertex3f(frontWinXb, 0.0f, 10.0f);
        glTexCoord2f(frontU(10.0f), frontV(0.0f)); glVertex3f(10.0f, 0.0f, 10.0f);
        glTexCoord2f(frontU(10.0f), frontV(4.0f)); glVertex3f(10.0f, 4.0f, 10.0f);
        glTexCoord2f(frontU(frontWinXb), frontV(4.0f)); glVertex3f(frontWinXb, 4.0f, 10.0f);

        glTexCoord2f(frontU(frontDoorXa), frontV(DOOR_H)); glVertex3f(frontDoorXa, DOOR_H, 10.0f);
        glTexCoord2f(frontU(frontDoorXb), frontV(DOOR_H)); glVertex3f(frontDoorXb, DOOR_H, 10.0f);
        glTexCoord2f(frontU(frontDoorXb), frontV(4.0f)); glVertex3f(frontDoorXb, 4.0f, 10.0f);
        glTexCoord2f(frontU(frontDoorXa), frontV(4.0f)); glVertex3f(frontDoorXa, 4.0f, 10.0f);

        glTexCoord2f(frontU(frontWinXa), frontV(0.0f)); glVertex3f(frontWinXa, 0.0f, 10.0f);
        glTexCoord2f(frontU(frontWinXb), frontV(0.0f)); glVertex3f(frontWinXb, 0.0f, 10.0f);
        glTexCoord2f(frontU(frontWinXb), frontV(winY0)); glVertex3f(frontWinXb, winY0, 10.0f);
        glTexCoord2f(frontU(frontWinXa), frontV(winY0)); glVertex3f(frontWinXa, winY0, 10.0f);

        glTexCoord2f(frontU(frontWinXa), frontV(winY1)); glVertex3f(frontWinXa, winY1, 10.0f);
        glTexCoord2f(frontU(frontWinXb), frontV(winY1)); glVertex3f(frontWinXb, winY1, 10.0f);
        glTexCoord2f(frontU(frontWinXb), frontV(4.0f)); glVertex3f(frontWinXb, 4.0f, 10.0f);
        glTexCoord2f(frontU(frontWinXa), frontV(4.0f)); glVertex3f(frontWinXa, 4.0f, 10.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
        /* Mesma normal que os quads da fachada z = +10 (0,0,-1); CCW visto de dentro (-Z) */
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, 10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f);
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, 10.0f);

        /* Igual à parede z = -10: normal (0,0,1); ordem CCW vista de dentro (+Z) */
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f);
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, -10.0f);
    glEnd();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, texGesso);
    GLfloat material_teto[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_teto_amb[] = { 0.40f, 0.40f, 0.42f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_teto_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_teto);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 4.0f,  10.0f);
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 4.0f,  10.0f);
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 4.0f, -10.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texTelhado);
    /* Telhado: menos ambiente/difuso que a parede baixa (evita faixa muito clara junto ao frontão) */
    GLfloat mat_telhado_amb[] = { 0.12f, 0.12f, 0.13f, 1.0f };
    GLfloat mat_telhado_dif[] = { 0.52f, 0.52f, 0.54f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_telhado_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_telhado_dif);
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

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawWindows() {
    const float winY0 = 1.2f;
    const float winY1 = 2.8f;
    const float backWin1Xa = -8.5f, backWin1Xb = -5.5f;
    const float backWin3Xa = -1.0f, backWin3Xb = 2.0f;
    const float backWin2Xa = 5.5f, backWin2Xb = 8.5f;
    const float frontWinXa = -3.0f, frontWinXb = 0.0f;
    const float frameHalf = 0.06f;
    const float frameDepth = 0.055f;

    glDisable(GL_TEXTURE_2D);
    GLfloat frameAmb[] = { 0.22f, 0.18f, 0.12f, 1.0f };
    GLfloat frameDif[] = { 0.48f, 0.40f, 0.26f, 1.0f };
    GLfloat frameSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, frameAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, frameDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, frameSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);

    auto drawFrameOnZ = [&](float xa, float xb, float y0, float y1, float zPlane) {
        const float cx = 0.5f * (xa + xb);
        const float cy = 0.5f * (y0 + y1);
        const float hx = 0.5f * (xb - xa);
        const float hy = 0.5f * (y1 - y0);
        glPushMatrix();
        glTranslatef(cx, y1 - frameHalf, zPlane);
        glScalef(hx * 2.0f, frameHalf * 2.0f, frameDepth);
        glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(cx, y0 + frameHalf, zPlane);
        glScalef(hx * 2.0f, frameHalf * 2.0f, frameDepth);
        glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(xa + frameHalf, cy, zPlane);
        glScalef(frameHalf * 2.0f, hy * 2.0f, frameDepth);
        glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(xb - frameHalf, cy, zPlane);
        glScalef(frameHalf * 2.0f, hy * 2.0f, frameDepth);
        glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(cx, cy, zPlane);
        glScalef(frameHalf * 1.3f, (hy - frameHalf) * 2.0f, frameDepth);
        glutSolidCube(1.0f);
        glPopMatrix();
    };

    drawFrameOnZ(backWin1Xa, backWin1Xb, winY0, winY1, -10.0f);
    drawFrameOnZ(backWin3Xa, backWin3Xb, winY0, winY1, -10.0f);
    drawFrameOnZ(backWin2Xa, backWin2Xb, winY0, winY1, -10.0f);
    drawFrameOnZ(frontWinXa, frontWinXb, winY0, winY1, 10.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    GLfloat vidroAmb[] = { 0.10f, 0.16f, 0.20f, 0.35f };
    GLfloat vidroDif[] = { 0.24f, 0.46f, 0.60f, 0.35f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, vidroAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, vidroDif);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(backWin1Xa + frameHalf, winY0 + frameHalf, -9.99f);
        glVertex3f(backWin1Xb - frameHalf, winY0 + frameHalf, -9.99f);
        glVertex3f(backWin1Xb - frameHalf, winY1 - frameHalf, -9.99f);
        glVertex3f(backWin1Xa + frameHalf, winY1 - frameHalf, -9.99f);

        glVertex3f(backWin3Xa + frameHalf, winY0 + frameHalf, -9.99f);
        glVertex3f(backWin3Xb - frameHalf, winY0 + frameHalf, -9.99f);
        glVertex3f(backWin3Xb - frameHalf, winY1 - frameHalf, -9.99f);
        glVertex3f(backWin3Xa + frameHalf, winY1 - frameHalf, -9.99f);

        glVertex3f(backWin2Xa + frameHalf, winY0 + frameHalf, -9.99f);
        glVertex3f(backWin2Xb - frameHalf, winY0 + frameHalf, -9.99f);
        glVertex3f(backWin2Xb - frameHalf, winY1 - frameHalf, -9.99f);
        glVertex3f(backWin2Xa + frameHalf, winY1 - frameHalf, -9.99f);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(frontWinXa + frameHalf, winY0 + frameHalf, 9.99f);
        glVertex3f(frontWinXb - frameHalf, winY0 + frameHalf, 9.99f);
        glVertex3f(frontWinXb - frameHalf, winY1 - frameHalf, 9.99f);
        glVertex3f(frontWinXa + frameHalf, winY1 - frameHalf, 9.99f);

    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND); 
    glEnable(GL_TEXTURE_2D);
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