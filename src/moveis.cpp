#include "../include/moveis.h"
#include "../include/texturas_util.h"
#include <GL/glut.h>

/* Escala linear +50% para mesas (jantar/estudo) e cadeiras */
static const float FURN_SCALE = 1.5f;
/* Escala para camas, pia e vaso sanitário (~+30%) */
static const float BATH_BED_SCALE = 1.3f;
/* Parede de fundo dos quartos em z (cenario drawHouse); margem para cabeceira não atravessar */
static const float QUARTO_PAREDE_FUNDO_Z = -10.0f;

void drawTexturedCube(GLuint texFrente, GLuint texLados, GLuint texTopo) {
    if (texFrente > 0) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texFrente); }
    else glDisable(GL_TEXTURE_2D);
    
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
    glEnd();

    if (texLados > 0) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texLados); }
    else glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
        
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();

    if (texTopo > 0) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texTopo); }
    else glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
}

/* Base e bordas do tampo com cor de madeira; topo com textura (cladding ~1,3 m) */
static void drawTabletopWithOakTop(float halfW, float halfH, float halfD,
                                   float sideR, float sideG, float sideB) {
    GLfloat sideMat[] = { sideR, sideG, sideB, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sideMat);
    glDisable(GL_TEXTURE_2D);
    const float yb = -halfH;
    const float yt = halfH;
    glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-halfW, yb, -halfD);
        glVertex3f( halfW, yb, -halfD);
        glVertex3f( halfW, yb,  halfD);
        glVertex3f(-halfW, yb,  halfD);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-halfW, yb, -halfD);
        glVertex3f(-halfW, yb,  halfD);
        glVertex3f(-halfW, yt,  halfD);
        glVertex3f(-halfW, yt, -halfD);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(halfW, yb,  halfD);
        glVertex3f(halfW, yb, -halfD);
        glVertex3f(halfW, yt, -halfD);
        glVertex3f(halfW, yt,  halfD);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-halfW, yb, -halfD);
        glVertex3f( halfW, yb, -halfD);
        glVertex3f( halfW, yt, -halfD);
        glVertex3f(-halfW, yt, -halfD);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(halfW, yb, halfD);
        glVertex3f(-halfW, yb, halfD);
        glVertex3f(-halfW, yt, halfD);
        glVertex3f(halfW, yt, halfD);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMesaTampo);
    /* Ambiente + difuso: evita topo preto se o modo two-side inverter a face */
    GLfloat topAmb[] = { 0.28f, 0.28f, 0.26f, 1.0f };
    GLfloat topDif[] = { 0.95f, 0.95f, 0.93f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, topAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, topDif);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    const float tile = 1.3f;
    float repU = (2.0f * halfW) / tile;
    float repV = (2.0f * halfD) / tile;
    /* Ordem CCW vista de cima (+Y): normal geométrica coincide com (0,1,0) */
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);  glVertex3f(-halfW, yt, -halfD);
        glTexCoord2f(0.0f, repV);  glVertex3f(-halfW, yt,  halfD);
        glTexCoord2f(repU, repV);  glVertex3f( halfW, yt,  halfD);
        glTexCoord2f(repU, 0.0f);  glVertex3f( halfW, yt, -halfD);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

