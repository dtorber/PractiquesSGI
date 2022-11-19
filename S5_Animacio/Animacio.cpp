/*!
	Manejar la animacion; variables temporales que actualizan la escena
 */

#define PROYECTO "ISGI::S2E02::Animacion"

#include <iostream>			
#include <Utilidades.h>
#include <sstream>

using namespace std;
//variables dependientes del tiempo
static float angulo = 0;
static int tasaFPS = 60;

void init()
// Inicializaciones
{


	// Inicializaciones 
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

//Mostra en el titol de la finestra els FPS actuals
void FPS() {
	int ahora, tiempo_transcurrido;
	//Amb el static cree la variable només la primera vegada que cride
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;
	//Cada vegada que cride a FPS s'incrementen els fotogrames
	fotogramas++;
	//calcule el temps que ha transcorregut entre 2 fotogrames
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;

	//Si ha transcorregut més d'un segon, mostre els FPS i reinciie el rellotge
	if (tiempo_transcurrido > 1000) {
		//Canviar el titol de la finestra
		stringstream titulo;
		titulo << "FPS= " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		//Reiniciar rellotge
		fotogramas = 0;
		antes = ahora;
	}
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2, 1, 2, 0, 0, 0, 0, 1, 0);

	ejes();
	glPushMatrix();
	glColor3fv(ROJO);
	glRotatef(angulo, 0, 1, 0);
	glutSolidTeapot(0.5);
	glColor3fv(BLANCO);
	glutWireTeapot(0.51);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1, 0, -1);
	glRotatef(angulo / 2, 0, 1, 0);
	glColor3fv(VERDE);
	glutSolidTeapot(0.5);
	glColor3fv(AMARILLO);
	glutWireTeapot(0.51);
	glPopMatrix();

	FPS();

	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	glViewport(0, 0, w, h);
	float relacion = float(w) / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//camara ortografica
	//los ifs son para mantener las distancias de las figuras
	/*
	if (relacion > 1)
		glOrtho(-2 * relacion, 2 * relacion, -2, 2, -2, 10);
	else
		glOrtho(-2, 2, -2 / relacion, 2 / relacion, -2, 10);
		*/
		//Camara perspectiva
		//si no se pone donde esta la camara se coloca por defecto en el eje de cordenadas mirando hacia -z
	gluPerspective(30, relacion, 0.1, 100);


}
//Callback d'atenció a l'event idle
void onIdle() {
	//Sense control del temps (pot funcionar diferent segons la maquina)
	//angulo += 0.1;
	//Amb control del temps -> Controle la velocitat de gir, quantes voltes dona per segon
	static const float vueltaPorSegundo = 1;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	angulo += vueltaPorSegundo * 360 * (ahora - antes) / 1000;
	antes = ahora;
	
	//Encolar un event de dibuix
	glutPostRedisplay();
}

//Callback d'atencio al compte arrere del timer
void onTimer(int tiempo) {
	//Encuar-se a ella mateixa
	glutTimerFunc(tiempo, onTimer, tiempo);
	onIdle();
}

int main(int argc, char** argv)
// Programa principal
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow(PROYECTO);
	init();

	// Registro de callbacks	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(onIdle);
	//glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}