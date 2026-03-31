#include <GL/glut.h> // A biblioteca principal que o Prof. Davi usa para gerenciar janelas e inputs.
#include <stdlib.h>  // Necessária para a função exit() quando quisermos fechar o programa.
#include <math.h>    // sin, cos, fmaxf, fminf para direção do olhar
#include <iostream> // Necessário para imprimir mensagens de erro

// Avisa o compilador para carregar o código completo da stb_image [cite: 215]
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

// Variáveis para guardar os IDs das texturas
GLuint texPiso;
GLuint texParede;

// Posição inicial da nossa câmera no mundo 3D
float camX = 0.0f; // Posição no eixo X (esquerda/direita)
float camY = 2.0f; // Altura da câmera (olhos a 2 metros do chão)
float camZ = 5.0f; // Posição no eixo Z (frente/trás)

// Direção do olhar (yaw em torno do eixo Y, pitch para cima/baixo), em radianos.
// Com yaw=0 e pitch=0 o vetor frontal aponta para -Z, como no gluLookAt original.
float camYaw = 0.0f;
float camPitch = 0.0f;

static const float CAM_PITCH_LIMIT = 1.55f; // ~89° — evita virar de cabeça para baixo

// Função para carregar a textura [cite: 226]
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
    // Isso garante que uma parede na frente esconda o que está atrás dela[cite: 67].
    glEnable(GL_DEPTH_TEST); 

    // Define que o sombreamento será suave (interpola as cores entre os vértices) [cite: 167]
    glShadeModel(GL_SMOOTH); 

    // --- CONFIGURAÇÃO DA LUZ 0 (Lâmpada da sala) ---
    // A posição da luz: (X=0, Y=3.5, Z=-3, W=1). 
    // O W=1 indica que é uma luz pontual (tem posição no espaço)[cite: 91, 94].
    GLfloat light_position[] = { 0.0f, 3.5f, -3.0f, 1.0f }; 
    
    // Cor ambiente (luz residual, fraquinha) 
    GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    // Cor difusa (a cor real da luz batendo nos objetos, branca) 
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    // Cor especular (o brilho refletido, branco) [cite: 89]
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 

    // Entrega essas configurações para a LIGHT0 do OpenGL 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Liga o "interruptor" geral de luz e liga a lâmpada 0 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Mapeamento texturas
    glEnable(GL_TEXTURE_2D); // Ativa o mapeamento de textura 
    
    // Carrega os arquivos que você colocou na pasta. 
    // Coloquei o nome exato do arquivo de tijolos que você fez upload.
    texPiso = loadTexture("texturas/piso.jpg");     
    texParede = loadTexture("texturas/paredes.jpg");
}

void reshape(int w, int h) {
    if (h == 0) h = 1; // Previne divisão por zero se a janela for minimizada.
    float aspect = (float)w / (float)h; // Calcula a proporção da tela.

    glViewport(0, 0, w, h); // Diz ao OpenGL para usar toda a área da janela.

    // Entrando no modo de PROJEÇÃO (configurando a lente da câmera)
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); // Reseta a matriz de projeção.

    // Define a perspectiva: (ângulo de visão, proporção, distância mínima visível, distância máxima visível)[cite: 12].
    gluPerspective(60.0, aspect, 0.1, 100.0); 

    // Volta para o modo MODELVIEW (onde colocamos os objetos no mundo 3D)
    glMatrixMode(GL_MODELVIEW); 
}

void drawGround() {
    // Piso padrão do "mundo" fora da casa.
    // Fica levemente abaixo do piso da casa para evitar z-fighting.
    glBindTexture(GL_TEXTURE_2D, 0);

    GLfloat material_chao[] = { 0.25f, 0.55f, 0.25f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_chao);

    const float y = -0.01f;
    const float size = 200.0f;

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-size, y, -size);
        glVertex3f(-size, y,  size);
        glVertex3f( size, y,  size);
        glVertex3f( size, y, -size);
    glEnd();
}

