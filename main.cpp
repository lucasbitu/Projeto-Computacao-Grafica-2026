#include <GL/glut.h> // A biblioteca principal que o Prof. Davi usa para gerenciar janelas e inputs.
#include <stdlib.h>  // Necessária para a função exit() quando quisermos fechar o programa.
#include <math.h>    // sin, cos, fmaxf, fminf para direção do olhar
#include <iostream> // Necessário para imprimir mensagens de erro

// Avisa o compilador para carregar o código completo da stb_image 
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

// Variáveis para guardar os IDs das texturas
GLuint texPiso;
GLuint texParede;
GLuint texGramado;
GLuint texTelhado; 

// Posição inicial da nossa câmera no mundo 3D
float camX = 0.0f; // Posição no eixo X (esquerda/direita)
float camY = 2.0f; // Altura da câmera (olhos a 2 metros do chão)
float camZ = 5.0f; // Posição no eixo Z (frente/trás)

// Direção do olhar (yaw em torno do eixo Y, pitch para cima/baixo), em radianos.
// Com yaw=0 e pitch=0 o vetor frontal aponta para -Z, como no gluLookAt original.
float camYaw = 0.0f;
float camPitch = 0.0f;

static const float CAM_PITCH_LIMIT = 1.55f; // ~89° — evita virar de cabeça para baixo

// Função para carregar a textura 
GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    // Configura o comportamento da textura para repetir (como azulejo)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Configura os filtros de suavização 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Garante que a imagem não fique de cabeça para baixo no OpenGL
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    // Carrega os dados da imagem 
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) { 
        if (nrChannels == 3) { 
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); 
        } else if (nrChannels == 4) { 
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data); 
        }
        stbi_image_free(data);
    } else { 
        std::cerr << "Falha ao carregar textura: " << filename << std::endl; 
    }
    
    return textureID;
}

void init(void) {
    // Define a cor de fundo da janela (R, G, B, Alpha). Vamos usar um azul claro para simular o céu.
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); 
    
    // Habilita o teste de profundidade (Z-buffer). 
    glEnable(GL_DEPTH_TEST); 

    // Define que o sombreamento será suave (interpola as cores entre os vértices) 
    glShadeModel(GL_SMOOTH); 

    // --- CONFIGURAÇÃO DA LUZ 0 (Lâmpada da sala) ---
    // Apenas as cores ficam no init() agora
    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // ==========================================
    // CONFIGURAÇÃO DA LUZ 1: O Sol
    // ==========================================
    // Apenas as cores ficam no init() agora
    GLfloat sol_difusa[] = { 0.7f, 0.7f, 0.7f, 1.0f }; 
    GLfloat sol_ambiente[] = { 0.6f, 0.6f, 0.6f, 1.0f };

    glLightfv(GL_LIGHT1, GL_DIFFUSE, sol_difusa);
    glLightfv(GL_LIGHT1, GL_AMBIENT, sol_ambiente); 
    glEnable(GL_LIGHT1); 

    // Mandamos o OpenGL calcular a luz na frente E no verso da parede
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    // ==========================================

    // Mapeamento texturas
    glEnable(GL_TEXTURE_2D); 
    
    texPiso = loadTexture("texturas/piso.jpg");     
    texParede = loadTexture("texturas/paredes.jpg");
    texGramado = loadTexture("texturas/grama.jpg");
    texTelhado = loadTexture("texturas/telhado.jpg"); 
}

void reshape(int w, int h) {
    if (h == 0) h = 1; 
    float aspect = (float)w / (float)h; 

    glViewport(0, 0, w, h); 

    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 

    gluPerspective(60.0, aspect, 0.1, 100.0); 

    glMatrixMode(GL_MODELVIEW); 
}

void drawGround() {
    glDisable(GL_LIGHTING); 

    glColor3f(1.0f, 1.0f, 1.0f); 

    glBindTexture(GL_TEXTURE_2D, texGramado);

    const float y = -0.01f;
    const float size = 200.0f;

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f,   0.0f);   glVertex3f(-size, y, -size);
        glTexCoord2f(0.0f,   200.0f); glVertex3f(-size, y,  size);
        glTexCoord2f(200.0f, 200.0f); glVertex3f( size, y,  size);
        glTexCoord2f(200.0f, 0.0f);   glVertex3f( size, y, -size);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_LIGHTING); 
}