/* Caixa centrada na origem, textura nas 6 faces (k escala repetição UV; ma/md modulam luminosidade) */
static void drawTexturedBox6(GLuint tex, float hx, float hy, float hz, float k,
                             float ma = 0.24f, float md = 0.86f) {
    const float uxz = k * (2.0f * hx + 2.0f * hz);
    const float vh = k * (2.0f * hy);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    GLfloat mAmb[] = { ma, ma, ma, 1.0f };
    GLfloat mDif[] = { md, md, md, 1.0f };
    GLfloat zsp[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zsp);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(uxz, 0.0f);  glVertex3f( hx, -hy, hz);
        glTexCoord2f(uxz, vh);    glVertex3f( hx,  hy, hz);
        glTexCoord2f(0.0f, vh);   glVertex3f(-hx,  hy, hz);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(uxz, 0.0f);  glVertex3f( hx, -hy, -hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(0.0f, vh);   glVertex3f(-hx,  hy, -hz);
        glTexCoord2f(uxz, vh);    glVertex3f( hx,  hy, -hz);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(uxz, 0.0f);  glVertex3f(-hx, -hy,  hz);
        glTexCoord2f(uxz, vh);    glVertex3f(-hx,  hy,  hz);
        glTexCoord2f(0.0f, vh);   glVertex3f(-hx,  hy, -hz);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(uxz, 0.0f);  glVertex3f(hx, -hy,  hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(0.0f, vh);   glVertex3f(hx,  hy, -hz);
        glTexCoord2f(uxz, vh);    glVertex3f(hx,  hy,  hz);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy,  hz);
        glTexCoord2f(uxz, 0.0f);  glVertex3f( hx, hy,  hz);
        glTexCoord2f(uxz, uxz);   glVertex3f( hx, hy, -hz);
        glTexCoord2f(0.0f, uxz);  glVertex3f(-hx, hy, -hz);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(uxz, 0.0f);  glVertex3f( hx, -hy, -hz);
        glTexCoord2f(uxz, uxz);   glVertex3f( hx, -hy,  hz);
        glTexCoord2f(0.0f, uxz);  glVertex3f(-hx, -hy,  hz);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

static void drawTexturedLegBox(float hx, float hy, float hz) {
    drawTexturedBox6(texMesaPe, hx, hy, hz, 1.8f);
}

static void drawChair(float tx, float tz, float rotYDeg) {
    const float S = FURN_SCALE;
    const float ck = 2.0f;
    const float shx = 0.23f * S;
    const float shy = 0.04f * S;
    const float shz = 0.21f * S;
    const float bhx = 0.22f * S;
    const float bhy = 0.24f * S;
    const float bhz = 0.045f * S;
    const float lhx = 0.035f * S;
    const float lhy = 0.22f * S;
    const float lhz = 0.035f * S;

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.44f * S, 0.0f);
    drawTexturedBox6(texCadeiras, shx, shy, shz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.72f * S, -0.18f * S);
    drawTexturedBox6(texCadeiras, bhx, bhy, bhz, ck);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.17f * S, 0.22f * S, -0.15f * S);
    drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f * S, 0.22f * S, -0.15f * S);
    drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.17f * S, 0.22f * S, 0.15f * S);
    drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f * S, 0.22f * S, 0.15f * S);
    drawTexturedBox6(texCadeiras, lhx, lhy, lhz, ck);
    glPopMatrix();

    glPopMatrix();
}

