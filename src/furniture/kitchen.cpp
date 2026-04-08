#include "../../include/moveis.h"
#include "../../include/texturas_util.h"
#include "../../include/render/primitives.h"

#include <GL/glut.h>

static const float BATH_BED_SCALE = 1.3f;

static void drawKitchenSinkTopOnCounter(float wx, float wyCounterTop, float wz, float rotYDeg) {
    const float pb = BATH_BED_SCALE;
    const float marmK = 1.1f;
    const float marmMa = 0.26f;
    const float marmMd = 0.88f;
    GLfloat porcelana[] = { 0.92f, 0.92f, 0.95f, 1.0f };
    GLfloat porcelanaAmb[] = { 0.35f, 0.35f, 0.38f, 1.0f };
    const float htx = 0.5f * 0.46f * pb;
    const float hty = 0.5f * 0.12f * pb;
    const float htz = 0.5f * 0.38f * pb;

    auto drawLouca = [&](float hx, float hy, float hz) {
        if (texMarmore2 != 0) {
            render::drawTexturedBox6(texMarmore2, hx, hy, hz, marmK, marmMa, marmMd);
        } else {
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
            glScalef(2.0f * hx, 2.0f * hy, 2.0f * hz);
            glutSolidCube(1.0f);
        }
    };

    glPushMatrix();
    glTranslatef(wx, wyCounterTop, wz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, hty, 0.03f * pb);
    drawLouca(htx, hty, htz);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    GLfloat cinzaMetalAmb[] = { 0.045f, 0.045f, 0.048f, 1.0f };
    GLfloat cinzaMetalDif[] = { 0.14f, 0.14f, 0.155f, 1.0f };
    GLfloat zeroSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cinzaMetalAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cinzaMetalDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glPushMatrix();
    glTranslatef(wx, wyCounterTop, wz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 2.0f * hty + 0.05f * pb, -0.18f * pb);
    glScalef(0.05f * pb, 0.14f * pb, 0.05f * pb);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawKitchenFurniture() {
    GLfloat mat_branca[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branca);

    glPushMatrix();
        glTranslatef(-9.0f, 1.0f, -9.2f);
        glScalef(0.8f, 2.0f, 0.8f);
        drawTexturedCube(texGeladeira, 0, 0);
    glPopMatrix();

    GLfloat mat_neutro[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_neutro);

    glPushMatrix();
        glTranslatef(-5.0f, 0.45f, -9.2f);
        glScalef(2.5f, 0.9f, 0.8f);
        drawTexturedCube(texMarmore, texMarmore, texMarmore);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branca);

    glPushMatrix();
        glTranslatef(-7.5f, 0.45f, -9.2f);
        glScalef(0.8f, 0.9f, 0.8f);
        drawTexturedCube(texFogaoFrente, 0, texFogaoCima);
    glPopMatrix();

    drawKitchenSinkTopOnCounter(-5.0f, 0.9f, -9.2f, 0.0f);
}
