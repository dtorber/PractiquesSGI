/*
*	Camera.cpp
*	Dibuixar amb diferents càmeres
*	@author Diego Torres
*/

//Biblioteques de C++
#include <iostream>
//Bilbioteques gràfiques
#include <Utilidades.h>

using namespace std;

//Variables globals

void init() {
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	float ar = float(w) / h;
	//Seleccionar càmera: definir el volum de la vista
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Ortogràfica
	/*if (ar > 1) glOrtho(-2 * ar, 2 * ar, -2, 2, -2, 10);
	else glOrtho(-2, 2, -2 / ar, 2 / ar, -2, 10);*/

	//Perspectiva
	gluPerspective(30, ar, 1, 10);
}
//Callback de dibuix
void display() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ejes();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Situar la càmera -> origen, punt a on mira, verticular subjectiva
	gluLookAt(0, 1 , 0, 0, 0, 0, 0, 0, 1);

	glColor3fv(ROJO);
	glutSolidTeapot(0.5);
	glColor3fv(BLANCO);
	glutWireTeapot(0.51);

	glPushMatrix();
	glTranslatef(-3, 0, -3);
	glColor3fv(ROJO);
	glutSolidTeapot(0.5);
	glColor3fv(BLANCO);
	glutWireTeapot(0.51);
	glPopMatrix();
	//Renovar el dibuix
	glFlush();
}


void main(int argc, char** argv) {
	//Inicialitzacions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);


	//Crear la finestra
	glutCreateWindow("Camera");
	glutDisplayFunc(display);

	glutMainLoop();
}