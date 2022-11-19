/*
*	Transformacions.cpp
*	Programa per a dibuixar una flor amb composició de displayLists i transformacions
*	@author Diego Torres
*/

//Biblioteques de C++
#include <iostream>
//Bilbioteques gràfiques
#include <Utilidades.h>

using namespace std;

//Variables globals
static GLuint petalo, corola, flor;

void init() {
	//Funció d'inicialització pròpia

	//Llistes de dibuix:
	//Pétalo:
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	glScalef(0.15, 0.5, 0.15);
	glColor3f(1, 1, 1);
	glutSolidSphere(1, 10, 10);

	glPopMatrix();
	glPopAttrib();
	glEndList();

	//corola
	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	for (int i = 0; i < 12; i++) {
		glPushMatrix();
		glRotatef(i * 360 / 12.0, 0, 0, 1);
		glTranslatef(0.0, 0.25, 0.0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(petalo);
		glPopMatrix();
	}
	glScalef(0.2, 0.2, 0.2);
	glColor3f(1, 1, 0);
	glutSolidSphere(1, 20, 20);

	glPopMatrix();
	glPopAttrib();
	glEndList();

	//Flor:
	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();

	//tallo
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.125, 0);
	glScalef(0.05, 0.75, 0.05);
	glutSolidCube(1.0);
	glPopMatrix();

	//corola
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glRotatef(13, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glCallList(corola);
	glPopMatrix();

	glPopMatrix();
	glPopAttrib();
	glEndList();
}
//Callback de dibuix
void display() {
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	init();
	ejes();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glCallList(flor);
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