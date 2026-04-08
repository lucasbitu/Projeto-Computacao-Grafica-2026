#include "../../include/render/primitives.h"

namespace render {

void drawTexturedCubeFaces(GLuint texFront, GLuint texSides, GLuint texTop) {
    if (texFront > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texFront);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glEnd();

    if (texSides > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texSides);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();

    if (texTop > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texTop);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
}

void drawTexturedBox6(GLuint textureId, float halfX, float halfY, float halfZ, float uvScale,
                      float ambientStrength, float diffuseStrength) {
    const float uvXZ = uvScale * (2.0f * halfX + 2.0f * halfZ);
    const float uvH = uvScale * (2.0f * halfY);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    GLfloat ambient[] = { ambientStrength, ambientStrength, ambientStrength, 1.0f };
    GLfloat diffuse[] = { diffuseStrength, diffuseStrength, diffuseStrength, 1.0f };
    GLfloat zeroSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfX, -halfY, halfZ);
        glTexCoord2f(uvXZ, 0.0f); glVertex3f(halfX, -halfY, halfZ);
        glTexCoord2f(uvXZ, uvH); glVertex3f(halfX, halfY, halfZ);
        glTexCoord2f(0.0f, uvH); glVertex3f(-halfX, halfY, halfZ);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(uvXZ, 0.0f); glVertex3f(halfX, -halfY, -halfZ);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfX, -halfY, -halfZ);
        glTexCoord2f(0.0f, uvH); glVertex3f(-halfX, halfY, -halfZ);
        glTexCoord2f(uvXZ, uvH); glVertex3f(halfX, halfY, -halfZ);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfX, -halfY, -halfZ);
        glTexCoord2f(uvXZ, 0.0f); glVertex3f(-halfX, -halfY, halfZ);
        glTexCoord2f(uvXZ, uvH); glVertex3f(-halfX, halfY, halfZ);
        glTexCoord2f(0.0f, uvH); glVertex3f(-halfX, halfY, -halfZ);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(uvXZ, 0.0f); glVertex3f(halfX, -halfY, halfZ);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfX, -halfY, -halfZ);
        glTexCoord2f(0.0f, uvH); glVertex3f(halfX, halfY, -halfZ);
        glTexCoord2f(uvXZ, uvH); glVertex3f(halfX, halfY, halfZ);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfX, halfY, halfZ);
        glTexCoord2f(uvXZ, 0.0f); glVertex3f(halfX, halfY, halfZ);
        glTexCoord2f(uvXZ, uvXZ); glVertex3f(halfX, halfY, -halfZ);
        glTexCoord2f(0.0f, uvXZ); glVertex3f(-halfX, halfY, -halfZ);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfX, -halfY, -halfZ);
        glTexCoord2f(uvXZ, 0.0f); glVertex3f(halfX, -halfY, -halfZ);
        glTexCoord2f(uvXZ, uvXZ); glVertex3f(halfX, -halfY, halfZ);
        glTexCoord2f(0.0f, uvXZ); glVertex3f(-halfX, -halfY, halfZ);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

} // namespace render
