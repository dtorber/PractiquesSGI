/*
	Noria.cpp
	Simulació del moviment d'una nòria
	@author Diego Torres
*/

#define PROYECTO "ISGI::Entregable::Noria"

#include <iostream>			
#include <Utilidades.h>
#include <sstream>

using namespace std;
//variables dependientes del tiempo
static float angleCamera = 0, angleGirRoda = 0, angleGirVago = 0;
static int tasaFPS = 60;
static float posCameraX, posCameraY, posCameraZ;
static float separacioBases=1.2, separacioRodes = 0.6 * separacioBases;
static GLfloat coordenadesTriangle[12], estiraTriangle = 1.5;
static GLuint triangle, baseNoria, estructuraNoria, rodaNoria, rodes, noria, eixos, vagon, soportVago;
static float radiCamera = 8;
static const float radiExterior = 1;
static const float radiInterior = 0.7;
static const float offset = PI / 2;
static const float radiNoria = 0.35 * radiExterior * 3;
//static const float alturaNoria = 0.35 * (radiExterior + estiraTriangle);
static const float alturaNoria = 0;
static const int nVagons = 15;

void init()
{
	//Inicialitzacions
	//Ubicació de la càmera
	posCameraX = 2;
	posCameraY = 1;
	posCameraZ = 2;
	//Netejar el fons i activar l'ús de la profunditat
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Construim el dibuix d'un triangle que forma la base de la nòria

	//Generem tots els punts que van a composar el triangle (tant interior com exterior)
	for (auto i = 0; i < 12; i += 4) {
		coordenadesTriangle[i] = radiExterior * cos((i / 4.0) * 2 * PI / 3 + offset);
		coordenadesTriangle[i + 1] = radiExterior * sin((i / 4.0) * 2 * PI / 3 + offset);
		coordenadesTriangle[i + 2] = radiInterior * cos((i / 4.0) * 2 * PI / 3 + offset);
		coordenadesTriangle[i + 3] = radiInterior * sin((i / 4.0) * 2 * PI / 3 + offset);
	}

	triangle = glGenLists(1);
	glNewList(triangle, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);

	//Afegim estos dos vértexs a mà perquè estiga més estirat el triangle
	glVertex3f(coordenadesTriangle[0], coordenadesTriangle[1] + estiraTriangle, 0.0);
	glVertex3f(coordenadesTriangle[2], coordenadesTriangle[3] + estiraTriangle, 0.0);
	for (auto i = 4; i < 12; i += 2) {
		glVertex3f(coordenadesTriangle[i], coordenadesTriangle[i + 1], 0.0);
	}
	//Afegim a mà els dos últims, perquè necessitem que tornen a estar connectats
	glVertex3f(coordenadesTriangle[0], coordenadesTriangle[1] + estiraTriangle, 0.0);
	glVertex3f(coordenadesTriangle[2], coordenadesTriangle[3] + estiraTriangle, 0.0);
	glEnd();
	glEndList();


	baseNoria = glGenLists(1);
	glNewList(baseNoria, GL_COMPILE);
	//Dibuixem la part del triangle
	glPushMatrix();
	glScalef(0.4, 0.4, 1);
	glTranslatef(0, -radiExterior - estiraTriangle, 0); //Ho baixem perquè el seu pic siga coincident amb l'eix Z
	glCallList(triangle);
	glPopMatrix();
	//Dibuixem la subjecció
	glPushMatrix();
	glRotatef(-20, 1, 0, 0); //Li posem la inclinació
	glScalef(0.4 * (radiExterior - radiInterior), 1.3, 0);//mateixa mida que el triangle, l'amplària és la mateixa que la dels triangles 
														   //(ext - int) però *0.4 perquè siga la mateixa escala, i 1 perquè ha de ser més llarg per la inclinació
	glTranslatef(0, -0.5, 0); //Novament el baixem perquè coincidisca amb l'eix Z
	glutSolidCube(1);
	glPopMatrix();
	glEndList();

	estructuraNoria = glGenLists(1);
	glNewList(estructuraNoria, GL_COMPILE);
	//Dibuixem dos bases, separadas separacioBases, per exemple en +/- separacioBases/2
	glPushMatrix();
	glTranslatef(0, 0, separacioBases / 2);
	glCallList(baseNoria);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -separacioBases / 2);
	glRotatef(180, 0, 1, 0); //En este cas necessitem pegar-li la volta per tal que la subjecció estiga cap a fora, si no esta cap a dins
	glCallList(baseNoria);
	glPopMatrix();
	glEndList();

	soportVago = glGenLists(1);
	glNewList(soportVago, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0, radiNoria, -separacioRodes / 2); //Ho desplacem en Z perquè si no té tota la longitud en +Z i volem que estiga meitat i meitat
	glutSolidCylinder(0.05, separacioRodes, 20, 20); //La distància ha de ser la mateixa que entre les dos rodes
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	glutWireCylinder(0.051, separacioRodes, 20, 20);
	glPopAttrib();
	glPopMatrix();
	glEndList();
}


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

