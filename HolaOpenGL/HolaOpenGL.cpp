/*
*	HolaOpenGL.cpp
*	Codi plantilla bàsic per a una aplicació de OpenGL
*	@author Diego Torres
*/

//Biblioteques de C++
#include <iostream>
//Bilbioteques gràfiques
#include <GL/freeglut.h>

using namespace std;

//Callback de dibuix
void display() {
	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT);
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