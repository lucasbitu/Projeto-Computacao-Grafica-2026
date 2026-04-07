#include "../include/moveis.h"
#include "../include/texturas_util.h"
#include <GL/glut.h>

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

static void drawChair(float tx, float tz, float rotYDeg) {
    GLfloat assento[] = { 0.25f, 0.22f, 0.35f, 1.0f };
    GLfloat encosto[] = { 0.22f, 0.2f, 0.32f, 1.0f };

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);
    glRotatef(rotYDeg, 0.0f, 1.0f, 0.0f);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, assento);
    glPushMatrix();
    glTranslatef(0.0f, 0.44f, 0.0f);
    glScalef(0.46f, 0.08f, 0.42f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, encosto);
    glPushMatrix();
    glTranslatef(0.0f, 0.72f, -0.18f);
    glScalef(0.44f, 0.48f, 0.09f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, assento);
    glPushMatrix();
    glTranslatef(-0.17f, 0.22f, -0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f, 0.22f, -0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.17f, 0.22f, 0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.17f, 0.22f, 0.15f);
    glScalef(0.07f, 0.44f, 0.07f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawDiningTable() {
    float tableX = 0.5f; 
    float tableZ = 4.5f; 

    glPushMatrix(); 
    glTranslatef(tableX, 0.5f, tableZ); 

    glPushMatrix(); 
        GLfloat tampo_difuso[] = { 0.6f, 0.4f, 0.2f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, tampo_difuso);
        glScalef(1.2f, 0.1f, 2.4f);  
        glutSolidCube(1.0); 
    glPopMatrix(); 

    GLfloat perna_difusa[] = { 0.4f, 0.2f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, perna_difusa);
    float px = 0.5f;
    float pz = 1.0f;

    glPushMatrix(); glTranslatef(-px, -0.45f,  pz); glScalef(0.1f, 0.9f, 0.1f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef( px, -0.45f,  pz); glScalef(0.1f, 0.9f, 0.1f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef(-px, -0.45f, -pz); glScalef(0.1f, 0.9f, 0.1f); glutSolidCube(1.0); glPopMatrix();
    glPushMatrix(); glTranslatef( px, -0.45f, -pz); glScalef(0.1f, 0.9f, 0.1f); glutSolidCube(1.0); glPopMatrix();

    glPopMatrix(); 

    drawChair(tableX - 0.9f, tableZ - 0.6f, 90.0f);
    drawChair(tableX - 0.9f, tableZ + 0.6f, 90.0f);
    drawChair(tableX + 0.9f, tableZ - 0.6f, -90.0f);
    drawChair(tableX + 0.9f, tableZ + 0.6f, -90.0f);
    drawChair(tableX, tableZ - 1.5f, 0.0f);   
    drawChair(tableX, tableZ + 1.5f, 180.0f); 
}

static void drawBed(float cx, float cz) {
    GLfloat colchao[] = { 0.52f, 0.5f, 0.62f, 1.0f };
    GLfloat cabeceira[] = { 0.38f, 0.24f, 0.14f, 1.0f };

    const float compX = 2.0f;
    const float largZ = 2.25f;

    glPushMatrix();
    glTranslatef(cx, 0.14f, cz);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colchao);
    glScalef(compX, 0.28f, largZ);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cx, 0.52f, cz - 0.5f * largZ - 0.06f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cabeceira);
    glScalef(2.05f, 0.9f, 0.12f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

static void drawStudyDesk(float tx, float tz) {
    GLfloat tampo[] = { 0.52f, 0.36f, 0.2f, 1.0f };
    GLfloat perna[] = { 0.32f, 0.18f, 0.1f, 1.0f };

    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);

    glPushMatrix();
    glTranslatef(0.0f, 0.76f, 0.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tampo);
    glScalef(1.35f, 0.07f, 0.58f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, perna);
    glPushMatrix();
    glTranslatef(-0.52f, 0.38f, 0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.52f, 0.38f, 0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.52f, 0.38f, -0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.52f, 0.38f, -0.22f);
    glScalef(0.08f, 0.76f, 0.08f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void drawWardrobe(float tx, float tz, float rotYDeg) {
    GLfloat mat_madeira[] = { 0.45f, 0.28f, 0.15f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_madeira);

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

void drawBedroomsFurniture() {
    drawBed(0.35f, -8.85f);
    drawStudyDesk(2.45f, -2.75f);
    drawChair(2.45f, -1.85f, 180.0f);
    drawWardrobe(-2.7f, -5.0f, 90.0f); 

    drawBed(7.0f, -8.85f);
    drawStudyDesk(8.4f, -2.75f);
    drawChair(8.4f, -1.85f, 180.0f);
    drawWardrobe(9.7f, -5.0f, -90.0f); 
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
}

void drawBathroomFurniture() {
    GLfloat porcelana[] = { 0.92f, 0.92f, 0.95f, 1.0f };
    GLfloat cromado[] = { 0.72f, 0.74f, 0.78f, 1.0f };
    GLfloat agua[] = { 0.5f, 0.62f, 0.72f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);

    glPushMatrix();
    glTranslatef(9.05f, 0.22f, 7.0f);
    glScalef(1.0f, 0.42f, 2.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, agua);
    glPushMatrix();
    glTranslatef(9.05f, 0.30f, 7.0f);
    glScalef(0.82f, 0.14f, 1.78f);
    glutSolidCube(1.0f);
    glPopMatrix();

    const float piaPx = 3.58f, piaPz = 6.4f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.34f, 0.0f);
    glScalef(0.40f, 0.68f, 0.44f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.71f, 0.03f);
    glScalef(0.46f, 0.12f, 0.38f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cromado);
    glPushMatrix();
    glTranslatef(piaPx, 0.0f, piaPz);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.82f, -0.18f);
    glScalef(0.05f, 0.14f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    const float vasoPx = 7.85f, vasoPz = 9.05f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porcelana);
    glPushMatrix();
    glTranslatef(vasoPx, 0.0f, vasoPz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.21f, 0.0f);
    glScalef(0.38f, 0.24f, 0.48f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(vasoPx, 0.0f, vasoPz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.50f, -0.27f);
    glScalef(0.34f, 0.42f, 0.22f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawLivingRoomFurniture() {
    glDisable(GL_TEXTURE_2D);

    GLfloat mat_sofa[] = { 0.25f, 0.3f, 0.35f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_sofa);

    glPushMatrix();
        glTranslatef(-5.5f, 0.0f, 6.0f); 
        
        glPushMatrix();
            glTranslatef(0.0f, 0.25f, 0.0f);
            glScalef(1.0f, 0.5f, 2.5f);
            glutSolidCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.35f, 0.6f, 0.0f);
            glScalef(0.3f, 0.7f, 2.5f);
            glutSolidCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.0f, 0.45f, 1.35f);
            glScalef(1.0f, 0.5f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.0f, 0.45f, -1.35f);
            glScalef(1.0f, 0.5f, 0.2f);
            glutSolidCube(1.0);
        glPopMatrix();
    glPopMatrix();

    GLfloat mat_rack[] = { 0.45f, 0.28f, 0.15f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_rack);
    
    glPushMatrix();
        glTranslatef(-9.5f, 0.25f, 6.0f);
        glScalef(0.6f, 0.5f, 2.2f);
        glutSolidCube(1.0);
    glPopMatrix();

    GLfloat mat_tv_borda[] = { 0.1f, 0.1f, 0.1f, 1.0f }; 
    GLfloat mat_tv_tela[]  = { 0.05f, 0.05f, 0.1f, 1.0f }; 
    
    glPushMatrix();
        glTranslatef(-9.5f, 0.85f, 6.0f); 
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_tv_borda);
        glPushMatrix();
            glTranslatef(0.0f, -0.2f, 0.0f);
            glScalef(0.4f, 0.05f, 0.6f);
            glutSolidCube(1.0);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.0f, -0.1f, 0.0f);
            glScalef(0.1f, 0.2f, 0.1f);
            glutSolidCube(1.0);
        glPopMatrix();

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_tv_tela);
        glScalef(0.1f, 0.9f, 1.6f);
        glutSolidCube(1.0);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D); 
}