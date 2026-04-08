#include "../include/moveis.h"
#include "../include/render/primitives.h"

void drawTexturedCube(GLuint texFrente, GLuint texLados, GLuint texTopo) {
    render::drawTexturedCubeFaces(texFrente, texLados, texTopo);
}
