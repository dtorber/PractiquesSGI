/*
*	Geometria.cpp
*	Ús bàsic de geometria usant mode immediat i retingut
*	@author Diego Torres
*/

//Biblioteques de C++
#include <iostream>
//Bilbioteques gràfiques
#include <Utilidades.h>

using namespace std;

//Variables globals
static GLfloat coordenades[10];
static GLuint pentagon;

void init() {
	//Funció d'inicialització pròpia
	const float radi = 0.5;
	for (auto i = 0; i < 10; i += 2) {
		coordenades[i] = radi * cos((i / 2.0) * 2 * PI / 5);
		coordenades[i + 1] = radi * sin((i / 2.0) * 2 * PI / 5);
	}
	//Per al mode retingut cree la llista pentagon
	pentagon = glGenLists(1);
	glNewList(pentagon, GL_COMPILE);
	glBegin(GL_POLYGON);
	for (auto i = 0; i < 10; i += 2) {
		glVertex3f(coordenades[i], coordenades[i + 1], 0.0);
	}
	glEnd();
	glEndList();
}
//Callback de dibuix
void display() {
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	init();
	ejes();
	//Mode immediat
	//glutWireTeapot(0.5);
	/*glBegin(GL_LINE_LOOP);
	for (auto i = 0; i < 10; i += 2) {
		glVertex3f(coordenades[i], coordenades[i + 1], 0.0);
	}
	glEnd();*/
	//Mode retingut
	glCallList(pentagon);
	//Renovar el dibuix
	glFlush();
}

void reshape(GLint w, GLint h) {
	//w, h representen les noves dimensions de la finestra
}

void main(int argc, char** argv) {
	//Inicialitzacions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 400);
	glutInitWindowPosition(0, 0);


	//Crear la finestra
	glutCreateWindow("Hola OpenGL");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();
}