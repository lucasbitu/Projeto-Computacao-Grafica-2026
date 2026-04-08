#ifndef TEXTURAS_UTIL_H
#define TEXTURAS_UTIL_H

/*
 * texturas_util.h
 * ---------------
 * Declarações das texturas globais e função de carregamento.
 *
 * Observação: os IDs são globais para facilitar acesso entre módulos de
 * renderização no pipeline fixo OpenGL usado no projeto.
 */

#include <GL/glut.h>

/* Texturas estruturais da casa e cenário externo. */
extern GLuint texPiso;
extern GLuint texParede;
extern GLuint texGramado;
extern GLuint texTelhado;
extern GLuint texGesso; 
extern GLuint texPorta; 

/* Texturas de móveis/objetos internos. */
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

/* Textura de céu para a skybox esférica. */
extern GLuint texCeu; 

/*
 * Carrega imagem de disco e cria textura OpenGL com mipmaps.
 * Retorna o ID da textura gerada.
 */
GLuint loadTexture(const char* filename);

#endif