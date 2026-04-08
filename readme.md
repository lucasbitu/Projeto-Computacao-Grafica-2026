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

## 5) Seção de resultados

### Imagem do programa
![Visão 1](<img/Screenshot from 2026-04-08 02-03-26.png>)
![Visão 2](<img/Screenshot from 2026-04-08 02-03-51.png>)
![Visão 3](<img/Screenshot from 2026-04-08 02-04-05.png>)
![Visão 4](<img/Screenshot from 2026-04-08 02-04-29.png>)
![Visão 5](<img/Screenshot from 2026-04-08 02-04-51.png>)
![Visão 6](<img/Screenshot from 2026-04-08 02-05-01.png>)

### Principais problemas encontrados
1) **A matemática do sistema de câmera livre**  
Abandonar a movimentação estática do projeto legado exigiu a construção de uma câmera estilo FPS (First-Person Shooter) do zero. O principal desafio foi dominar a matemática vetorial e trigonométrica (`sin` e `cos`) para converter os ângulos de visão (`Yaw` e `Pitch`) em um vetor de direção cartesiano usado pelo `gluLookAt`. Também foi necessário limitar o `Pitch` para evitar que a câmera “capotasse” ao olhar totalmente para cima/baixo, mantendo navegação fluida e estável.

2) **O efeito “lanterna de mineiro” (luz presa à câmera)**  
Enfrentamos uma armadilha clássica do OpenGL de pipeline fixo: a luz direcional parecia seguir a câmera, em vez de permanecer fixa como “Sol” no mundo. A causa era a ordem das operações: a posição da luz estava sendo definida antes da matriz de visualização final do frame. A solução foi atualizar as posições de `GL_LIGHT0` (e demais luzes dependentes) **depois** do `gluLookAt`, dentro do `display()`, para que a iluminação fique ancorada no espaço da cena.

3) **Conflito estético entre gramado e skybox**  
Ao expandir o cenário externo, a textura da grama inicialmente não se integrava bem com a iluminação global e o céu, gerando aspecto artificial de “tapete flutuante”. Ajustamos a repetição da textura com `GL_REPEAT`, melhorando a continuidade visual no chão. Em contrapartida, surgiu outro efeito colateral: em posições mais altas da câmera, a repetição no horizonte fica perceptível e pode “engolir” visualmente a transição com o céu.

4) **Colisões e “cortes de caminho” nas splines de Bézier**  
O objetivo inicial do Tour Virtual era usar uma única curva de Bézier grande para atravessar a casa. Porém, a interpolação fazia a câmera “cortar caminho” e atravessar paredes. A solução arquitetural foi dividir o percurso em **8 curvas de Bézier interligadas** (cada uma representando 12,5% da animação), permitindo curvas de 90° nos corredores e passagens mais precisas pelo centro das portas.

5) **Iluminação externa e zonas de sombra**  
Com apenas o “Sol” (`GL_LIGHT0`) ativo, as faces diretamente iluminadas ficavam boas, mas laterais/fachada oposta perdiam leitura e iam para sombra excessiva. Para equilibrar o conjunto, usamos luz de preenchimento (fill light), melhorando contraste sem destruir o relevo.

Esse problema não foi 100% resolvido: com a porta fechada, a textura recebe iluminação adequada e a leitura visual fica boa; porém, ao abrir para o outro lado, a face principal passa a ficar orientada para uma direção com menor contribuição de luz, e a iluminação fica insuficiente. Em resumo, a solução de iluminação externa melhorou bastante o cenário geral, mas ainda há carência de luz na porta quando ela muda de orientação.

### O que pode ser melhorado (e como melhorar)
Este projeto já atende aos objetivos principais, mas ainda possui limitações técnicas e visuais que podem ser evoluídas.

**1) Artefatos visuais no horizonte (grama x skybox)**  
**Limitação atual:** em alturas maiores de câmera, a repetição da textura do chão (`GL_REPEAT`) fica evidente e a transição com o céu perde naturalidade.  
**Como melhorar:** adicionar névoa (`fog`) para suavizar a distância, ajustar escala/tiling da textura por faixa de distância e usar blend gradual entre terreno e horizonte.

**2) Ausência de colisão física real**  
**Limitação atual:** a câmera pode atravessar paredes e móveis em navegação manual.  
**Como melhorar:** implementar um sistema simples de colisão por volumes (`AABB`) para paredes/objetos e restringir o movimento da câmera quando houver interseção.

**3) Falta de sombreamento mais realista**  
**Limitação atual:** a iluminação de pipeline fixo resolve o básico, mas não gera sombras projetadas nem variações mais físicas de luz.  
**Como melhorar:** migrar gradualmente para OpenGL moderno com shaders, começando por Phong em fragment shader e depois adicionando shadow mapping para portas, móveis e paredes.