void drawHouse() {
    // Definimos como a superfície reage à luz (Material)
    // Usamos branco puro para não distorcer as cores reais das fotos
    GLfloat material_branco[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_branco);

    // --- O CHÃO ---
    glBindTexture(GL_TEXTURE_2D, texPiso); // Aplica a textura do piso [cite: 198]
    
    glBegin(GL_QUADS); 
        glNormal3f(0.0f, 1.0f, 0.0f); 
        // O valor 10.0 faz a textura repetir 10 vezes, para os tacos não ficarem gigantescos
        glTexCoord2f(0.0f, 0.0f);   glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(0.0f, 10.0f);  glVertex3f(-10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 10.0f); glVertex3f( 10.0f, 0.0f,  10.0f); 
        glTexCoord2f(10.0f, 0.0f);  glVertex3f( 10.0f, 0.0f, -10.0f); 
    glEnd();

    // --- A PAREDE DO FUNDO ---
    glBindTexture(GL_TEXTURE_2D, texParede); // Troca para a textura de tijolos [cite: 198]

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f); 
        // A parede é mais larga, então repetimos 4x na largura e 2x na altura
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 0.0f); glVertex3f( 10.0f, 0.0f, -10.0f); 
        glTexCoord2f(4.0f, 2.0f); glVertex3f( 10.0f, 4.0f, -10.0f); 
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-10.0f, 4.0f, -10.0f); 
    glEnd();

    // Desativa a textura no final para que a mesa e outros objetos 
    // voltem a usar apenas cores sólidas (se não a mesa vai ficar de tijolo também!)
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawTable() {
    // Salva o sistema de coordenadas atual do mundo (para não afetar o resto da casa).
    glPushMatrix(); 

    // Move a mesa inteira para o centro da sala (um pouco à frente da câmera original)
    // Subimos o Y em 0.5 para o centro da mesa não ficar enterrado no chão.
    glTranslatef(0.0f, 0.5f, -3.0f); 

    // --- TAMPO DA MESA ---
    glPushMatrix(); // Salva a posição central da mesa [cite: 16, 17]
        // Define o material do tampo (Marrom claro)
        GLfloat tampo_difuso[] = { 0.6f, 0.4f, 0.2f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tampo_difuso);
        // Escala o cubo base (1.0) para virar um retângulo largo (2.0) e achatado (0.1)
        glScalef(2.0f, 0.1f, 1.0f);  
        glutSolidCube(1.0); // Define um cubo sólido 
    glPopMatrix(); // Volta para o centro da mesa [cite: 16, 17]

    // --- MATERIAL DAS PERNAS ---
    // Como todas as pernas têm a mesma cor, 
    // definimos o material (Marrom escuro) uma vez aqui fora, 
    // e ele valerá para os 4 cubos seguintes.
    GLfloat perna_difusa[] = { 0.4f, 0.2f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, perna_difusa);

    // --- PERNA 1 (Frente Esquerda) ---
    glPushMatrix();
        // Desloca a perna para o canto do tampo e desce ela para tocar o chão
        glTranslatef(-0.9f, -0.45f, 0.4f); 
        // Deixa a perna fina (0.1) e comprida (0.9)
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    // --- PERNA 2 (Frente Direita) ---
    glPushMatrix();
        glTranslatef(0.9f, -0.45f, 0.4f); // Muda apenas o X para a direita
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    // --- PERNA 3 (Trás Esquerda) ---
    glPushMatrix();
        glTranslatef(-0.9f, -0.45f, -0.4f); // Muda o Z para o fundo
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    // --- PERNA 4 (Trás Direita) ---
    glPushMatrix();
        glTranslatef(0.9f, -0.45f, -0.4f); 
        glScalef(0.1f, 0.9f, 0.1f);
        glutSolidCube(1.0);
    glPopMatrix();

    // Restaura o sistema de coordenadas geral do mundo.
    glPopMatrix(); 
}

void display(void) {
    // Limpa a cor da tela e o buffer de profundidade (para a oclusão funcionar)[cite: 69].
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    glLoadIdentity(); // Reseta as transformações (limpa qualquer rotação/translação do frame anterior).

    // Vetor unitário para onde a câmera está olhando (derivado de yaw/pitch).
    float cosP = cosf(camPitch);
    float dirX = sinf(camYaw) * cosP;
    float dirY = sinf(camPitch);
    float dirZ = -cosf(camYaw) * cosP;

    // Posiciona a câmera no mundo 3D usando as nossas variáveis[cite: 10, 11].
    gluLookAt(camX, camY, camZ,                      // Posição dos seus "olhos"
              camX + dirX, camY + dirY, camZ + dirZ, // Ponto na linha do olhar
              0.0, 1.0, 0.0);                        // Qual direção é "para cima" (eixo Y)

    // Piso padrão do mundo (fora da casa).
    drawGround();

    // Chama a função que constrói as paredes e o chão.
    drawHouse();
    // Chama a função que constrói a mesa.
    drawTable();

    // Troca os buffers de vídeo (mostra o que acabamos de desenhar).
    glutSwapBuffers(); 
}

void keyboard(unsigned char key, int x, int y) {
    float velocidade = 0.5f; // O tamanho do passo que damos a cada clique.

    // Movimento relativo à câmera (baseado no yaw).
    // Ignoramos o pitch para manter o movimento "no chão" (sem subir/descer).
    float forwardX = sinf(camYaw);
    float forwardZ = -cosf(camYaw);
    float rightX = cosf(camYaw);
    float rightZ = sinf(camYaw);
    
    switch (key) {
        case 'w':
            camX += forwardX * velocidade;
            camZ += forwardZ * velocidade;
            break;
        case 's':
            camX -= forwardX * velocidade;
            camZ -= forwardZ * velocidade;
            break;
        case 'a':
            camX -= rightX * velocidade;
            camZ -= rightZ * velocidade;
            break;
        case 'd':
            camX += rightX * velocidade;
            camZ += rightZ * velocidade;
            break;
        case 27: exit(0); break;             // A tecla 27 é o ESC (para fechar o programa)[cite: 8].
    }
    
    // Avisa o OpenGL que a câmera mudou e ele precisa redesenhar a tela inteira.
    glutPostRedisplay(); 
}

void specialKeys(int key, int x, int y) {
    (void)x;
    (void)y;
    float velocidadeOlhar = 0.04f; // radianos por pressionamento (~2,3°)

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
    glutInit(&argc, argv); // Inicializa a biblioteca GLUT.
    
    // Configura a janela para usar Duplo Buffer, cores RGB e o Buffer de Profundidade[cite: 68].
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(800, 600); // Tamanho inicial da janela.
    glutCreateWindow("Projeto CG - Ambiente Simulado"); // Título da janela.

    init(); // Chama nossa função de configuração inicial.

    // Cadastra os "Callbacks". Dizemos ao GLUT quais funções nossas ele deve chamar em cada evento.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Entra em um loop infinito, esperando você apertar teclas ou redimensionar a tela.
    glutMainLoop(); 
    return 0;
}