void drawDiningTable() {
    const float S = FURN_SCALE;
    float tableX = 0.5f;
    float tableZ = 4.5f;

    glPushMatrix();
    glTranslatef(tableX, 0.5f * S, tableZ);

    glPushMatrix();
        drawTabletopWithOakTop(0.6f * S, 0.05f * S, 1.2f * S, 0.6f, 0.4f, 0.2f);
    glPopMatrix();

    float px = 0.5f * S;
    float pz = 1.0f * S;
    const float lhx = 0.05f * S;
    const float lhy = 0.45f * S;
    const float lhz = 0.05f * S;

    glPushMatrix(); glTranslatef(-px, -0.45f * S,  pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();
    glPushMatrix(); glTranslatef( px, -0.45f * S,  pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();
    glPushMatrix(); glTranslatef(-px, -0.45f * S, -pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();
    glPushMatrix(); glTranslatef( px, -0.45f * S, -pz); drawTexturedLegBox(lhx, lhy, lhz); glPopMatrix();

    glPopMatrix();

    drawChair(tableX - 0.9f * S, tableZ - 0.6f * S, 90.0f);
    drawChair(tableX - 0.9f * S, tableZ + 0.6f * S, 90.0f);
    drawChair(tableX + 0.9f * S, tableZ - 0.6f * S, -90.0f);
    drawChair(tableX + 0.9f * S, tableZ + 0.6f * S, -90.0f);
    drawChair(tableX, tableZ - 1.5f * S, 0.0f);
    drawChair(tableX, tableZ + 1.5f * S, 180.0f);
}

/* Face +Z local = vista do quarto / colchão; restantes cor madeira */
static void drawBedHeadboard(float cx, float cz, float B, float largZ) {
    GLfloat cabeceira[] = { 0.38f, 0.24f, 0.14f, 1.0f };
    GLfloat cabAmb[] = { 0.12f, 0.08f, 0.06f, 1.0f };
    GLfloat zsp[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    /* Centro Y alinhado ao colchão + base (topo ~0,35*B) */
    glPushMatrix();
    glTranslatef(cx, 0.59f * B, cz - 0.5f * largZ - 0.06f * B);
    glScalef(2.05f * B, 0.9f * B, 0.12f * B);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cabAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cabeceira);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zsp);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glDisable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f,  0.5f);
        glVertex3f(0.5f,  0.5f,  0.5f);
        glVertex3f(0.5f,  0.5f, -0.5f);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f,  0.5f);
        glVertex3f( 0.5f, 0.5f,  0.5f);
        glVertex3f( 0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
    glEnd();

    if (texCabeceiraCama != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texCabeceiraCama);
        GLfloat mAmb[] = { 0.32f, 0.32f, 0.30f, 1.0f };
        GLfloat mDif[] = { 0.90f, 0.90f, 0.88f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mDif);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, 0.5f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, 0.5f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, 0.5f);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cabAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cabeceira);
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f( 0.5f, -0.5f, 0.5f);
            glVertex3f( 0.5f,  0.5f, 0.5f);
            glVertex3f(-0.5f,  0.5f, 0.5f);
        glEnd();
    }

    glPopMatrix();
}

static void drawBed(float cx, float cz) {
    const float B = BATH_BED_SCALE;
    const float compX = 2.0f * B;
    const float largZ = 2.25f * B;
    const float hBase = 0.14f * B;
    const float hColchao = 0.21f * B;
    const float colchaoEncolher = 0.97f;

    GLfloat baseMadeira[] = { 0.30f, 0.20f, 0.14f, 1.0f };
    GLfloat baseAmb[] = { 0.08f, 0.05f, 0.04f, 1.0f };
    GLfloat colchao[] = { 0.54f, 0.51f, 0.66f, 1.0f };
    GLfloat colchaoAmb[] = { 0.20f, 0.19f, 0.26f, 1.0f };

    const float hxB = 0.5f * compX;
    const float hyB = 0.5f * hBase;
    const float hzB = 0.5f * largZ;
    glPushMatrix();
    glTranslatef(cx, hyB, cz);
    if (texBaseCama != 0) {
        /* k ajusta repetição nas faces laterais da base */
        drawTexturedBox6(texBaseCama, hxB, hyB, hzB, 0.38f, 0.28f, 0.88f);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, baseAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, baseMadeira);
        glScalef(compX, hBase, largZ);
        glutSolidCube(1.0f);
    }
    glPopMatrix();

    const float compM = compX * colchaoEncolher;
    const float largM = largZ * colchaoEncolher;
    const float hxC = 0.5f * compM;
    const float hyC = 0.5f * hColchao;
    const float hzC = 0.5f * largM;
    glPushMatrix();
    glTranslatef(cx, hBase + hyC, cz);
    if (texColchaoCama != 0) {
        drawTexturedBox6(texColchaoCama, hxC, hyC, hzC, 0.42f, 0.30f, 0.90f);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colchaoAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colchao);
        glScalef(compM, hColchao, largM);
        glutSolidCube(1.0f);
    }
    glPopMatrix();

    drawBedHeadboard(cx, cz, B, largZ);
}

