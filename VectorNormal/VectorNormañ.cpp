/*
*	Estrella3D.cpp
*	Construcció de l'estrella de 6 estrelles de David
*	@author Diego Torres
*/

//Biblioteques de C++
#include <iostream>
//Bilbioteques gràfiques
#include <Utilidades.h>

using namespace std;

//Variables globals
static GLfloat coordenadesTriangle[12];
static GLuint triangleSuperior, triangleInferior, estrellaCompleta;
static float posCameraX, posCameraY, posCameraZ;

void init() {
	const float radiExterior = 1;
	const float radiInterior = 0.7;
	const float offset = PI / 2;
	posCameraX = 3;
	posCameraY = 3;
	posCameraZ = 3;
	glEnable(GL_DEPTH_TEST);
	//Si mirem el dibuix, v0 és interior, v1 és exterior i se va sumant
	//Llavors mirarem en cada i la (x,y) de l'interior i després de l'exterior
	for (auto i = 0; i < 12; i += 4) {
		coordenadesTriangle[i] = radiExterior * cos((i / 4.0) * 2 * PI / 3 + offset);
		coordenadesTriangle[i + 1] = radiExterior * sin((i / 4.0) * 2 * PI / 3 + offset);
		coordenadesTriangle[i + 2] = radiInterior * cos((i / 4.0) * 2 * PI / 3 + offset);
		coordenadesTriangle[i + 3] = radiInterior * sin((i / 4.0) * 2 * PI / 3 + offset);
	}
	//Per al mode retingut creem una llista que després ja imprimirem
	triangleSuperior = glGenLists(1);
	glNewList(triangleSuperior, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (auto i = 0; i < 12; i += 2) {
		glVertex3f(coordenadesTriangle[i], coordenadesTriangle[i + 1], 0.0);
	}
	//Afegim a mà els dos últims, perquè necessitem que tornen a estar connectats
	glVertex3f(coordenadesTriangle[0], coordenadesTriangle[1], 0.0);
	glVertex3f(coordenadesTriangle[2], coordenadesTriangle[3], 0.0);
	glEnd();
	glEndList();
	triangleInferior = glGenLists(1);
	glNewList(triangleInferior, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	for (auto i = 0; i < 12; i += 2) {
		glVertex3f(-coordenadesTriangle[i], -coordenadesTriangle[i + 1], 0.0);
	}
	//Afegim a mà els dos últims, perquè necessitem que tornen a estar connectats
	glVertex3f(-coordenadesTriangle[0], -coordenadesTriangle[1], 0.0);
	glVertex3f(-coordenadesTriangle[2], -coordenadesTriangle[3], 0.0);
	glEnd();
	glEndList();

	estrellaCompleta = glGenLists(1);
	glNewList(estrellaCompleta, GL_COMPILE);
	glCallList(triangleInferior);
	glCallList(triangleSuperior);
	glEndList();
}
//Callback de dibuix
void display() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posCameraX, posCameraY, posCameraZ, 0, 0, 0, 0, 1, 0);
	ejes();
	float posX = 1 * cos(30);
	float posY = 1 * sin(30);
	glPushMatrix();
	glColor3f(0, 0, 0);
	//glTranslatef(posX, posY, 0);
	glTranslatef(0, 1, 0);
	glRotatef(30, 0, 0, 1);
	glutSolidTeapot(0.2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(posX, posY, 0);
	glutSolidTeapot(0.2);
	glPopMatrix();
	//Renovar el dibuix
	glFlush();
}


void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	float relacio = float(w) / h;
	float distancia = sqrt(posCameraX * posCameraX + posCameraY * posCameraY + posCameraZ * posCameraZ);
	float fovy = asin(1 / distancia) * 2 * 180 / PI;
	//Seleccionar càmera: definir el volum de la vista
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Perspectiva
	gluPerspective(fovy, relacio, 1, 10);
}

void main(int argc, char** argv) {
	//Inicialitzacions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);

	//Crear la finestra
	glutCreateWindow("Estrella 3D");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
}