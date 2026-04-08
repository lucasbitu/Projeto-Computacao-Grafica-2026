# Projeto Final CG — Ambiente Simulado (Casa 3D)

## 1) O que o código faz
Este projeto implementa um ambiente 3D de uma casa usando OpenGL (pipeline fixo + GLUT).

Funcionalidades principais:
- Navegação manual com câmera livre no ambiente.
- Tour virtual automático com trajetória por curvas de Bézier.
- Renderização de cenário, paredes internas, portas, janelas e móveis por cômodo.
- Iluminação global + iluminação por cômodos.
- Mapeamento de texturas para superfícies e objetos.

## 2) Tecnologias e dependências
- Linguagem: C++17
- API gráfica: OpenGL + GLU + GLUT
- Carregamento de imagens: `stb_image.h`

No Linux (Ubuntu/Debian), instale:

```bash
sudo apt-get install freeglut3-dev
```

## 3) Como compilar e executar
### Opção recomendada (Makefile)

```bash
make
./simulador
```

ou

```bash
make run
```

### Opção manual

```bash
g++ src/main.cpp src/texturas_util.cpp src/camera.cpp src/moveis.cpp src/cenario.cpp src/render/primitives.cpp src/scene/house.cpp src/scene/internal_walls.cpp src/scene/windows.cpp src/scene/doors.cpp src/furniture/dining.cpp src/furniture/living_room.cpp src/furniture/kitchen.cpp src/furniture/bathroom.cpp src/furniture/bedroom.cpp -I include -lGL -lGLU -lglut -o simulador
./simulador
```

## 4) Organização do código
Resumo da estrutura:

- `src/main.cpp`: inicialização OpenGL, carregamento de texturas, loop e render principal.
- `src/camera.cpp`: controle manual da câmera e tour virtual.
- `src/cenario.cpp` + `src/scene/*`: cenário, casa, paredes internas, portas e janelas.
- `src/moveis.cpp` + `src/furniture/*`: móveis por ambiente.
- `src/render/primitives.cpp`: primitivas reutilizáveis de renderização.
- `src/texturas_util.cpp`: variáveis e carregamento de texturas.

Os detalhes técnicos de cada módulo estão descritos diretamente nos comentários
dos arquivos de código (`.h` e `.cpp`).

## 5) Seções para preenchimento da equipe

### Imagem do programa
![Visão 1](<img/Screenshot from 2026-04-08 02-03-26.png>)
![Visão 2](<img/Screenshot from 2026-04-08 02-03-51.png>)
![Visão 3](<img/Screenshot from 2026-04-08 02-04-05.png>)
![Visão 4](<img/Screenshot from 2026-04-08 02-04-29.png>)
![Visão 5](<img/Screenshot from 2026-04-08 02-04-51.png>)
![Visão 6](<img/Screenshot from 2026-04-08 02-05-01.png>)

### Principais problemas encontrados
[ESPAÇO PARA PREENCHER]

### O que pode ser melhorado (e como melhorar)
[ESPAÇO PARA PREENCHER]

### Os elementos de cada atividade prática
[ESPAÇO PARA PREENCHER]

### O que cada integrante fez
[ESPAÇO PARA PREENCHER]