void drawHouse() {
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco);

    // ==========================================
    // 1. O CHÃO
    // ==========================================
    glBindTexture(GL_TEXTURE_2D, texPiso); 
    glBegin(GL_QUADS); 
        glNormal3f(0.0f, 1.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f); 
    glEnd();

    // ==========================================
    // 2. AS PAREDES
    // ==========================================
    glBindTexture(GL_TEXTURE_2D, texParede); 

    glBegin(GL_QUADS);
        // Parede do Fundo (Z = -10)
        glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f); 

        // Parede Esquerda (X = -10)
        glNormal3f(1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f,  10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f,  10.0f);

        // Parede Direita (X = 10)
        glNormal3f(-1.0f, 0.0f, 0.0f); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(10.0f, 0.0f,  10.0f);
        glTexCoord2f(4.0f, 2.0f); glVertex3f(10.0f, 4.0f,  10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(10.0f, 4.0f, -10.0f);

        // Parede da Frente com Porta (Z = 10)
        glNormal3f(0.0f, 0.0f, -1.0f); 
        
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(-2.0f,  0.0f, 10.0f);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(-2.0f,  4.0f, 10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, 10.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 2.0f, 0.0f, 10.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(10.0f, 0.0f, 10.0f);
        glTexCoord2f(2.0f, 2.0f); glVertex3f(10.0f, 4.0f, 10.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f( 2.0f, 4.0f, 10.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 3.0f, 10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.0f, 3.0f, 10.0f);
        glTexCoord2f(1.0f, 0.5f); glVertex3f( 2.0f, 4.0f, 10.0f);
        glTexCoord2f(0.0f, 0.5f); glVertex3f(-2.0f, 4.0f, 10.0f);
    glEnd();

    // ==========================================
    // 3. O TETO INTERNO (Forro cinza)
    // ==========================================
    glBindTexture(GL_TEXTURE_2D, 0); 
    GLfloat cor_teto[] = { 0.9f, 0.9f, 0.9f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cor_teto);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f); // Normal aponta para baixo (recebe luz de dentro)
        
        // MODIFICADO: Invertida a ordem para garantir sentido anti-horário
        // olhando de baixo para cima (a "frente" fica para dentro da sala).
        glVertex3f(-10.0f, 4.0f,  10.0f); // Frente Esq
        glVertex3f( 10.0f, 4.0f,  10.0f); // Frente Dir
        glVertex3f( 10.0f, 4.0f, -10.0f); // Fundo Dir
        glVertex3f(-10.0f, 4.0f, -10.0f); // Fundo Esq
    glEnd();

    // ==========================================
    // 4. ESTRUTURA DO TELHADO EXTERNO (NOVO)
    // ==========================================
    glBindTexture(GL_TEXTURE_2D, texTelhado); // Aplica textura de telhas
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_branco); // Usa branco para textura real

    // Definimos o pico do telhado (centro da casa, mais alto)
    float picoY = 8.0f;

    glBegin(GL_TRIANGLES);
        // --- Face Frontal (Olhando para +Z) ---
        // Normal aponta para frente e para cima
        glNormal3f(0.0f, 0.5f, 0.8f); 
        // Coordenadas de textura mapeadas para o triângulo (repete 4x na largura, 2x na altura)
        glTexCoord2f(2.0f, 2.0f); glVertex3f( 0.0f, picoY, 0.0f);   // Pico
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 4.0f, 10.0f); // Base Esq Frontal
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 4.0f, 10.0f); // Base Dir Frontal

        // --- Face Traseira (Olhando para -Z) ---
        glNormal3f(0.0f, 0.5f, -0.8f);
        glTexCoord2f(2.0f, 2.0f); glVertex3f( 0.0f, picoY, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 4.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-10.0f, 4.0f, -10.0f);

        // --- Face Esquerda (Olhando para -X) ---
        glNormal3f(-0.8f, 0.5f, 0.0f);
        glTexCoord2f(2.0f, 2.0f); glVertex3f( 0.0f, picoY, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 4.0f, -10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f(-10.0f, 4.0f,  10.0f);

        // --- Face Direita (Olhando para +X) ---
        glNormal3f(0.8f, 0.5f, 0.0f);
        glTexCoord2f(2.0f, 2.0f); glVertex3f( 0.0f, picoY, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 4.0f,  10.0f);
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 4.0f, -10.0f);
    glEnd();

    // Desativa textura no final
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawTable() {
    glPushMatrix(); 
    glTranslatef(0.0f, 0.5f, -3.0f); 

    glPushMatrix(); 
        GLfloat tampo_difuso[] = { 0.6f, 0.4f, 0.2f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tampo_difuso);
        glScalef(2.0f, 0.1f, 1.0f);  
        glutSolidCube(1.0); 
    glPopMatrix(); 

    GLfloat perna_difusa[] = { 0.4f, 0.2f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, perna_difusa);

    glPushMatrix();
        glTranslatef(-0.9f, -0.45f, 0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.9f, -0.45f, 0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-0.9f, -0.45f, -0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.9f, -0.45f, -0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix(); 
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity(); 

    float cosP = cosf(camPitch);
    float dirX = sinf(camYaw) * cosP;
    float dirY = sinf(camPitch);
    float dirZ = -cosf(camYaw) * cosP;

    // 1. A câmera é posicionada no mundo
    gluLookAt(camX, camY, camZ,                      
              camX + dirX, camY + dirY, camZ + dirZ, 
              0.0, 1.0, 0.0);                        

    // ========================================================
    // 2. CÓDIGO NOVO: As luzes são posicionadas FIXAS no mundo
    // ========================================================
    // A lâmpada fica fixa no teto da sala
    GLfloat light_position[] = { 0.0f, 3.5f, -3.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // O Sol fica fixo alto no céu, de frente para a fachada da casa (Z=30)
    GLfloat sol_posicao[] = { 0.0f, 15.0f, 30.0f, 0.0f }; 
    glLightfv(GL_LIGHT1, GL_POSITION, sol_posicao);
    // ========================================================

    drawGround();
    drawHouse();
    drawTable();

    glutSwapBuffers(); 
}

void keyboard(unsigned char key, int x, int y) {
    float velocidade = 0.5f; 

    // O cálculo de andar para frente e para os lados continua o mesmo
    float forwardX = sinf(camYaw);
    float forwardZ = -cosf(camYaw);
    float rightX = cosf(camYaw);
    float rightZ = sinf(camYaw);
    
    switch (key) {
        case 'w':
        case 'W':
            camX += forwardX * velocidade;
            camZ += forwardZ * velocidade;
            break;
        case 's':
        case 'S':
            camX -= forwardX * velocidade;
            camZ -= forwardZ * velocidade;
            break;
        case 'a':
        case 'A':
            camX -= rightX * velocidade;
            camZ -= rightZ * velocidade;
            break;
        case 'd':
        case 'D':
            camX += rightX * velocidade;
            camZ += rightZ * velocidade;
            break;
            
        // ==========================================
        // CÓDIGO NOVO: Controle de Altitude (Drone)
        // ==========================================
        case 'e': // Sobe no eixo Y
        case 'E':
            camY += velocidade;
            break;
        case 'q': // Desce no eixo Y
        case 'Q':
            camY -= velocidade;
            // Opcional: Travar a câmera para não afundar abaixo do chão
            if (camY < 0.5f) camY = 0.5f; 
            break;
        // ==========================================

        case 27: // Tecla ESC
            exit(0); 
            break;             
    }
    
    glutPostRedisplay(); 
}

void specialKeys(int key, int x, int y) {
    (void)x;
    (void)y;
    float velocidadeOlhar = 0.04f; 

    switch (key) {
        case GLUT_KEY_LEFT:
            camYaw -= velocidadeOlhar;
            break;
        case GLUT_KEY_RIGHT:
            camYaw += velocidadeOlhar;
            break;
        case GLUT_KEY_UP:
            camPitch = fminf(camPitch + velocidadeOlhar, CAM_PITCH_LIMIT);
            break;
        case GLUT_KEY_DOWN:
            camPitch = fmaxf(camPitch - velocidadeOlhar, -CAM_PITCH_LIMIT);
            break;
        default:
            return;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); 
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(800, 600); 
    glutCreateWindow("Projeto CG - Ambiente Simulado"); 

    init(); 

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop(); 
    return 0;
}