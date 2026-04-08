#ifndef MOVEIS_H
#define MOVEIS_H

/*
 * moveis.h
 * --------
 * Interface de renderização do mobiliário.
 *
 * A implementação está modularizada por cômodo na pasta src/furniture.
 * Este header reúne as funções públicas usadas pelo render principal.
 */

#include <GL/glut.h>

/* Wrapper utilitário para desenhar um cubo com texturas distintas por grupo de faces. */
void drawTexturedCube(GLuint texFrente, GLuint texLados, GLuint texTopo);

/* Sala de jantar: mesa e cadeiras. */
void drawDiningTable();

/* Quartos: camas, guarda-roupas e mesas de estudo. */
void drawBedroomsFurniture();

/* Cozinha: eletrodomésticos, bancada e pia. */
void drawKitchenFurniture();

/* Banheiro: banheira, sanitário e pia. */
void drawBathroomFurniture();

/* Sala de estar: sofá, rack e TV. */
void drawLivingRoomFurniture();

#endif