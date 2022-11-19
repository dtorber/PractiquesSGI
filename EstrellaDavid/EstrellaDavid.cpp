/*
*	EstrellaDavid.cpp
*	Construcció de l'estrella de David mitjançant dos triangles, un superior i un inferior
*	@author Diego Torres
*/

//Biblioteques de C++
#include <iostream>
//Bilbioteques gràfiques
#include <Utilidades.h>

using namespace std;

//Variables globals
static GLfloat coordenadesTriangle[12];
static GLuint triangleSuperior, triangleInferior;

void init() {
	const float radiExterior = 1;
	const float radiInterior = 0.7;
	const float offset = PI / 2;
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
}
//Callback de dibuix
void display() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	init();
	ejes();
	glColor3f(0, 0, 0.3);
	glCallList(triangleSuperior);
	glCallList(triangleInferior);
	//Renovar el dibuix
	glFlush();
}


void main(int argc, char** argv) {
	//Inicialitzacions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);


	//Crear la finestra
	glutCreateWindow("Estrella de David");
	glutDisplayFunc(display);

	glutMainLoop();
}