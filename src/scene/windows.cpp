#include "../../include/cenario.h"
#include <GL/glut.h>

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
