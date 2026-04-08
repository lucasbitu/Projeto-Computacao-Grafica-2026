#include "../../include/cenario.h"
#include "../../include/texturas_util.h"

/*
 * scene/internal_walls.cpp
 * ------------------------
 * Renderiza a compartimentação interna da casa:
 * - paredes divisórias com vãos de portas,
 * - mureta e tampo de balcão entre ambientes.
 */

void drawInternalWalls() {
    /* Material claro de parede para manter leitura uniforme dos ambientes. */
    glBindTexture(GL_TEXTURE_2D, texParede);
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    const float q1a = -0.2f, q1b = q1a + DOOR_W;
    const float q2a = 6.2f, q2b = q2a + DOOR_W;
    const float bathXa = 5.2f, bathXb = bathXa + DOOR_W;
    const float muretaTopoY = 1.32f;
    const float balcaoY = muretaTopoY;
    const float muretaTexV = muretaTopoY * (2.0f / 1.7f);
    const float bzCoz = -0.55f;
    const float bzSala = 0.42f;

    /* Quads montados em segmentos para preservar recortes e UV coerente. */
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f);
        glTexCoord2f(2.15f, 0.0f); glVertex3f(q1a, 0.0f, 0.0f);
        glTexCoord2f(2.15f, 2.0f); glVertex3f(q1a, 4.0f, 0.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f, 0.0f);

        glTexCoord2f(2.15f, 1.1f); glVertex3f(q1a, DOOR_H, 0.0f);
        glTexCoord2f(3.38f, 1.1f); glVertex3f(q1b, DOOR_H, 0.0f);
        glTexCoord2f(3.38f, 2.0f); glVertex3f(q1b, 4.0f, 0.0f);
        glTexCoord2f(2.15f, 2.0f); glVertex3f(q1a, 4.0f, 0.0f);

        glTexCoord2f(3.38f, 0.0f); glVertex3f(q1b, 0.0f, 0.0f);
        glTexCoord2f(7.08f, 0.0f); glVertex3f(q2a, 0.0f, 0.0f);
        glTexCoord2f(7.08f, 2.0f); glVertex3f(q2a, 4.0f, 0.0f);
        glTexCoord2f(3.38f, 2.0f); glVertex3f(q1b, 4.0f, 0.0f);

        glTexCoord2f(7.08f, 1.1f); glVertex3f(q2a, DOOR_H, 0.0f);
        glTexCoord2f(8.31f, 1.1f); glVertex3f(q2b, DOOR_H, 0.0f);
        glTexCoord2f(8.31f, 2.0f); glVertex3f(q2b, 4.0f, 0.0f);
        glTexCoord2f(7.08f, 2.0f); glVertex3f(q2a, 4.0f, 0.0f);

        glTexCoord2f(8.31f, 0.0f); glVertex3f(q2b, 0.0f, 0.0f);
        glTexCoord2f(10.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
        glTexCoord2f(10.0f, 2.0f); glVertex3f(10.0f, 4.0f, 0.0f);
        glTexCoord2f(8.31f, 2.0f); glVertex3f(q2b, 4.0f, 0.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f,  10.0f);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(3.0f, 0.0f,   4.0f);
        glTexCoord2f(5.0f, 2.0f); glVertex3f(3.0f, 4.0f,   4.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(3.0f, 4.0f,  10.0f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f,   0.0f);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -10.0f);
        glTexCoord2f(5.0f, 2.0f); glVertex3f(-3.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f,   0.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(4.0f, 0.0f,   0.0f);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(4.0f, 0.0f, -10.0f);
        glTexCoord2f(5.0f, 2.0f); glVertex3f(4.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(4.0f, 4.0f,   0.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 3.0f, 0.0f, 4.0f);
        glTexCoord2f(1.1f, 0.0f); glVertex3f(bathXa, 0.0f, 4.0f);
        glTexCoord2f(1.1f, 2.0f); glVertex3f(bathXa, 4.0f, 4.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f( 3.0f, 4.0f, 4.0f);

        glTexCoord2f(1.1f, 1.1f); glVertex3f(bathXa, DOOR_H, 4.0f);
        glTexCoord2f(1.9f, 1.1f); glVertex3f(bathXb, DOOR_H, 4.0f);
        glTexCoord2f(1.9f, 2.0f); glVertex3f(bathXb, 4.0f, 4.0f);
        glTexCoord2f(1.1f, 2.0f); glVertex3f(bathXa, 4.0f, 4.0f);

        glTexCoord2f(1.9f, 0.0f); glVertex3f(bathXb, 0.0f, 4.0f);
        glTexCoord2f(3.5f, 0.0f); glVertex3f(10.0f, 0.0f, 4.0f);
        glTexCoord2f(3.5f, 2.0f); glVertex3f(10.0f, 4.0f, 4.0f);
        glTexCoord2f(1.9f, 2.0f); glVertex3f(bathXb, 4.0f, 4.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 0.0f);
        glTexCoord2f(10.0f, 0.0f); glVertex3f( -6.0f, 0.0f, 0.0f);
        glTexCoord2f(10.0f, muretaTexV); glVertex3f( -6.0f, muretaTopoY, 0.0f);
        glTexCoord2f(0.0f, muretaTexV); glVertex3f(-3.0f, muretaTopoY, 0.0f);

        glBindTexture(GL_TEXTURE_2D, texPiso);
        GLfloat mat_balcao[] = { 0.88f, 0.86f, 0.82f, 1.0f };
        GLfloat amb_balcao[] = { 0.36f, 0.35f, 0.33f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb_balcao);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_balcao);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, balcaoY, bzCoz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, balcaoY, bzSala);
        glTexCoord2f(3.0f, 1.0f); glVertex3f(-3.0f, balcaoY, bzSala);
        glTexCoord2f(3.0f, 0.0f); glVertex3f(-3.0f, balcaoY, bzCoz);
        glBindTexture(GL_TEXTURE_2D, texParede);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}
