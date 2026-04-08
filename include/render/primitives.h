#ifndef RENDER_PRIMITIVES_H
#define RENDER_PRIMITIVES_H

/*
 * render/primitives.h
 * -------------------
 * Biblioteca interna de primitivas reutilizáveis para desenho texturizado.
 *
 * Objetivo: reduzir duplicação de código OpenGL imediato nas malhas dos
 * módulos de cenário e mobiliário.
 */

#include <GL/glut.h>

namespace render {

/*
 * Desenha um cubo unitário com:
 * - uma textura para face frontal,
 * - uma textura para faces laterais/traseira/base,
 * - uma textura para a face superior.
 */
void drawTexturedCubeFaces(GLuint texFront, GLuint texSides, GLuint texTop);

/*
 * Desenha um paralelepípedo centrado na origem com textura em 6 faces.
 * halfX/halfY/halfZ: semi-dimensões da caixa.
 * uvScale: fator de repetição de UV.
 * ambientStrength/diffuseStrength: modulação de material no pipeline fixo.
 */
void drawTexturedBox6(GLuint textureId, float halfX, float halfY, float halfZ, float uvScale,
                      float ambientStrength = 0.24f, float diffuseStrength = 0.86f);

} // namespace render

#endif
