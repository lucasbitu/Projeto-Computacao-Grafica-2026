#include "../include/moveis.h"
#include "../include/render/primitives.h"

/*
 * moveis.cpp
 * ----------
 * Arquivo de compatibilidade/entrada do módulo de móveis.
 *
 * Após a refatoração, as implementações de mobiliário ficam em
 * src/furniture. Este arquivo mantém apenas wrappers utilitários
 * compartilhados para preservar API estável.
 */

void drawTexturedCube(GLuint texFrente, GLuint texLados, GLuint texTopo) {
    /* Delegação direta para a primitiva reutilizável de cubo texturizado. */
    render::drawTexturedCubeFaces(texFrente, texLados, texTopo);
}