static void drawStudyDesk(float tx, float tz) {
    const float S = FURN_SCALE;
    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);

    glPushMatrix();
    glTranslatef(0.0f, 0.76f * S, 0.0f);
    drawTabletopWithOakTop(0.675f * S, 0.035f * S, 0.29f * S, 0.52f, 0.36f, 0.2f);
    glPopMatrix();

    const float dhx = 0.04f * S;
    const float dhy = 0.38f * S;
    const float dhz = 0.04f * S;
    glPushMatrix();
    glTranslatef(-0.52f * S, 0.38f * S, 0.22f * S);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.52f * S, 0.38f * S, 0.22f * S);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.52f * S, 0.38f * S, -0.22f * S);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.52f * S, 0.38f * S, -0.22f * S);
    drawTexturedLegBox(dhx, dhy, dhz);
    glPopMatrix();

    glPopMatrix();
}

void drawWardrobe(float tx, float tz, float rotYDeg) {
    GLfloat mat_branco[] = { 0.95f, 0.95f, 0.95f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branco);

    glPushMatrix();
        glTranslatef(tx, 1.1f, tz); 
        glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);
        
        glPushMatrix();
            glScalef(1.5f, 2.2f, 0.6f);
            glDisable(GL_TEXTURE_2D); 
            glutSolidCube(1.0);
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texGuardaRoupa);
        
        glBegin(GL_QUADS);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.24f, 0.08f); glVertex3f(-0.75f, -1.1f, 0.301f); 
            glTexCoord2f(0.76f, 0.08f); glVertex3f( 0.75f, -1.1f, 0.301f); 
            glTexCoord2f(0.76f, 0.88f); glVertex3f( 0.75f,  1.1f, 0.301f); 
            glTexCoord2f(0.24f, 0.88f); glVertex3f(-0.75f,  1.1f, 0.301f); 
        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
}

/* Só o tampo/lavatório (retângulo superior da pia) apoiado no plano wyCounterTop do balcão */
static void drawKitchenSinkTopOnCounter(float wx, float wyCounterTop, float wz, float rotYDeg) {
    const float Pb = BATH_BED_SCALE;
    const float marmK = 1.1f;
    const float marmMa = 0.26f;
    const float marmMd = 0.88f;
    GLfloat porcelana[] = { 0.92f, 0.92f, 0.95f, 1.0f };
    GLfloat porcelanaAmb[] = { 0.35f, 0.35f, 0.38f, 1.0f };
    const float htx = 0.5f * 0.46f * Pb;
    const float hty = 0.5f * 0.12f * Pb;
    const float htz = 0.5f * 0.38f * Pb;

    auto drawLouca = [&](float hx, float hy, float hz) {
        if (texMarmore2 != 0)
            drawTexturedBox6(texMarmore2, hx, hy, hz, marmK, marmMa, marmMd);
        else {
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
            glScalef(2.0f * hx, 2.0f * hy, 2.0f * hz);
            glutSolidCube(1.0f);
        }
    };

    glPushMatrix();
    glTranslatef(wx, wyCounterTop, wz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, hty, 0.03f * Pb);
    drawLouca(htx, hty, htz);
    glPopMatrix();

    /* Torneira (mesmo bloco cinza que no banheiro), acima do tampo da pia */
    glDisable(GL_TEXTURE_2D);
    GLfloat cinzaMetalAmb[] = { 0.045f, 0.045f, 0.048f, 1.0f };
    GLfloat cinzaMetalDif[] = { 0.14f, 0.14f, 0.155f, 1.0f };
    GLfloat zeroSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cinzaMetalAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cinzaMetalDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glPushMatrix();
    glTranslatef(wx, wyCounterTop, wz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 2.0f * hty + 0.05f * Pb, -0.18f * Pb);
    glScalef(0.05f * Pb, 0.14f * Pb, 0.05f * Pb);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawBedroomsFurniture() {
    /* Centro Z: borda -Z da cabeceira = cz - 1,245*B (colchão + offset + meia espessura cabeceira) */
    const float bedZ = QUARTO_PAREDE_FUNDO_Z + 0.08f + 1.245f * BATH_BED_SCALE;
    drawBed(0.35f, bedZ);
    drawStudyDesk(2.45f, -2.75f);
    drawChair(2.45f, -1.85f, 180.0f);
    /* Centros ligeiramente afastados das paredes x=-3 e x=10 para evitar z-fighting */
    drawWardrobe(-2.62f, -5.0f, 90.0f);

    drawBed(7.0f, bedZ);
    drawStudyDesk(8.4f, -2.75f);
    drawChair(8.4f, -1.85f, 180.0f);
    drawWardrobe(9.62f, -5.0f, -90.0f); 
}

void drawKitchenFurniture() {
    GLfloat mat_branca[] = { 0.8f, 0.8f, 0.8f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branca);

    glPushMatrix();
        glTranslatef(-9.0f, 1.0f, -9.2f); 
        glScalef(0.8f, 2.0f, 0.8f);
        drawTexturedCube(texGeladeira, 0, 0);
    glPopMatrix();

    GLfloat mat_neutro[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_neutro);
    
    glPushMatrix();
        glTranslatef(-5.0f, 0.45f, -9.2f); 
        glScalef(2.5f, 0.9f, 0.8f);
        drawTexturedCube(texMarmore, texMarmore, texMarmore);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_branca);
    
    glPushMatrix();
        glTranslatef(-7.5f, 0.45f, -9.2f); 
        glScalef(0.8f, 0.9f, 0.8f);
        drawTexturedCube(texFogaoFrente, 0, texFogaoCima);
    glPopMatrix();

    /* Pia da cozinha: apenas o bloco superior sobre o tampo do balcão (y = 0,45 + 0,45) */
    drawKitchenSinkTopOnCounter(-5.0f, 0.9f, -9.2f, 0.0f);
}

