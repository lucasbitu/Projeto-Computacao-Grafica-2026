#include "../../include/cenario.h"
#include "../../include/texturas_util.h"

#include <GL/glut.h>

/*
 * scene/doors.cpp
 * ---------------
 * Implementa malha de porta padrão e instancia portas do cenário com
 * transformação por dobradiça (rotação em torno da lateral).
 */

void drawDoorModel() {
    /* Cubo fino texturizado representando uma folha de porta. */
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
    /* Todas as portas compartilham a mesma textura-base. */
    glBindTexture(GL_TEXTURE_2D, texPorta);
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    /* Porta frontal (entrada principal). */
    glPushMatrix();
        const float frontDoorXa = -6.4f - 0.5f * DOOR_W;
        glTranslatef(frontDoorXa, 0.0f, 10.0f);
        glRotatef(portaFrenteAberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f);
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel();
    glPopMatrix();

    /* Porta do quarto 1. */
    glPushMatrix();
        glTranslatef(-0.2f, 0.0f, 0.0f);
        glRotatef(portaQ1Aberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f);
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel();
    glPopMatrix();

    /* Porta do quarto 2. */
    glPushMatrix();
        glTranslatef(6.2f, 0.0f, 0.0f);
        glRotatef(portaQ2Aberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f);
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel();
    glPopMatrix();

    /* Porta da suíte/banheiro interno. */
    glPushMatrix();
        glTranslatef(5.2f, 0.0f, 4.0f);
        glRotatef(portaSuiteAberta ? -90.0f : 0.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(DOOR_W / 2.0f, DOOR_H / 2.0f, 0.0f);
        glScalef(DOOR_W, DOOR_H, 0.1f);
        drawDoorModel();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
}
