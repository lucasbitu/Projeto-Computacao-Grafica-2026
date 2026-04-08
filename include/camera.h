#ifndef CAMERA_H
#define CAMERA_H

/*
 * camera.h
 * --------
 * Contrato público do sistema de câmera.
 *
 * Este módulo expõe:
 * 1) Estado global da câmera (posição e orientação), usado pelo renderer.
 * 2) Controle de tour virtual automático.
 * 3) Callbacks de entrada de teclado (GLUT) para navegação manual.
 */

extern float camX; 
extern float camY; 
extern float camZ; 
extern float camYaw;
extern float camPitch;

/* Indica se a câmera está em modo de tour automático. */
extern bool tourAtivo;

/*
 * Atualiza a câmera ao longo da trajetória do tour.
 * Deve ser chamada continuamente (via idle) enquanto tourAtivo == true.
 */
void updateTour();

/* Callback de teclado alfanumérico (WASD, etc.). */
void keyboard(unsigned char key, int x, int y);

/* Callback de teclas especiais (setas para rotação de câmera). */
void specialKeys(int key, int x, int y);

#endif