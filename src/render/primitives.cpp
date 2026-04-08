#include "../../include/render/primitives.h"

/*
 * render/primitives.cpp
 * ---------------------
 * Implementação das primitivas geométricas reutilizáveis do projeto.
 *
 * O objetivo é centralizar padrões de desenho (normais, UV, materiais)
 * e evitar duplicação em módulos de cenário e mobiliário.
 */

namespace render {

void drawTexturedCubeFaces(GLuint texFront, GLuint texSides, GLuint texTop) {
    /* Frente com textura dedicada (útil para eletrodomésticos e móveis). */
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

    /* Laterais/traseira/base compartilham a mesma textura. */
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

    /* Topo separado para permitir aparência distinta da tampa/superfície. */
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
    /*
     * Gera UV proporcional ao perímetro horizontal e altura,
     * mantendo repetição visual coerente em caixas de tamanhos diferentes.
     */
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