void drawBathroomFurniture() {
    GLfloat porcelana[] = { 0.92f, 0.92f, 0.95f, 1.0f };
    GLfloat porcelanaAmb[] = { 0.35f, 0.35f, 0.38f, 1.0f };
    GLfloat aguaAmb[] = { 0.12f, 0.22f, 0.34f, 1.0f };
    GLfloat aguaDif[] = { 0.32f, 0.58f, 0.78f, 0.42f };

    const float bx = 9.05f;
    const float by = 0.22f;
    const float bz = 7.0f;
    /* Banheira ~12% maior que o bloco original scale(1, 0.42, 2.05) */
    const float tubS = 1.12f;
    const float hx = 0.5f * tubS;
    const float hy = 0.21f * tubS;
    const float hz = 1.025f * tubS;
    const float wt = 0.048f * tubS;
    const float ix = hx - wt;
    const float iz = hz - wt;
    const float yInBottom = -hy + wt;
    const float yInTop = hy - wt;
    const float yWater = yInTop - 0.055f;
    const float yWBot = yInBottom + 0.015f;
    const float inset = 0.012f;
    const float wx = ix - inset;
    const float wz = iz - inset;
    const float tubMarmK = 1.05f;
    const float tubMarmMa = 0.26f;
    const float tubMarmMd = 0.88f;

    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);

    glPushMatrix();
    glTranslatef(bx, by, bz);

    /* Banheira: blocos sólidos com mármore (ou louça se textura indisponível) */
    auto drawTubBox = [&](float cx, float cy, float cz, float hfx, float hfy, float hfz) {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        if (texMarmore2 != 0)
            drawTexturedBox6(texMarmore2, hfx, hfy, hfz, tubMarmK, tubMarmMa, tubMarmMd);
        else {
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
            glScalef(2.0f * hfx, 2.0f * hfy, 2.0f * hfz);
            glutSolidCube(1.0f);
        }
        glPopMatrix();
    };

    drawTubBox(0.0f, -hy + 0.5f * wt, 0.0f, hx + wt, 0.5f * wt, hz + wt);
    drawTubBox(-hx - 0.5f * wt, 0.0f, 0.0f, 0.5f * wt, hy, hz + wt);
    drawTubBox(hx + 0.5f * wt, 0.0f, 0.0f, 0.5f * wt, hy, hz + wt);
    drawTubBox(0.0f, 0.0f, -hz - 0.5f * wt, hx, hy, 0.5f * wt);
    drawTubBox(0.0f, 0.0f, hz + 0.5f * wt, hx, hy, 0.5f * wt);

    /* Volume de água semitransparente (renderizado por cima da louça opaca) */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, aguaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, aguaDif);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-wx, yWater, -wz);
        glVertex3f(-wx, yWater,  wz);
        glVertex3f( wx, yWater,  wz);
        glVertex3f( wx, yWater, -wz);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-wx, yWBot, -wz);
        glVertex3f( wx, yWBot, -wz);
        glVertex3f( wx, yWBot,  wz);
        glVertex3f(-wx, yWBot,  wz);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-wx, yWBot, -wz);
        glVertex3f(-wx, yWBot,  wz);
        glVertex3f(-wx, yWater,  wz);
        glVertex3f(-wx, yWater, -wz);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(wx, yWBot,  wz);
        glVertex3f(wx, yWBot, -wz);
        glVertex3f(wx, yWater, -wz);
        glVertex3f(wx, yWater,  wz);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-wx, yWBot, -wz);
        glVertex3f(-wx, yWater, -wz);
        glVertex3f( wx, yWater, -wz);
        glVertex3f( wx, yWBot, -wz);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-wx, yWBot, wz);
        glVertex3f( wx, yWBot, wz);
        glVertex3f( wx, yWater, wz);
        glVertex3f(-wx, yWater, wz);
    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();

    const float Pb = BATH_BED_SCALE;
    const float piaPx = 3.58f, piaPz = 6.4f;
    const float marmK = 1.1f;
    const float marmMa = 0.26f;
    const float marmMd = 0.88f;

    auto drawLoucaOuMarmore = [&](float hx, float hy, float hz) {
        if (texMarmore2 != 0)
            drawTexturedBox6(texMarmore2, hx, hy, hz, marmK, marmMa, marmMd);
        else {
            glDisable(GL_TEXTURE_2D);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
            glScalef(2.0f * hx, 2.0f * hy, 2.0f * hz);
            glutSolidCube(1.0f);
        }
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, porcelanaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.34f * Pb, 0.0f);
    drawLoucaOuMarmore(0.5f * 0.40f * Pb, 0.5f * 0.68f * Pb, 0.5f * 0.44f * Pb);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.71f * Pb, 0.03f * Pb);
    drawLoucaOuMarmore(0.5f * 0.46f * Pb, 0.5f * 0.12f * Pb, 0.5f * 0.38f * Pb);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    GLfloat cinzaMetalAmb[] = { 0.045f, 0.045f, 0.048f, 1.0f };
    GLfloat cinzaMetalDif[] = { 0.14f, 0.14f, 0.155f, 1.0f };
    GLfloat zeroSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cinzaMetalAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cinzaMetalDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.82f * Pb, -0.18f * Pb);
    glScalef(0.05f * Pb, 0.14f * Pb, 0.05f * Pb);
    glutSolidCube(1.0f);
    glPopMatrix();

    const float vasoPx = 7.85f, vasoPz = 9.05f;
    const float vasoS = 1.12f * Pb;
    glPushMatrix();
    glTranslatef(vasoPx, 0.0f, vasoPz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.21f * vasoS, 0.0f);
    drawLoucaOuMarmore(0.5f * 0.38f * vasoS, 0.5f * 0.24f * vasoS, 0.5f * 0.48f * vasoS);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(vasoPx, 0.0f, vasoPz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.50f * vasoS, -0.27f * vasoS);
    drawLoucaOuMarmore(0.5f * 0.34f * vasoS, 0.5f * 0.42f * vasoS, 0.5f * 0.22f * vasoS);
    glPopMatrix();
}

