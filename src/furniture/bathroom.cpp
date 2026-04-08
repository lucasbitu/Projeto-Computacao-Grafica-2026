#include "../../include/moveis.h"
#include "../../include/texturas_util.h"
#include "../../include/render/primitives.h"

#include <GL/glut.h>

void drawBathroomFurniture() {
    GLfloat porcelana[] = { 0.92f, 0.92f, 0.95f, 1.0f };
    GLfloat porcelanaAmb[] = { 0.35f, 0.35f, 0.38f, 1.0f };
    GLfloat aguaAmb[] = { 0.12f, 0.22f, 0.34f, 1.0f };
    GLfloat aguaDif[] = { 0.32f, 0.58f, 0.78f, 0.42f };

    const float bathBedScale = 1.3f;
    const float bx = 9.05f;
    const float by = 0.22f;
    const float bz = 7.0f;
    const float tubS = 1.12f;
    const float hx = 0.5f * tubS;
    const float hy = 0.21f * tubS;
    const float hz = 1.025f * tubS;
    const float wt = 0.048f * tubS;
    const float ix = hx - wt;
    const float iz = hz - wt;
    const float yInBottom = -hy + wt;
    const float yInTop = hy - wt;
    const float yWater = yInTop - 0.055f;
    const float yWBot = yInBottom + 0.015f;
    const float inset = 0.012f;
    const float wx = ix - inset;
    const float wz = iz - inset;
    const float tubMarmK = 1.05f;
    const float tubMarmMa = 0.26f;
    const float tubMarmMd = 0.88f;

    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);

    glPushMatrix();
    glTranslatef(bx, by, bz);

    auto drawTubBox = [&](float cx, float cy, float cz, float hfx, float hfy, float hfz) {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        if (texMarmore2 != 0) {
            render::drawTexturedBox6(texMarmore2, hfx, hfy, hfz, tubMarmK, tubMarmMa, tubMarmMd);
        } else {
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
            glScalef(2.0f * hfx, 2.0f * hfy, 2.0f * hfz);
            glutSolidCube(1.0f);
        }
        glPopMatrix();
    };

    drawTubBox(0.0f, -hy + 0.5f * wt, 0.0f, hx + wt, 0.5f * wt, hz + wt);
    drawTubBox(-hx - 0.5f * wt, 0.0f, 0.0f, 0.5f * wt, hy, hz + wt);
    drawTubBox(hx + 0.5f * wt, 0.0f, 0.0f, 0.5f * wt, hy, hz + wt);
    drawTubBox(0.0f, 0.0f, -hz - 0.5f * wt, hx, hy, 0.5f * wt);
    drawTubBox(0.0f, 0.0f, hz + 0.5f * wt, hx, hy, 0.5f * wt);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, aguaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, aguaDif);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-wx, yWater, -wz);
        glVertex3f(-wx, yWater,  wz);
        glVertex3f( wx, yWater,  wz);
        glVertex3f( wx, yWater, -wz);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-wx, yWBot, -wz);
        glVertex3f( wx, yWBot, -wz);
        glVertex3f( wx, yWBot,  wz);
        glVertex3f(-wx, yWBot,  wz);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-wx, yWBot, -wz);
        glVertex3f(-wx, yWBot,  wz);
        glVertex3f(-wx, yWater,  wz);
        glVertex3f(-wx, yWater, -wz);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(wx, yWBot,  wz);
        glVertex3f(wx, yWBot, -wz);
        glVertex3f(wx, yWater, -wz);
        glVertex3f(wx, yWater,  wz);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-wx, yWBot, -wz);
        glVertex3f(-wx, yWater, -wz);
        glVertex3f( wx, yWater, -wz);
        glVertex3f( wx, yWBot, -wz);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-wx, yWBot, wz);
        glVertex3f( wx, yWBot, wz);
        glVertex3f( wx, yWater, wz);
        glVertex3f(-wx, yWater, wz);
    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();

    const float pb = bathBedScale;
    const float piaPx = 3.58f, piaPz = 6.4f;
    const float marmK = 1.1f;
    const float marmMa = 0.26f;
    const float marmMd = 0.88f;

    auto drawLoucaOuMarmore = [&](float hfx, float hfy, float hfz) {
        if (texMarmore2 != 0) {
            render::drawTexturedBox6(texMarmore2, hfx, hfy, hfz, marmK, marmMa, marmMd);
        } else {
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
            glScalef(2.0f * hfx, 2.0f * hfy, 2.0f * hfz);
            glutSolidCube(1.0f);
        }
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.34f * pb, 0.0f);
    drawLoucaOuMarmore(0.5f * 0.40f * pb, 0.5f * 0.68f * pb, 0.5f * 0.44f * pb);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.71f * pb, 0.03f * pb);
    drawLoucaOuMarmore(0.5f * 0.46f * pb, 0.5f * 0.12f * pb, 0.5f * 0.38f * pb);
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
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.82f * pb, -0.18f * pb);
    glScalef(0.05f * pb, 0.14f * pb, 0.05f * pb);
    glutSolidCube(1.0f);
    glPopMatrix();

    const float vasoPx = 7.85f, vasoPz = 9.05f;
    const float vasoS = 1.12f * pb;
    glPushMatrix();
    glTranslatef(vasoPx, 0.0f, vasoPz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.21f * vasoS, 0.0f);
    drawLoucaOuMarmore(0.5f * 0.38f * vasoS, 0.5f * 0.24f * vasoS, 0.5f * 0.48f * vasoS);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(vasoPx, 0.0f, vasoPz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.50f * vasoS, -0.27f * vasoS);
    drawLoucaOuMarmore(0.5f * 0.34f * vasoS, 0.5f * 0.42f * vasoS, 0.5f * 0.22f * vasoS);
    glPopMatrix();
}
