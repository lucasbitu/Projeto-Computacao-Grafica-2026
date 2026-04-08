#ifndef CENARIO_H
#define CENARIO_H

/*
 * cenario.h
 * ---------
 * Interface pública do cenário (arquitetura da casa, céu, chão e aberturas).
 *
 * Este módulo centraliza estados compartilhados (portas) e funções de desenho
 * chamadas pelo pipeline principal em src/main.cpp.
 */

#include <GL/glut.h>

/* Estados de animação das portas (true = aberta, false = fechada). */
extern bool portaFrenteAberta; 
extern bool portaQ1Aberta;
extern bool portaQ2Aberta;
extern bool portaSuiteAberta;

/* Dimensões padrão de porta utilizadas em recortes e modelos. */
extern const float DOOR_H;
extern const float DOOR_W;

/* Distância no plano XZ (útil para interação com portas e gatilhos de proximidade). */
float calcularDistancia(float x1, float z1, float x2, float z2);

/* Renderiza o céu (skybox esférica). */
void drawSkybox();

/* Renderiza o terreno externo. */
void drawGround();

/* Renderiza paredes internas e elementos fixos internos (ex.: balcão). */
void drawInternalWalls();

/* Renderiza estrutura principal da casa (fachadas, teto e telhado). */
void drawHouse();

/* Atualiza posições das luzes internas por cômodo. */
void updateRoomLightPositions(void);

/* Renderiza luminárias de teto (malha visível das fontes de luz). */
void drawCeilingLampFixtures(void);

/* Renderiza molduras e vidros de janelas. */
void drawWindows();

/* Modelo geométrico de uma porta padrão. */
void drawDoorModel();

/* Renderiza todas as portas posicionadas no cenário, com animação. */
void drawDoors();

#endif