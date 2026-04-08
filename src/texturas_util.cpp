#include "../include/texturas_util.h"
#include <iostream>

/*
 * texturas_util.cpp
 * -----------------
 * Implementa o gerenciamento básico de texturas:
 * - instancia os IDs globais declarados no header,
 * - carrega imagens com stb_image,
 * - cria texturas OpenGL com mipmaps para melhor qualidade visual.
 */

#define STB_IMAGE_IMPLEMENTATION 
#include "../include/stb_image.h" 

// Instanciação real das variáveis
GLuint texPiso;
GLuint texParede;
GLuint texGramado;
GLuint texTelhado;
GLuint texGesso; 
GLuint texPorta; 

GLuint texGeladeira;
GLuint texMarmore;
GLuint texMarmore2;
GLuint texFogaoFrente;
GLuint texFogaoCima;
GLuint texGuardaRoupa;
GLuint texMesaTampo;
GLuint texMesaPe;
GLuint texCadeiras;
GLuint texSofa;
GLuint texMesaTv;
GLuint texConteudoTv;
GLuint texCabeceiraCama;
GLuint texBaseCama;
GLuint texColchaoCama;

GLuint texCeu; 

GLuint loadTexture(const char* filename) {
    /* Cria objeto de textura e o torna ativo para configuração. */
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    /* Repetição de textura para coordenadas UV fora de [0,1]. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    /* Mipmap trilinear na minificação; linear na magnificação. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Ajusta orientação para convenção UV usada no projeto. */
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) { 
        /* Seleciona formato conforme número de canais da imagem carregada. */
        if (nrChannels == 3) { 
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); 
        } else if (nrChannels == 4) { 
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data); 
        }
        stbi_image_free(data);
    } else { 
        /* Mantém log explícito para depuração de caminhos/arquivos ausentes. */
        std::cerr << "Falha ao carregar textura: " << filename << std::endl; 
    }
    
    return textureID;
}