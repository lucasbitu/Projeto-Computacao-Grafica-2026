#include "../../include/moveis.h"
#include "../../include/texturas_util.h"
#include "../../include/render/primitives.h"

#include <GL/glut.h>

void drawLivingRoomFurniture() {
    const float sofaK = 1.55f;

    glPushMatrix();
        glTranslatef(-5.5f, 0.0f, 6.0f);

        glPushMatrix();
            glTranslatef(0.0f, 0.25f, 0.0f);
            render::drawTexturedBox6(texSofa, 0.5f, 0.25f, 1.25f, sofaK);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.38f, 0.87f, 0.0f);
            render::drawTexturedBox6(texSofa, 0.15f, 0.35f, 1.21f, sofaK);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.45f, 1.37f);
            render::drawTexturedBox6(texSofa, 0.5f, 0.25f, 0.1f, sofaK);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.45f, -1.37f);
            render::drawTexturedBox6(texSofa, 0.5f, 0.25f, 0.1f, sofaK);
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-9.46f, 0.25f, 6.0f);
        render::drawTexturedBox6(texMesaTv, 0.3f, 0.25f, 1.1f, 1.75f, 0.08f, 0.38f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    GLfloat mat_tv_borda[] = { 0.08f, 0.08f, 0.09f, 1.0f };
    GLfloat mat_tv_bordaAmb[] = { 0.04f, 0.04f, 0.045f, 1.0f };
    GLfloat mat_tv_zeroSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const float tvPy = 1.03f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_tv_zeroSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_tv_bordaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_tv_borda);
    glPushMatrix();
        glTranslatef(-9.46f, 0.515f, 6.0f);
        glScalef(0.38f, 0.02f, 0.58f);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-9.46f, tvPy, 6.0f);

        const float phx = 0.05f;
        const float phy = 0.45f;
        const float phz = 0.8f;

        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texConteudoTv);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(phx, -phy, -phz);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(phx, -phy,  phz);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(phx,  phy,  phz);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(phx,  phy, -phz);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_tv_zeroSpec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_tv_bordaAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_tv_borda);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glVertex3f(-phx, -phy,  phz);
            glVertex3f(-phx, -phy, -phz);
            glVertex3f(-phx,  phy, -phz);
            glVertex3f(-phx,  phy,  phz);

            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-phx, phy,  phz);
            glVertex3f( phx, phy,  phz);
            glVertex3f( phx, phy, -phz);
            glVertex3f(-phx, phy, -phz);

            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(-phx, -phy, -phz);
            glVertex3f( phx, -phy, -phz);
            glVertex3f( phx, -phy,  phz);
            glVertex3f(-phx, -phy,  phz);

            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-phx, -phy, phz);
            glVertex3f( phx, -phy, phz);
            glVertex3f( phx,  phy, phz);
            glVertex3f(-phx,  phy, phz);

            glNormal3f(0.0f, 0.0f, -1.0f);
            glVertex3f( phx, -phy, -phz);
            glVertex3f(-phx, -phy, -phz);
            glVertex3f(-phx,  phy, -phz);
            glVertex3f( phx,  phy, -phz);
        glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
}
