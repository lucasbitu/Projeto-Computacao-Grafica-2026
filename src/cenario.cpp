#include "../include/cenario.h"
#include "../include/texturas_util.h"
#include "../include/camera.h"
#include <math.h>

bool portaFrenteAberta = false; 
bool portaQ1Aberta = false;
bool portaQ2Aberta = false;
bool portaSuiteAberta = false;

const float DOOR_H = 2.2f;
const float DOOR_W = 1.6f;

float calcularDistancia(float x1, float z1, float x2, float z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2));
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
    const float muretaTopoY = 1.7f;
    const float balcaoY = muretaTopoY + 0.03f;
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
        glTexCoord2f(10.0f, 2.0f); glVertex3f( -6.0f, 1.7f, 0.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 1.7f, 0.0f); 

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

    glBindTexture(GL_TEXTURE_2D, texPiso); 
    glBegin(GL_QUADS); 
        glNormal3f(0.0f, 1.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f); 
    glEnd();

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

void drawWindows() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D); 

    // Vidro transparente opcional (como as paredes não estão recortadas em malha, os vidros assentam na parede inteira)
    // Se não estiver a usar, pode deixar vazio ou com as janelas "cegas" se preferir no futuro
    
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND); 
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