void drawLivingRoomFurniture() {
    const float sofaK = 1.55f;

    glPushMatrix();
        glTranslatef(-5.5f, 0.0f, 6.0f);

        glPushMatrix();
            glTranslatef(0.0f, 0.25f, 0.0f);
            drawTexturedBox6(texSofa, 0.5f, 0.25f, 1.25f, sofaK);
        glPopMatrix();

        /* Encosto acima do assento (assento y máx. 0,5) e ligeiramente em +X para não colidir em profundidade */
        glPushMatrix();
            glTranslatef(0.38f, 0.87f, 0.0f);
            drawTexturedBox6(texSofa, 0.15f, 0.35f, 1.21f, sofaK);
        glPopMatrix();

        /* Braços afastados em Z das faces do encosto (antes coplanares em |z|≈1,25) */
        glPushMatrix();
            glTranslatef(0.0f, 0.45f, 1.37f);
            drawTexturedBox6(texSofa, 0.5f, 0.25f, 0.1f, sofaK);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.45f, -1.37f);
            drawTexturedBox6(texSofa, 0.5f, 0.25f, 0.1f, sofaK);
        glPopMatrix();
    glPopMatrix();

    /* Mesa um pouco mais para o interior (parede em x = -10) */
    /* Mesa da TV: textura mais escura (evita aspecto “lavado” / branco) */
    glPushMatrix();
        glTranslatef(-9.46f, 0.25f, 6.0f);
        drawTexturedBox6(texMesaTv, 0.3f, 0.25f, 1.1f, 1.75f, 0.08f, 0.38f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    GLfloat mat_tv_borda[] = { 0.08f, 0.08f, 0.09f, 1.0f };
    GLfloat mat_tv_bordaAmb[] = { 0.04f, 0.04f, 0.045f, 1.0f };
    GLfloat mat_tv_zeroSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const float tvPy = 1.03f;

    /* Chapa entre mesa e painel — moldura escura, sem brilho especular */
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_tv_zeroSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_tv_bordaAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_tv_borda);
    glPushMatrix();
        glTranslatef(-9.46f, 0.515f, 6.0f);
        glScalef(0.38f, 0.02f, 0.58f);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-9.46f, tvPy, 6.0f);

        const float phx = 0.05f;
        const float phy = 0.45f;
        const float phz = 0.8f;

        /* Frente (+X): imagem a brilho pleno (sem iluminação — efeito ecrã ligado) */
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texConteudoTv);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(phx, -phy, -phz);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(phx, -phy,  phz);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(phx,  phy,  phz);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(phx,  phy, -phz);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_tv_zeroSpec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_tv_bordaAmb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_tv_borda);
        glBegin(GL_QUADS);
            glNormal3f(-1.0f, 0.0f, 0.0f);
            glVertex3f(-phx, -phy,  phz);
            glVertex3f(-phx, -phy, -phz);
            glVertex3f(-phx,  phy, -phz);
            glVertex3f(-phx,  phy,  phz);

            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-phx, phy,  phz);
            glVertex3f( phx, phy,  phz);
            glVertex3f( phx, phy, -phz);
            glVertex3f(-phx, phy, -phz);

            glNormal3f(0.0f, -1.0f, 0.0f);
            glVertex3f(-phx, -phy, -phz);
            glVertex3f( phx, -phy, -phz);
            glVertex3f( phx, -phy,  phz);
            glVertex3f(-phx, -phy,  phz);

            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-phx, -phy, phz);
            glVertex3f( phx, -phy, phz);
            glVertex3f( phx,  phy, phz);
            glVertex3f(-phx,  phy, phz);

            glNormal3f(0.0f, 0.0f, -1.0f);
            glVertex3f( phx, -phy, -phz);
            glVertex3f(-phx, -phy, -phz);
            glVertex3f(-phx,  phy, -phz);
            glVertex3f( phx,  phy, -phz);
        glEnd();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D); 
}