#ifndef TEXTURAS_UTIL_H
#define TEXTURAS_UTIL_H

#include <GL/glut.h>

// Variáveis globais partilhadas
extern GLuint texPiso;
extern GLuint texParede;
extern GLuint texGramado;
extern GLuint texTelhado;
extern GLuint texGesso; 
extern GLuint texPorta; 

extern GLuint texGeladeira;
extern GLuint texMarmore;
extern GLuint texMarmore2;
extern GLuint texFogaoFrente;
extern GLuint texFogaoCima;
extern GLuint texGuardaRoupa;
extern GLuint texMesaTampo;
extern GLuint texMesaPe;
extern GLuint texCadeiras;
extern GLuint texSofa;
extern GLuint texMesaTv;
extern GLuint texConteudoTv;
extern GLuint texCabeceiraCama;
extern GLuint texBaseCama;
extern GLuint texColchaoCama;

extern GLuint texCeu; 

// Função de carregamento
GLuint loadTexture(const char* filename);

#endif