#include "../include/camera.h"
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h> 

/*
 * camera.cpp
 * ----------
 * Implementa navegação manual e tour virtual automatizado.
 *
 * Fluxo principal:
 * - updateTour() calcula posição/orientação em curvas de Bézier,
 * - keyboard()/specialKeys() tratam entrada quando o tour não está ativo,
 * - estado da câmera é consumido por src/main.cpp (gluLookAt).
 */

float camX = -6.4f;    
float camY = 2.0f;     
float camZ = 18.0f;    
float camYaw = 0.0f;   
float camPitch = 0.0f; 
const float CAM_PITCH_LIMIT = 1.55f;

extern bool portaFrenteAberta; 
extern bool portaQ1Aberta;
extern bool portaQ2Aberta;
extern bool portaSuiteAberta;
extern const float DOOR_W;
extern float calcularDistancia(float x1, float z1, float x2, float z2);

// ==========================================
// LÓGICA DO TOUR VIRTUAL (Splines de Bézier)
// ==========================================
bool tourAtivo = false;
float tourTempo = 0.0f;

struct Ponto3D { float x, y, z; };

// CURVA 1: Aproximação e Entrada (0% a 12.5%)
Ponto3D c1_p0 = {  5.0f,  6.0f,  25.0f }; 
Ponto3D c1_p1 = { -6.4f,  4.0f,  20.0f }; 
Ponto3D c1_p2 = { -6.4f,  2.0f,  13.0f }; 
Ponto3D c1_p3 = { -6.4f,  2.0f,   8.0f }; 

// CURVA 2: Corredor da Entrada (12.5% a 25%)
Ponto3D c2_p0 = { -6.4f,  2.0f,   8.0f }; 
Ponto3D c2_p1 = { -7.0f,  3.0f,  -1.0f }; 
Ponto3D c2_p2 = {  0.5f,  3.0f,   8.0f }; 
Ponto3D c2_p3 = {  0.5f,  2.0f,   2.0f }; 

// CURVA 3: O Looping no Quarto 1 (25% a 37.5%)
Ponto3D c3_p0 = {  0.5f,  2.0f,   2.0f }; 
Ponto3D c3_p1 = { -2.0f,  2.0f, -12.0f }; 
Ponto3D c3_p2 = {  3.0f,  2.0f, -12.0f }; 
Ponto3D c3_p3 = {  0.5f,  2.0f,   2.0f }; 

// CURVA 4: Ida até a porta do Banheiro (37.5% a 50%)
Ponto3D c4_p0 = {  0.5f,  2.0f,   2.0f }; 
Ponto3D c4_p1 = {  3.0f,  2.0f,   2.0f }; 
Ponto3D c4_p2 = {  6.0f,  2.0f,   2.0f }; 
Ponto3D c4_p3 = {  6.0f,  2.0f,   3.5f }; 

// CURVA 5: Tour no Banheiro e deslocamento até a porta do Quarto 2 (50% a 62.5%)
Ponto3D c5_p0 = {  6.0f,  2.0f,   3.5f }; 
Ponto3D c5_p1 = {  9.0f,  2.0f,  10.0f }; 
Ponto3D c5_p2 = {  3.5f,  2.0f,  10.0f }; 
Ponto3D c5_p3 = {  7.0f,  2.0f,   1.5f }; 

// CURVA 6: Tour no Quarto 2 (62.5% a 75%)
Ponto3D c6_p0 = {  7.0f,  2.0f,   1.5f }; 
Ponto3D c6_p1 = {  7.0f,  2.0f,  -8.0f }; 
Ponto3D c6_p2 = {  9.5f,  2.0f,  -8.0f }; 
Ponto3D c6_p3 = {  7.0f,  2.0f,   1.5f }; 

// CURVA 7: Condução segura pelos corredores até à porta da frente (75% a 87.5%)
Ponto3D c7_p0 = {  7.0f,  2.0f,   1.5f }; // Nasce onde a Curva 6 parou
Ponto3D c7_p1 = {  1.0f,  2.0f,   1.5f }; // Desliza pelo corredor
Ponto3D c7_p2 = { -6.4f,  2.0f,   4.0f }; // Vira em direção ao corredor da porta
Ponto3D c7_p3 = { -6.4f,  2.0f,   8.0f }; // Para exatamente por dentro da porta (Mesmo ponto do c1_p3)