**4) Simplicidade geométrica de alguns móveis**  
**Limitação atual:** alguns objetos foram modelados com primitivas básicas para priorizar integração funcional.  
**Como melhorar:** refinar proporções, adicionar detalhes (puxadores, bordas, pés, espessuras), normal maps e materiais distintos para melhorar percepção de profundidade e realismo.

**5) Iluminação dependente de orientação da porta**  
**Limitação atual:** a leitura visual da porta frontal ainda cai quando ela abre para o lado menos iluminado.  
**Como melhorar:** reforçar iluminação de preenchimento local perto da entrada, ajustar componentes ambiente/difusa do material da porta e revisar normais para manter resposta luminosa mais estável em rotações.

**6) Tour virtual com transições rígidas entre segmentos**  
**Limitação atual:** embora as 8 curvas de Bézier resolvam cortes de caminho, a mudança entre trechos ainda pode parecer mecânica em alguns pontos.  
**Como melhorar:** suavizar continuidade de velocidade e direção (continuidade C1/C2), aplicar easing temporal e incluir controle de orientação da câmera por tangente da curva.

### Os elementos de cada atividade prática
**Aula Prática 02 — Visualização 3D no OpenGL**  
Aplicamos os conceitos de visualização 3D principalmente no sistema de câmera livre e no enquadramento da cena. A movimentação deixou de ser estática e passou a ser controlada por orientação angular (`Yaw`/`Pitch`) e posição da câmera, com montagem da visão via `gluLookAt` em `src/main.cpp` e controle de navegação em `src/camera.cpp`. O projeto também utiliza projeção em perspectiva (`gluPerspective` no `reshape`), consolidando a noção de volume e profundidade exigida nessa prática.

**Aula Prática 03 — Transformações e Visibilidade 3D**  
Os princípios de transformação hierárquica (translação, rotação e escala) foram usados em toda a modelagem da casa e dos móveis, com uso intenso de pilha de matrizes (`glPushMatrix`/`glPopMatrix`) em módulos como `src/scene/*` e `src/furniture/*`. A visibilidade/oclusão foi tratada com `GL_DEPTH_TEST` ativo desde a inicialização em `src/main.cpp`, garantindo ordem visual correta entre paredes, portas, janelas e objetos internos, independentemente da ordem de desenho.

**Aula Prática 04 — Iluminação e Sombreamento**  
O projeto implementa iluminação multicamada no pipeline fixo: luz direcional externa ("Sol", `GL_LIGHT0`), luz de preenchimento (`GL_LIGHT7`) e luzes de cômodo (`GL_LIGHT1` a `GL_LIGHT6`) com atenuação para alcance local. Também foram configuradas componentes ambiente/difusa/especular e sombreamento suave (`GL_SMOOTH`). Esse conjunto foi essencial para evitar áreas totalmente escuras e melhorar a leitura volumétrica dos ambientes.

**Aula Prática 05 — Mapeamento de Texturas**  
A prática foi aplicada de forma direta no projeto com carregamento de múltiplas imagens por `stb_image` em `src/texturas_util.cpp` e uso extensivo de texturas em piso, paredes, telhado, móveis e céu. O mapeamento UV e os parâmetros de textura permitiram sair de superfícies sólidas para materiais mais realistas, incluindo diferentes respostas visuais em objetos como cama, sofá, mesa, porta, armários e gramado externo.

**Aula Prática 06 — Curvas Paramétricas**  
Os conceitos de curvas de Bézier foram incorporados ao Tour Virtual da câmera em `src/camera.cpp`. Em vez de um deslocamento linear, o percurso foi estruturado por múltiplos segmentos de curva interligados, permitindo navegação automática mais natural pelos cômodos e corredores. Essa implementação também aproveita a base de transformações da cena para sincronizar movimento da câmera e orientação de observação durante a animação.

### O que cada integrante fez
**Lucas**
- Escrita de comentários no código e manutenção do `README`.
- Implementação do módulo de janelas.
- Implementação do sistema de splines (curvas de Bézier do tour).
- Aplicação de texturas do ambiente externo (céu e grama).
- Implementação da interação por proximidade nas portas.
- Implementação da movimentação vertical da câmera (subir e descer).

**Luiz**
- Implementação de iluminação e sombreamento.
- Aplicação de texturas dos móveis.
- Estruturação/modelagem dos móveis.
- Estrutura da casa, planta e construção dos cômodos.
- Implementação do controle angular da câmera (mudança de direção do olhar).
