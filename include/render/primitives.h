#ifndef RENDER_PRIMITIVES_H
#define RENDER_PRIMITIVES_H

#include <GL/glut.h>

namespace render {

void drawTexturedCubeFaces(GLuint texFront, GLuint texSides, GLuint texTop);

void drawTexturedBox6(GLuint textureId, float halfX, float halfY, float halfZ, float uvScale,
                      float ambientStrength = 0.24f, float diffuseStrength = 0.86f);

} // namespace render

#endif