// CURVA 8: Saída da Casa e Retorno à Origem (87.5% a 100%)
Ponto3D c8_p0 = { -6.4f,  2.0f,   8.0f }; // Nasce na porta
Ponto3D c8_p1 = { -6.4f,  2.0f,  15.0f }; // Atravessa a porta em linha reta para evitar bater nas paredes
Ponto3D c8_p2 = { -2.0f,  4.0f,  20.0f }; // Inicia a subida suave
Ponto3D c8_p3 = {  5.0f,  6.0f,  25.0f }; // VOLTA EXATAMENTE AO PONTO INICIAL (c1_p0)

Ponto3D calcularBezierCubo(float t, Ponto3D p0, Ponto3D p1, Ponto3D p2, Ponto3D p3) {
    /* Avaliação clássica de curva de Bézier cúbica por combinação Bernstein. */
    float u = 1.0f - t;
    float tt = t * t, uu = u * u;
    float uuu = uu * u, ttt = tt * t;
    Ponto3D p;
    p.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    p.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
    p.z = uuu * p0.z + 3 * uu * t * p1.z + 3 * u * tt * p2.z + ttt * p3.z;
    return p;
}

// Gerenciador da Spline (Emenda de OITO Curvas, 12.5% cada)
Ponto3D getPosicaoTour(float t_global) {
    /* Segmenta [0,1] em 8 partes iguais e seleciona curva correspondente. */
    if (t_global <= 0.125f) return calcularBezierCubo(t_global * 8.0f, c1_p0, c1_p1, c1_p2, c1_p3);
    else if (t_global <= 0.250f) return calcularBezierCubo((t_global - 0.125f) * 8.0f, c2_p0, c2_p1, c2_p2, c2_p3);
    else if (t_global <= 0.375f) return calcularBezierCubo((t_global - 0.250f) * 8.0f, c3_p0, c3_p1, c3_p2, c3_p3);
    else if (t_global <= 0.500f) return calcularBezierCubo((t_global - 0.375f) * 8.0f, c4_p0, c4_p1, c4_p2, c4_p3);
    else if (t_global <= 0.625f) return calcularBezierCubo((t_global - 0.500f) * 8.0f, c5_p0, c5_p1, c5_p2, c5_p3);
    else if (t_global <= 0.750f) return calcularBezierCubo((t_global - 0.625f) * 8.0f, c6_p0, c6_p1, c6_p2, c6_p3);
    else if (t_global <= 0.875f) return calcularBezierCubo((t_global - 0.750f) * 8.0f, c7_p0, c7_p1, c7_p2, c7_p3);
    else return calcularBezierCubo((t_global - 0.875f) * 8.0f, c8_p0, c8_p1, c8_p2, c8_p3);
}

