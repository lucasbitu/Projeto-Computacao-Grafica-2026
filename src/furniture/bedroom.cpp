#include "../../include/moveis.h"
#include "../../include/texturas_util.h"
#include "../../include/render/primitives.h"

#include <GL/glut.h>

static const float furnitureScale = 1.5f;
static const float bedBathScale = 1.3f;
static const float quartoParedeFundoZ = -10.0f;

static void drawTexturedLegBox(float halfX, float halfY, float halfZ) {
    render::drawTexturedBox6(texMesaPe, halfX, halfY, halfZ, 1.8f);
}

static void drawBedroomChair(float tx, float tz, float rotYDeg) {
    const float s = furnitureScale;
    const float ck = 2.0f;
    const float shx = 0.23f * s;
    const float shy = 0.04f * s;
    const float shz = 0.21f * s;
    const float bhx = 0.22f * s;
    const float bhy = 0.24f * s;
    const float bhz = 0.045f * s;
    const float lhx = 0.035f * s;
    const float lhy = 0.22f * s;
    const float lhz = 0.035f * s;

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.44f * s, 0.0f);
    render::drawTexturedBox6(texCadeiras, shx, shy, shz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.72f * s, -0.18f * s);
    render::drawTexturedBox6(texCadeiras, bhx, bhy, bhz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.17f * s, 0.22f * s, -0.15f * s);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.17f * s, 0.22f * s, -0.15f * s);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.17f * s, 0.22f * s, 0.15f * s);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.17f * s, 0.22f * s, 0.15f * s);
    render::drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();

    glPopMatrix();
}

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

static void drawBedHeadboard(float cx, float cz, float b, float largZ) {
    GLfloat cabeceira[] = { 0.38f, 0.24f, 0.14f, 1.0f };
    GLfloat cabAmb[] = { 0.12f, 0.08f, 0.06f, 1.0f };
    GLfloat zsp[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glPushMatrix();
    glTranslatef(cx, 0.59f * b, cz - 0.5f * largZ - 0.06f * b);
    glScalef(2.05f * b, 0.9f * b, 0.12f * b);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cabAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cabeceira);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zsp);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f,  0.5f);
        glVertex3f(0.5f,  0.5f,  0.5f);
        glVertex3f(0.5f,  0.5f, -0.5f);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f,  0.5f);
        glVertex3f( 0.5f, 0.5f,  0.5f);
        glVertex3f( 0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();

    if (texCabeceiraCama != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texCabeceiraCama);
        GLfloat mAmb[] = { 0.32f, 0.32f, 0.30f, 1.0f };
        GLfloat mDif[] = { 0.90f, 0.90f, 0.88f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDif);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, 0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, 0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, 0.5f);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cabAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cabeceira);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f( 0.5f, -0.5f, 0.5f);
            glVertex3f( 0.5f,  0.5f, 0.5f);
            glVertex3f(-0.5f,  0.5f, 0.5f);
        glEnd();
    }

    glPopMatrix();
}

static void drawBed(float cx, float cz) {
    const float b = bedBathScale;
    const float compX = 2.0f * b;
    const float largZ = 2.25f * b;
    const float hBase = 0.14f * b;
    const float hColchao = 0.21f * b;
    const float colchaoEncolher = 0.97f;

    GLfloat baseMadeira[] = { 0.30f, 0.20f, 0.14f, 1.0f };
    GLfloat baseAmb[] = { 0.08f, 0.05f, 0.04f, 1.0f };
    GLfloat colchao[] = { 0.54f, 0.51f, 0.66f, 1.0f };
    GLfloat colchaoAmb[] = { 0.20f, 0.19f, 0.26f, 1.0f };

    const float hxB = 0.5f * compX;
    const float hyB = 0.5f * hBase;
    const float hzB = 0.5f * largZ;
    glPushMatrix();
    glTranslatef(cx, hyB, cz);
    if (texBaseCama != 0) {
        render::drawTexturedBox6(texBaseCama, hxB, hyB, hzB, 0.38f, 0.28f, 0.88f);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, baseAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, baseMadeira);
        glScalef(compX, hBase, largZ);
        glutSolidCube(1.0f);
    }
    glPopMatrix();

    const float compM = compX * colchaoEncolher;
    const float largM = largZ * colchaoEncolher;
    const float hxC = 0.5f * compM;
    const float hyC = 0.5f * hColchao;
    const float hzC = 0.5f * largM;
    glPushMatrix();
    glTranslatef(cx, hBase + hyC, cz);
    if (texColchaoCama != 0) {
        render::drawTexturedBox6(texColchaoCama, hxC, hyC, hzC, 0.42f, 0.30f, 0.90f);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colchaoAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colchao);
        glScalef(compM, hColchao, largM);
        glutSolidCube(1.0f);
    }
    glPopMatrix();

    drawBedHeadboard(cx, cz, b, largZ);
}

static void drawStudyDesk(float tx, float tz) {
    const float s = furnitureScale;
    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);

    glPushMatrix();
    glTranslatef(0.0f, 0.76f * s, 0.0f);
    drawTabletopWithOakTop(0.675f * s, 0.035f * s, 0.29f * s, 0.52f, 0.36f, 0.2f);
    glPopMatrix();

    const float dhx = 0.04f * s;
    const float dhy = 0.38f * s;
    const float dhz = 0.04f * s;
    glPushMatrix();
    glTranslatef(-0.52f * s, 0.38f * s, 0.22f * s);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.52f * s, 0.38f * s, 0.22f * s);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.52f * s, 0.38f * s, -0.22f * s);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.52f * s, 0.38f * s, -0.22f * s);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();

    glPopMatrix();
}

static void drawWardrobe(float tx, float tz, float rotYDeg) {
    GLfloat mat_branco[] = { 0.95f, 0.95f, 0.95f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branco);

    glPushMatrix();
        glTranslatef(tx, 1.1f, tz);
        glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);

        glPushMatrix();
            glScalef(1.5f, 2.2f, 0.6f);
            glDisable(GL_TEXTURE_2D);
            glutSolidCube(1.0);
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texGuardaRoupa);

        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.24f, 0.08f); glVertex3f(-0.75f, -1.1f, 0.301f);
            glTexCoord2f(0.76f, 0.08f); glVertex3f( 0.75f, -1.1f, 0.301f);
            glTexCoord2f(0.76f, 0.88f); glVertex3f( 0.75f,  1.1f, 0.301f);
            glTexCoord2f(0.24f, 0.88f); glVertex3f(-0.75f,  1.1f, 0.301f);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

void drawBedroomsFurniture() {
    const float bedZ = quartoParedeFundoZ + 0.08f + 1.245f * bedBathScale;
    drawBed(0.35f, bedZ);
    drawStudyDesk(2.45f, -2.75f);
    drawBedroomChair(2.45f, -1.85f, 180.0f);
    drawWardrobe(-2.62f, -5.0f, 90.0f);

    drawBed(7.0f, bedZ);
    drawStudyDesk(8.4f, -2.75f);
    drawBedroomChair(8.4f, -1.85f, 180.0f);
    drawWardrobe(9.62f, -5.0f, -90.0f);
}
