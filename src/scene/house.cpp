#include "../../include/cenario.h"
#include "../../include/texturas_util.h"

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
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, 10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f);
        glTexCoord2f(2.0f, 4.0f); glVertex3f(  0.0f, picoY, 10.0f);

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