void updateTour() {
    if (!tourAtivo) return;

    tourTempo += 0.00065f; // Ajuste perfeito para 8 curvas
    if (tourTempo > 1.0f) { 
        tourTempo = 0.0f; // Permite o ciclo contínuo
    }

    Ponto3D pos = getPosicaoTour(tourTempo);
    camX = pos.x; camY = pos.y; camZ = pos.z;

    float t_frente = (tourTempo + 0.01f > 1.0f) ? 1.0f : tourTempo + 0.01f;
    Ponto3D frente = getPosicaoTour(t_frente);
    
    Ponto3D guardaRoupa = { -2.7f, 2.0f, -5.0f };
    Ponto3D alvoAtual;
    float pesoFoco = 0.0f;

    // Foco no Guarda-Roupa (Curva 3 recalculada para o intervalo 25% a 37.5%)
    if (tourTempo >= 0.25f && tourTempo <= 0.35f) {
        if (tourTempo < 0.27f) pesoFoco = (tourTempo - 0.25f) / 0.02f;
        else if (tourTempo > 0.33f) pesoFoco = 1.0f - ((tourTempo - 0.33f) / 0.02f);
        else pesoFoco = 1.0f;
    }

    alvoAtual.x = frente.x * (1.0f - pesoFoco) + guardaRoupa.x * pesoFoco;
    alvoAtual.y = frente.y * (1.0f - pesoFoco) + guardaRoupa.y * pesoFoco;
    alvoAtual.z = frente.z * (1.0f - pesoFoco) + guardaRoupa.z * pesoFoco;

    float dx = alvoAtual.x - camX, dy = alvoAtual.y - camY, dz = alvoAtual.z - camZ;
    camYaw = atan2f(dx, -dz);
    camPitch = atan2f(dy, sqrtf(dx*dx + dz*dz));

    /* Abertura automática de portas por proximidade da câmera durante o tour. */
    if (calcularDistancia(camX, camZ, (-6.4f - 0.5f * DOOR_W), 10.0f) < 4.0f) portaFrenteAberta = true;
    if (calcularDistancia(camX, camZ, 0.6f, 0.0f) < 4.0f) portaQ1Aberta = true;
    if (calcularDistancia(camX, camZ, 5.2f, 4.0f) < 4.0f) portaSuiteAberta = true;
    if (calcularDistancia(camX, camZ, 7.0f, 0.0f) < 4.0f) portaQ2Aberta = true;
}

void keyboard(unsigned char key, int x, int y) {
    /* Ignora comandos manuais durante tour, exceto tecla de alternância do tour. */
    if (tourAtivo && key != 't' && key != 'T') return;
    float velocidade = 0.5f; 
    float fX = sinf(camYaw), fZ = -cosf(camYaw);
    float rX = cosf(camYaw), rZ = sinf(camYaw);
    
    switch (key) {
        case 'w': case 'W': camX += fX * velocidade; camZ += fZ * velocidade; break;
        case 's': case 'S': camX -= fX * velocidade; camZ -= fZ * velocidade; break;
        case 'a': case 'A': camX -= rX * velocidade; camZ -= rZ * velocidade; break;
        case 'd': case 'D': camX += rX * velocidade; camZ += rZ * velocidade; break;
        case 'e': case 'E': camY += velocidade; break;
        case 'q': case 'Q': camY -= velocidade; if (camY < 0.5f) camY = 0.5f; break;
        case 't': case 'T': 
            tourAtivo = !tourAtivo; // O 'T' serve como Play/Pause
            if(tourAtivo && tourTempo == 0.0f) {
                portaFrenteAberta = portaQ1Aberta = portaQ2Aberta = portaSuiteAberta = false;
            }
            break;
        case 'o': case 'O':
            if (calcularDistancia(camX, camZ, (-6.4f - 0.5f * DOOR_W), 10.0f) < 3.0f) portaFrenteAberta = !portaFrenteAberta;
            else if (calcularDistancia(camX, camZ, -0.2f, 0.0f) < 3.0f) portaQ1Aberta = !portaQ1Aberta;
            else if (calcularDistancia(camX, camZ, 6.2f, 0.0f) < 3.0f) portaQ2Aberta = !portaQ2Aberta;
            else if (calcularDistancia(camX, camZ, 5.2f, 4.0f) < 3.0f) portaSuiteAberta = !portaSuiteAberta;
            break;
        case 27: exit(0); break;             
    }
    glutPostRedisplay(); 
}

void specialKeys(int key, int x, int y) {
    /* Rotação fina da câmera pelas setas (somente fora do tour automático). */
    if (tourAtivo) return; 
    float velO = 0.04f; 
    switch (key) {
        case GLUT_KEY_LEFT: camYaw -= velO; break;
        case GLUT_KEY_RIGHT: camYaw += velO; break;
        case GLUT_KEY_UP: camPitch = fminf(camPitch + velO, CAM_PITCH_LIMIT); break;
        case GLUT_KEY_DOWN: camPitch = fmaxf(camPitch - velO, -CAM_PITCH_LIMIT); break;
    }
    glutPostRedisplay();
}