void dibuixarRodesAmbEixos() {
	//Dibuixem la roda situada en +Z
	glPushMatrix();
	glTranslatef(0, 0, 0.3 * separacioBases); //ho arrimem a la base però que se quede abans d'ella
	glCallList(rodaNoria);
	glPopMatrix();

	//Dibuixem la roda situada en -Z
	glPushMatrix();
	glTranslatef(0, 0, -0.3 * separacioBases); //ho arrimem a la base però que se quede abans d'ella
	glCallList(rodaNoria);
	glPopMatrix();

	//Ara li dibuixem a una i altra roda els travesaños
	for (auto i = 0; i < 15; i++) {
		glPushMatrix();
		glTranslatef(0, alturaNoria * 0.4, -0.3 * separacioBases); //ho arrimem a la base però que se quede abans d'ella
		glRotatef(-i * 360 / 15.0, 0,0, 1);
		glCallList(eixos);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, alturaNoria * 0.4, 0.3 * separacioBases); //ho arrimem a la base però que se quede abans d'ella
		glRotatef(-i * 360 / 15.0, 0,0, 1);
		glCallList(eixos);
		glPopMatrix();
	}
}

void dibuixarEixCentral() {
	//I dibuixem també un cilindre que serà al voltant del qual gire la nòria
	glPushMatrix();
	glScalef(1, 1, 1.1); //ho escalem després perquè se repartisca equitativament
	glTranslatef(0, alturaNoria * 0.4, -separacioBases / 2); //com està centrat ho apartem perquè quede cada meitat en un semi-eix de Z
	glRotatef(-angleGirRoda, 0, 0, 1); //apliquem el gir addient
	glutSolidCylinder((radiExterior - radiInterior) / 3, separacioBases, 20, 20);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0, 0, 0);
	glutWireCylinder((radiExterior - radiInterior) / 3 + 0.01, separacioBases, 20, 20);
	glPopAttrib();
	glPopMatrix();
}

void dibuixarSoportVagons(int n) {
	float posX = radiNoria * sin(angleGirRoda);
	float posY = radiNoria * cos(angleGirRoda);
	for (auto i = 0; i < n; i++) {
		glPushMatrix();
		glRotatef(i * 360 / float(n), 0, 0, 1); //Ho repartim al llarg de la circumferència
		//glTranslatef(posX, posY, 0); //En este cas traslladem abans i després rotem, perquè si rotem i després traslladem és com si no haguerem rotat
		glRotatef(-angleGirRoda, 0, 0, 1); //AÇO NO HO GASTEM PERQUE DEMANEN UN MOVIMENT TRASLACIONAL, aleshores ho fem mitjançant translate
		glCallList(soportVago);
		glPopMatrix();
	}
}

void dibuixarVagons(int n) {
	for (auto i = 0; i < n; i++) {
		float posX = radiNoria * cos(-angleGirRoda);
		float posY = radiNoria * sin(-angleGirRoda);
		glPushMatrix();
		glRotatef(i * 360 / float(n), 0, 0, 1); //Ho repartim al llarg de la circumferència
		//glTranslatef(posX, posY, 0); //En este cas traslladem abans i després rotem, perquè si rotem i després traslladem és com si no haguerem rotat
		glRotatef(-angleGirRoda, 0, 0, 1);//AÇO NO HO GASTEM PERQUE DEMANEN UN MOVIMENT TRASLACIONAL, aleshores ho fem mitjançant translate
		glCallList(vagon);
		glPopMatrix();
	}
}

