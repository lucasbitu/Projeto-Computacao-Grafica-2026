# Projeto Final CG: Ambiente Simulado (Casa)

## 📌 O que o código faz
Este projeto é uma simulação 3D em OpenGL de um ambiente virtual (uma casa). O usuário pode navegar interativamente pelo ambiente utilizando as teclas W, A, S e D, observando a aplicação de transformações geométricas em móveis (mesa), iluminação global e texturas foto-realistas aplicadas ao chão e às paredes. Há também uma função de "Tour Virtual" onde uma câmera automatizada percorre o cenário.

## 🚀 Como compilar e executar
**Dependências:** O projeto requer as bibliotecas base do OpenGL e GLUT. 
No Linux (Ubuntu/Debian), instale com: `sudo apt-get install freeglut3-dev`

Com `Makefile` (recomendado), abra o terminal na pasta do projeto e execute:
`make`

Para compilar manualmente (alternativa):
g++ src/main.cpp src/texturas_util.cpp src/camera.cpp src/moveis.cpp src/cenario.cpp src/render/primitives.cpp src/scene/house.cpp src/scene/internal_walls.cpp src/scene/windows.cpp src/scene/doors.cpp src/furniture/dining.cpp src/furniture/living_room.cpp src/furniture/kitchen.cpp src/furniture/bathroom.cpp src/furniture/bedroom.cpp -I include -lGL -lGLU -lglut -o simulador

Para executar o programa:
`./simulador`

Ou com `make`:
`make run`

## 🧩 Elementos das Atividades Práticas
O projeto engloba os seguintes requisitos exigidos:
* **Aula Prática 02 (Visualização 3D):** Uso de `gluLookAt` para a câmera e transformações hierárquicas (`glPushMatrix` / `glPopMatrix`) para desenhar as pernas e o tampo da mesa a partir do centro do mundo.
* **Aula Prática 03 (Transformações e Visibilidade):** Ativação do Z-Buffer (`GL_DEPTH_TEST`) para oclusão correta dos objetos na sala.
* **Aula Prática 04 (Iluminação e Sombreamento):** Implementação de uma luz pontual no teto (`GL_LIGHT0`) e configuração de materiais (`glMaterialfv`) com sombreamento `GL_SMOOTH`.
* **Aula Prática 05 (Mapeamento de Texturas):** Uso da biblioteca `stb_image.h` para mapear texturas reais de tijolo e madeira nos polígonos `GL_QUADS` das paredes e piso usando `GL_REPEAT`.


## 💡 O que pode ser melhorado (Trabalhos Futuros)
*(Ideias: Adicionar colisão para o usuário não atravessar as paredes, colocar mais móveis detalhados, adicionar um teto, etc.)*
