#include "../../include/moveis.h"
#include "../../include/texturas_util.h"
#include "../../include/render/primitives.h"

#include <GL/glut.h>

static const float FURN_SCALE = 1.5f;

static void drawTabletopWithOakTop(float halfW, float halfH, float halfD,
                                   float sideR, float sideG, float sideB) {
    GLfloat sideMat[] = { sideR, sideG, sideB, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sideMat);
    glDisable(GL_TEXTURE_2D);
    const float yb = -halfH;
    const float yt = halfH;
    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-halfW, yb, -halfD);
        glVertex3f( halfW, yb, -halfD);
        glVertex3f( halfW, yb,  halfD);
        glVertex3f(-halfW, yb,  halfD);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-halfW, yb, -halfD);
        glVertex3f(-halfW, yb,  halfD);
        glVertex3f(-halfW, yt,  halfD);
        glVertex3f(-halfW, yt, -halfD);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(halfW, yb,  halfD);
        glVertex3f(halfW, yb, -halfD);
        glVertex3f(halfW, yt, -halfD);
        glVertex3f(halfW, yt,  halfD);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-halfW, yb, -halfD);
        glVertex3f( halfW, yb, -halfD);
        glVertex3f( halfW, yt, -halfD);
        glVertex3f(-halfW, yt, -halfD);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(halfW, yb, halfD);
        glVertex3f(-halfW, yb, halfD);
        glVertex3f(-halfW, yt, halfD);
        glVertex3f(halfW, yt, halfD);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMesaTampo);
    GLfloat topAmb[] = { 0.28f, 0.28f, 0.26f, 1.0f };
    GLfloat topDif[] = { 0.95f, 0.95f, 0.93f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, topAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, topDif);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    const float tile = 1.3f;
    float repU = (2.0f * halfW) / tile;
    float repV = (2.0f * halfD) / tile;
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);  glVertex3f(-halfW, yt, -halfD);
        glTexCoord2f(0.0f, repV);  glVertex3f(-halfW, yt,  halfD);
        glTexCoord2f(repU, repV);  glVertex3f( halfW, yt,  halfD);
        glTexCoord2f(repU, 0.0f);  glVertex3f( halfW, yt, -halfD);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

static void drawTexturedLegBox(float hx, float hy, float hz) {
    render::drawTexturedBox6(texMesaPe, hx, hy, hz, 1.8f, 0.24f, 0.86f);
}

static void drawChair(float tx, float tz, float rotYDeg) {
    const float scale = FURN_SCALE;
    const float ck = 2.0f;
    const float shx = 0.23f * scale;
    const float shy = 0.04f * scale;
    const float shz = 0.21f * scale;
    const float bhx = 0.22f * scale;
    const float bhy = 0.24f * scale;
    const float bhz = 0.045f * scale;
    const float lhx = 0.035f * scale;
    const float lhy = 0.22f * scale;
    const float lhz = 0.035f * scale;

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.44f * scale, 0.0f);
    render::drawTexturedBox6(texCadeiras, shx, shy, shz, ck, 0.24f, 0.86f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.72f * scale, -0.18f * scale);
    render::drawTexturedBox6(texCadeiras, bhx, bhy, bhz, ck, 0.24f, 0.86f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.17f * scale, 0.22f * scale, -0.15f * scale);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck, 0.24f, 0.86f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f * scale, 0.22f * scale, -0.15f * scale);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck, 0.24f, 0.86f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.17f * scale, 0.22f * scale, 0.15f * scale);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck, 0.24f, 0.86f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f * scale, 0.22f * scale, 0.15f * scale);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck, 0.24f, 0.86f);
    glPopMatrix();

    glPopMatrix();
}

void drawDiningTable() {
    const float scale = FURN_SCALE;
    const float tableX = 0.5f;
    const float tableZ = 4.5f;

    glPushMatrix();
    glTranslatef(tableX, 0.5f * scale, tableZ);

    glPushMatrix();
        drawTabletopWithOakTop(0.6f * scale, 0.05f * scale, 1.2f * scale, 0.6f, 0.4f, 0.2f);
    glPopMatrix();

    const float px = 0.5f * scale;
    const float pz = 1.0f * scale;
    const float lhx = 0.05f * scale;
    const float lhy = 0.45f * scale;
    const float lhz = 0.05f * scale;

    glPushMatrix(); glTranslatef(-px, -0.45f * scale,  pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();
    glPushMatrix(); glTranslatef( px, -0.45f * scale,  pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();
    glPushMatrix(); glTranslatef(-px, -0.45f * scale, -pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();
    glPushMatrix(); glTranslatef( px, -0.45f * scale, -pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();

    glPopMatrix();

    drawChair(tableX - 0.9f * scale, tableZ - 0.6f * scale, 90.0f);
    drawChair(tableX - 0.9f * scale, tableZ + 0.6f * scale, 90.0f);
    drawChair(tableX + 0.9f * scale, tableZ - 0.6f * scale, -90.0f);
    drawChair(tableX + 0.9f * scale, tableZ + 0.6f * scale, -90.0f);
    drawChair(tableX, tableZ - 1.5f * scale, 0.0f);
    drawChair(tableX, tableZ + 1.5f * scale, 180.0f);
}