void inicialitzacionsDisplay() {

	//Preparem la rodaNoria, ha de ser ací dins perquè cada vegada s'ha de gira
	rodaNoria = glGenLists(1);
	glNewList(rodaNoria, GL_COMPILE);
	glPushMatrix();
	glScalef(radiNoria / 3, radiNoria / 3, 1); //asignem arbitrariament les mides, usem radiNoria però dividit per 3 perquè estem escalant i ja té 3 en la creació 
	glTranslatef(0, alturaNoria, 0); //ho pugem un poc
	glRotatef(-angleGirRoda, 0, 0, 1); //efectuem el gir 
	glutWireTorus(0.1, 3, 20, 20);
	glPopMatrix();
	glEndList();

	eixos = glGenLists(1);
	glNewList(eixos, GL_COMPILE);
	glPushMatrix();
	//glTranslatef(0, alturaNoria, 0); //ho pugem un poc, perquè estiga a la mateixa alçada que la noria
	glRotatef(-angleGirRoda, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.01, radiNoria, 20, 20);
	glutWireCylinder(0.011, radiNoria, 20, 20); //Es tan xicotet que no se nota que està
	glPopMatrix();
	glEndList();

	vagon = glGenLists(1);
	glNewList(vagon, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0, radiNoria-0.1, 0);
	glScalef(0.1, 0.1, 0.1);
	glutSolidTeapot(1);
	glPopMatrix();
	glEndList();
}

void display()
// Funcion de atencion al dibujo
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(posCameraX, posCameraY, posCameraZ, 0,0,0, 0,1,0);
	gluLookAt(radiCamera*cos(angleCamera), 0, -radiCamera * sin(angleCamera), 0, 0, 0, 0, 1, 0);
	ejes();


	//Com cada vegada volem que tinga un gir, però necessitem que eixe gir siga la primera transformació que se faça
	//hem de fer ací la crida al Toroide, no podem fer el callList de les nòries
	inicialitzacionsDisplay();
	glCallList(estructuraNoria);
	dibuixarRodesAmbEixos();
	dibuixarEixCentral();
	dibuixarSoportVagons(nVagons);
	dibuixarVagons(nVagons);

	//Mostrem en el títol els FPS
	FPS();

	//Ús dels dos buffers
	glutSwapBuffers();
}

void reshape(GLint w, GLint h)
// Funcion de atencion al redimensionamiento
{
	//Este codi d'ací ens permetrà que l'esfera s'adapte als màrgens, com en la pràctica 
	glViewport(0, 0, w, h);
	float relacio = float(w) / h;
	float distancia = sqrt(posCameraX * posCameraX + posCameraY * posCameraY + posCameraZ * posCameraZ);
	float fovy = asin(radiNoria / distancia) * 2 * 180 / PI;
	//Seleccionar càmera: definir el volum de la vista
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Perspectiva
	gluPerspective(fovy, relacio, 0.1, 100);
}
//Callback d'atenció a l'event idle
void onIdle() {
	//Sense control del temps (pot funcionar diferent segons la maquina)
	//angleCamera += 0.1;
	//angleGirRoda += 0.1;

	//Amb control del temps -> Controle la velocitat de gir, quantes voltes dona per segon
	static const float vueltaPorSegundoCamera = 0.001;
	static const float vueltaPorSegundoRueda = 0.1;
	static const float vueltaPorSegundoVagon = 0;
	static int sentit = 1;

	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	angleCamera += vueltaPorSegundoCamera * 360 * (ahora - antes) / 1000;
	angleGirRoda += vueltaPorSegundoRueda * 360 * (ahora - antes) / 1000;
	angleGirVago += sentit * vueltaPorSegundoVagon * 360 * (ahora - antes) / 1000; //només en 30º perquè volem que siga un balanceig lleuger, no que pegue tota la volta
	if (angleGirVago > 30 || angleGirVago < -30) { sentit = sentit * -1; }//perquè faça el balanceig en el moment que passe de cert angle ha de tornar
	
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
	//glutIdleFunc(onIdle);
	glutTimerFunc(1000 / tasaFPS, onTimer, 1000 / tasaFPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}