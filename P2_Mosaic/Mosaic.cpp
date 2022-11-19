/*
	GeometriaL.cpp
	Estrella de David + mosaico

	@author Marc Mestre
*/

// Biblioteca de C++
#include <iostream>

//Bibliotecas graficas

#include <Utilidades.h>

using namespace std;

//GLOBALES
static GLfloat coordenadas[10];
static GLuint estrelladoble;
static GLuint estrella;
static GLuint estrella2;
static GLuint estrella_completa;
void init() {
	//fucion propia de inicializacones

	//Coordenadas del pentagono
	const float radio1 = 1.0;
	const float radio2 = 0.7;

	estrella = glGenLists(1);


	glNewList(estrella, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0, 0, 1);
	for (auto i = 0; i < 3; i += 1) {
		glVertex3f(radio2 * -sin(i * 2 * PI / 3), radio2 * cos(i * 2 * PI / 3), 0.0);
		glVertex3f(radio1 * -sin(i * 2 * PI / 3), radio1 * cos(i * 2 * PI / 3), 0.0);
	}
	glVertex3f(radio2 * -sin(0 * 2 * PI / 3), radio2 * cos(0 * 2 * PI / 3), 0.0);
	glVertex3f(radio1 * -sin(0 * 2 * PI / 3), radio1 * cos(0 * 2 * PI / 3), 0.0);

	glEnd();
	glEndList();
	estrella2 = glGenLists(1);

	glNewList(estrella2, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1, 0, 0);
	for (auto i = 1; i < 3; i += 1) {
		glVertex3f(radio2 * sin(i * 2 * PI / 3), radio2 * -cos(i * 2 * PI / 3), 0.0);
		glVertex3f(radio1 * sin(i * 2 * PI / 3), radio1 * -cos(i * 2 * PI / 3), 0.0);
	}
	for (auto i = 0; i < 2; i += 1) {
		glVertex3f(radio2 * sin(i * 2 * PI / 3), radio2 * -cos(i * 2 * PI / 3), 0.0);
		glVertex3f(radio1 * sin(i * 2 * PI / 3), radio1 * -cos(i * 2 * PI / 3), 0.0);
	}


	glEnd();
	glEndList();
	estrelladoble = glGenLists(1);
	glNewList(estrelladoble, GL_COMPILE);
	glCallList(estrella);
	glCallList(estrella2);
	glEndList();

	estrella_completa = glGenLists(1);
	glNewList(estrella_completa, GL_COMPILE);

	//estrella de dalt a l'esquerre
	glPushMatrix();
	glTranslatef(-0.5, 0.5, 0);
	glScalef(0.5, 0.5, 0);
	glRotatef(15, 0, 0, 1);
	glCallList(estrelladoble);
	glRotatef(-30, 0, 0, 1);
	glScalef(0.4, 0.4, 0);
	glCallList(estrelladoble);
	glPopMatrix();

	//estrella de dalt a la dreta

	glPushMatrix();
	glTranslatef(0.5, 0.5, 0);
	glScalef(0.5, 0.5, 0);
	glRotatef(-15, 0, 0, 1);
	glCallList(estrelladoble);
	glRotatef(30, 0, 0, 1);
	glScalef(0.4, 0.4, 0);
	glCallList(estrelladoble);
	glPopMatrix();

	//estrella de baix a l'esquerre

	glPushMatrix();
	glTranslatef(0.5, -0.5, 0);
	glScalef(0.5, 0.5, 0);
	glRotatef(15, 0, 0, 1);
	glCallList(estrelladoble);
	glRotatef(-30, 0, 0, 1);
	glScalef(0.4, 0.4, 0);
	glCallList(estrelladoble);
	glPopMatrix();

	//estrella de dalt a la dreta

	glPushMatrix();
	glTranslatef(-0.5, -0.5, 0);
	glScalef(0.5, 0.5, 0);
	glRotatef(-15, 0, 0, 1);
	glCallList(estrelladoble);
	glRotatef(30, 0, 0, 1);
	glScalef(0.4, 0.4, 0);
	glCallList(estrelladoble);
	glPopMatrix();
	glEndList();

}

void display() {
	//callback de dibujo
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ejes();
	glCallList(estrella_completa);
	//Renovar dibujo
	glutSwapBuffers();
	//glFlush();
}




void reshape(GLint w, GLint h) {
	// w h son las nuevas dimensiones de la ventana
}

void main(int argc, char** argv) {

	//inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 400);
	glutInitWindowPosition(0, 0);


	// Crear la ventana
	glutCreateWindow("Hola OpenGL");
	init();
	// Seccion de registro  de callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	//pone en marcha el bucle de atencion de eventos
	glutMainLoop();


}