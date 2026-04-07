#include "../include/texturas_util.h"
#include <iostream>

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
GLuint texFogaoFrente;
GLuint texFogaoCima;
GLuint texGuardaRoupa;

GLuint texCeu; 

GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
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