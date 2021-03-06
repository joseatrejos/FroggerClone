#include "stdafx.h"
#include <stdio.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>

#include <windows.h>

#pragma comment(lib, "Winmm.lib")



using namespace std;

// Funciones
void dibujar();
void actualizar();
void dibujarRana();
void dibujarRanaCoop();
void dibujarCarro();
void dibujarTroncos();
void dibujarMapa();
void dibujarNenunfar();
void dibujarTortuga();
void dibujarVidasRana();
void dibujarVidasRanaCoop();
void dibujarAlcantarillas();
void dibujarCocodrilo();
void dibujarMuerteRana();
void dibujarMuerteRanaCoop();
void dibujarTrofeo();
void dibujarTrofeoCoop();
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void moverRanaCoop();
void moverCarrosDerecha();
void moverCarrosIzquierda();
void moverTroncos();
void moverTortugas();
void moverCocodrilos();
void dibujarSerpiente();
void moverSerpiente();
void colisionesVictoria();
void colisionesDerrota();
void colisionesVictoriaCoop();
void colisionesDerrotaCoop();
void restart();

// Variables Globales
int contGolpes = 0;
float xRana = -.9375f;
float yRana = -.9375f;
float xRanaCoop = .9375f;
float yRanaCoop = -.9375f;
float saltoRana = .125f;
float giroRana = 0.0f;
float velocidadCarros = .7f;
float velocidadRana = 1.2f;
float velocidadActual = 0.0f;
float velocidadTroncos = .6f;
float velocidadCocodrilo = .6f;
float velocidadSerpiente = .3f;
float velocidadTortugas = .5f;
float anguloRotacionRanaCoop = 0.0f;
float anguloTrayectoria = 0.0f;
float velocidadAngular = 180.0f;
bool colorVictoriaNenunfar[4] = {false, false, false, false};
bool montarCosasAcuiferas[3] = { true, true, true };
float posicionXCarroIzquierda[6] = {-.575f, 0.5f, 0.375f, .8125f, 0.0f, 1.0f};
float posicionYCarroIzquierda[6] = {-.6875f, -0.4375f, -0.1875f, -.6875f, -0.4375f, -0.1875f};
float posicionXCarroDerecha[6] = {.8125f, 0.0f, -0.1875f, -.5f, -.72f, .25f};
float posicionYCarroDerecha[6] = {-.8125f, -.5625f, -0.3125f, -.8125f, -.5625f, -0.3125f};
float posicionXNenunfares[4] = {-0.7475f, -0.25f, 0.2475f, 0.7475f};
float posicionYNenunfares = .8125f;
float posicionXTronco[9] = { 0.0f, -.875f,-.875f,0.25f, 1.0f, -0.75f,0.0f, .125f,-0.8f };
float posicionYTronco[9] = { 0.1875f,0.1875f,0.1875f, 0.3125f,0.3125f,0.3125f,.5625f,.5625f,.5625f };
float posicionXTortugas[8] = { 0.375f, 0.875f,-.375f,-.875f,.25f, 1.0f,-.25f,-1.0f };
float posicionYTortugas[8] = { 0.0625f,0.0625f,0.0625f,0.0625f,.6875f,.6875f ,.6875f ,.6875f };
float posicionXCocodrilo[2] = { .5625f,-.875f };
float posicionYCocodrilo[2] = { .43755f,.43755f };
float posicionXAlcantarillas[2] = { -.625f, 0.125f };
float posicionYAlcantarillas[2] = { -.25f, 0.0};
float posicionXSerpiente = 0.0f;
float posicionYSerpiente = -0.0625f;
double tiempoAnterior = 0.0f;
double tiempoActual = 0.0f;
double tiempoDiferencial = 0.0f;
double tiempoAnteriorColision = 0.0f;
double tiempoDiferencialColision = 0.0f;
int contadorVidasRana = 3;
int contadorVidasRanaCoop = 3;
float rAgua = 0.2f;
float gAgua = 0.6f;
float bAgua = 0.8f;

enum Direccion { Izquierda, Derecha };
Direccion direccionIzquierda = Direccion::Izquierda;
Direccion direccionDerecha = Direccion::Derecha;

// Declaramos apuntador de ventana
GLFWwindow *window;

int main()
{
	
	LPCWSTR archivo = L"C:\\Frogger_Song.wav";

	PlaySound(archivo, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	// Si no se pudo iniciar glfw, terminamos la ejecución 
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Si se pudo inciar GLFW, entonces inicializamos la ventana

	window = glfwCreateWindow(1024, 768, "Ventana", NULL, NULL);

	// Si no podemos iniciar la ventana, terminamos la ejecución
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Se establece el contexto
	glfwMakeContextCurrent(window);

	// Una vez establecido el contexto, activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();

	if (errorGlew != GLEW_OK)
	{
		cout << glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL;

	glfwSetKeyCallback(window, teclado_callback);
	tiempoAnterior = glfwGetTime();

	// Ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, 1024, 768);

		//Establecer el color de orrado
		glClearColor(0.5, 0.4, 1, 1);

		//Borramos
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();

		//Cambiar los buffers
		glfwSwapBuffers(window);

		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void dibujar()
{
	dibujarMapa();
	dibujarCarro();
	dibujarTroncos();
	dibujarNenunfar();
	dibujarTortuga();
	dibujarSerpiente();
	dibujarCocodrilo();
	dibujarRana();
	dibujarRanaCoop();
	dibujarVidasRana();
	dibujarVidasRanaCoop();
}

void actualizar()
{
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;

	colisionesVictoria();
	colisionesDerrota();
	colisionesVictoriaCoop();
	colisionesDerrotaCoop();
	
	moverRanaCoop();
	moverCarrosDerecha();
	moverCarrosIzquierda();
	moverTroncos();
	moverTortugas();
	moverCocodrilos();
	moverSerpiente();

	tiempoAnterior = tiempoActual;

	if (xRana > 0.9375) {
		xRana = 0.9375;
	}

	if (xRana < -0.9375) {
		xRana = -0.9375;
	}

	if (yRana > 0.9375) {
		yRana = 0.9375;
	}

	if (yRana < -0.9375) {
		yRana = -0.9375;
	}
}

void dibujarRana()
{
	if (contadorVidasRana > 0)
	{
		glPushMatrix();
			glTranslatef(xRana, yRana, 0.0f);
			glRotatef(giroRana, 0.0f, 0.0f, 1.0f);

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.03125f, -.03125f, 0.0f);
				glVertex3f(-.03125f, -.03125f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
			glEnd();

			// Patas Y bRAzos
			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.045f, 0.0f, 0.0f);
				glVertex3f(.045f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.0625f, -0.04f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, -0.04f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.045f, 0.0f, 0.0f);
				glVertex3f(-.045f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.0625f, -0.04f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -0.04f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.02f, .03128f, 0.0f);
				glVertex3f(.02f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.02f, .03128f, 0.0f);
				glVertex3f(-.02f, .0625f, 0.0f);
			glEnd();

			//Ojitos
			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.02f, .02f, 0.0f);
				glVertex3f(.02f, .0f, 0.0f);
				glVertex3f(.01f, .0f, 0.0f);
				glVertex3f(.01f, .02f, 0.0f);
			glEnd();
			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.02f, .02f, 0.0f);
				glVertex3f(-.02f, .0f, 0.0f);
				glVertex3f(-.01f, .0f, 0.0f);
				glVertex3f(-.01f, .02f, 0.0f);
			glEnd();

		glPopMatrix();
	}
}

void dibujarRanaCoop() 
{
	if (contadorVidasRanaCoop > 0)
	{
		glPushMatrix();
			glTranslatef(xRanaCoop, yRanaCoop, 0.0f);
			glRotatef(anguloRotacionRanaCoop, 0.0f, 0.0f, 1.0f);

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.03125f, -.03125f, 0.0f);
				glVertex3f(-.03125f, -.03125f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
			glEnd();

			//patas Y bRAzos
			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.045f, 0.0f, 0.0f);
				glVertex3f(.045f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.0625f, -0.04f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, -0.04f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.045f, 0.0f, 0.0f);
				glVertex3f(-.045f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.0625f, -0.04f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -0.04f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.02f, .03128f, 0.0f);
				glVertex3f(.02f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.02f, .03128f, 0.0f);
				glVertex3f(-.02f, .0625f, 0.0f);
			glEnd();

			//Ojitos
			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.02f, .02f, 0.0f);
				glVertex3f(.02f, .0f, 0.0f);
				glVertex3f(.01f, .0f, 0.0f);
				glVertex3f(.01f, .02f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.02f, .02f, 0.0f);
				glVertex3f(-.02f, .0f, 0.0f);
				glVertex3f(-.01f, .0f, 0.0f);
				glVertex3f(-.01f, .02f, 0.0f);
			glEnd();
		glPopMatrix();
	}
}

void dibujarCarro()
{
	// (Derecha) Carro Rosa 1er Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha[0], posicionYCarroDerecha[0], 0.0f);
		
		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.45f, 0.796f);
			glVertex3f(-.0625f, -.0475f, 0.0f);
			glVertex3f(.0625f, -.0475f, 0.0f);
			glVertex3f(.0625f, .0475f, 0.0f);
			glVertex3f(-.0625f, .0475f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.7f, 0.8f);
			glVertex3f(.025f, -.0475f, 0.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.025f, .0475f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0475f, 0.0f);
			glVertex3f(-.05f, -.0475f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0625f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0625f, 0.0f);
			glVertex3f(-.05f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.025f, .0475f, 0.0f);
			glVertex3f(.025f, .0625f, 0.0f);
			glVertex3f(.05f, .0625f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Derecha) 2do Carro 1er Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha[3], posicionYCarroDerecha[3], 0.0f);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.2f, 0.5f);
			glVertex3f(-.0625f, -.0475f, 0.0f);
			glVertex3f(.0625f, -.0475f, 0.0f);
			glVertex3f(.0625f, .0475f, 0.0f);
			glVertex3f(-.0625f, .0475f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.7f, 0.8f);
			glVertex3f(.025f, -.0475f, 0.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.025f, .0475f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0475f, 0.0f);
			glVertex3f(-.05f, -.0475f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0625f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0625f, 0.0f);
			glVertex3f(-.05f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.025f, .0475f, 0.0f);
			glVertex3f(.025f, .0625f, 0.0f);
			glVertex3f(.05f, .0625f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Derecha) 2do Camión 5to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha[5], posicionYCarroDerecha[5], 0.0f);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.6f, 0.35f, 0.8f);
			glVertex3f(-.125f, -.0475f, 0.0f);
			glVertex3f(.075f, -.0475f, 0.0f);
			glVertex3f(.075f, .0475f, 0.0f);
			glVertex3f(-.125f, .0475f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(0.6f, 0.35f, 0.8f);
			glVertex3f(.125f, -.035f, 0.0f);
			glVertex3f(.075f, -.035f, 0.0f);
			glVertex3f(.075f, .035f, 0.0f);
			glVertex3f(.125f, .035f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.2f, 0.8f);
			glVertex3f(.1125f, -.035f, 0.0f);
			glVertex3f(.0875f, -.035f, 0.0f);
			glVertex3f(.0875f, .035f, 0.0f);
			glVertex3f(.1125f, .035f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0475f, 0.0f);
			glVertex3f(-.1125f, -.0475f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.0125f, -.0475f, 0.0f);
			glVertex3f(.0125f, -.0625f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, .0475f, 0.0f);
			glVertex3f(-.05f, .0475f, 0.0f);
			glVertex3f(-.05, .0625f, 0.0f);
			glVertex3f(-.1125f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.0125f, .0475f, 0.0f);
			glVertex3f(.0125f, .0625f, 0.0f);
			glVertex3f(.05f, .0625f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Derecha) Camión Azul Claro 3er Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha[1], posicionYCarroDerecha[1], 0.0f);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.7f, 0.8f);
			glVertex3f(-.125f, -.0475f, 0.0f);
			glVertex3f(.075f, -.0475f, 0.0f);
			glVertex3f(.075f, .0475f, 0.0f);
			glVertex3f(-.125f, .0475f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.7f, 0.8f);
			glVertex3f(.125f, -.035f, 0.0f);
			glVertex3f(.075f, -.035f, 0.0f);
			glVertex3f(.075f, .035f, 0.0f);
			glVertex3f(.125f, .035f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.2f, 0.8f);
			glVertex3f(.1125f, -.035f, 0.0f);
			glVertex3f(.0875f, -.035f, 0.0f);
			glVertex3f(.0875f, .035f, 0.0f);
			glVertex3f(.1125f, .035f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0475f, 0.0f);
			glVertex3f(-.1125f, -.0475f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.0125f, -.0475f, 0.0f);
			glVertex3f(.0125f, -.0625f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, .0475f, 0.0f);
			glVertex3f(-.05f, .0475f, 0.0f);
			glVertex3f(-.05, .0625f, 0.0f);
			glVertex3f(-.1125f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.0125f, .0475f, 0.0f);
			glVertex3f(.0125f, .0625f, 0.0f);
			glVertex3f(.05f, .0625f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Derecha) Tractor Amarillo 5to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha[2], posicionYCarroDerecha[2], 0.0f);

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0375f, 0.0f);
			glVertex3f(-.05f, -.0375f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0375f, -.035f, 0.0f);
			glVertex3f(.0125f, -.035f, 0.0f);
			glVertex3f(.0125f, -.0625f, 0.0f);
			glVertex3f(.0375f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, .0375f, 0.0f);
			glVertex3f(-.0125f, .0375f, 0.0f);
			glVertex3f(-.0125f, .0625f, 0.0f);
			glVertex3f(-.05f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0375f, .035f, 0.0f);
			glVertex3f(.0125f, .035f, 0.0f);
			glVertex3f(.0125f, .0625f, 0.0f);
			glVertex3f(.0375f, .0625f, 0.0f);
		glEnd();

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.7f, 0.2f);
			glVertex3f(-.0625f, -.0375f, 0.0f);
			glVertex3f(.0375f, -.0375f, 0.0f);
			glVertex3f(.0375f, .0375f, 0.0f);
			glVertex3f(-.0625f, .0375f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.7f, 0.2f);
			glVertex3f(.0625f, -.025f, 0.0f);
			glVertex3f(.0375f, -.025f, 0.0f);
			glVertex3f(.0375f, .025f, 0.0f);
			glVertex3f(.0625f, .025f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.45f, 0.796f);
			glVertex3f(.05625f, -.025f, 0.0f);
			glVertex3f(.04375f, -.025f, 0.0f);
			glVertex3f(.04375f, .025f, 0.0f);
			glVertex3f(.05625f, .025f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Derecha) 2do Tractor 3er Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha[4], posicionYCarroDerecha[4], 0.0f);

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0375f, 0.0f);
			glVertex3f(-.05f, -.0375f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0375f, -.035f, 0.0f);
			glVertex3f(.0125f, -.035f, 0.0f);
			glVertex3f(.0125f, -.0625f, 0.0f);
			glVertex3f(.0375f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, .0375f, 0.0f);
			glVertex3f(-.0125f, .0375f, 0.0f);
			glVertex3f(-.0125f, .0625f, 0.0f);
			glVertex3f(-.05f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0375f, .035f, 0.0f);
			glVertex3f(.0125f, .035f, 0.0f);
			glVertex3f(.0125f, .0625f, 0.0f);
			glVertex3f(.0375f, .0625f, 0.0f);
		glEnd();

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.7f, 0.2f);
			glVertex3f(-.0625f, -.0375f, 0.0f);
			glVertex3f(.0375f, -.0375f, 0.0f);
			glVertex3f(.0375f, .0375f, 0.0f);
			glVertex3f(-.0625f, .0375f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.7f, 0.2f);
			glVertex3f(.0625f, -.025f, 0.0f);
			glVertex3f(.0375f, -.025f, 0.0f);
			glVertex3f(.0375f, .025f, 0.0f);
			glVertex3f(.0625f, .025f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.45f, 0.796f);
			glVertex3f(.05625f, -.025f, 0.0f);
			glVertex3f(.04375f, -.025f, 0.0f);
			glVertex3f(.04375f, .025f, 0.0f);
			glVertex3f(.05625f, .025f, 0.0f);
		glEnd();
	glPopMatrix();
	
	// (Izquierda) Carro Morado 2do Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[0], posicionYCarroIzquierda[0], 0.0f);
		glRotatef(180, 0, 0, 1);

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.01875, -.0625f, 0.0f);
			glVertex3f(-.01875, -.0375f, 0.0f);
			glVertex3f(-.05f, -.0375f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.025f, -.035f, 0.0f);
			glVertex3f(.00625f, -.035f, 0.0f);
			glVertex3f(.00625f, -.05625f, 0.0f);
			glVertex3f(.025f, -.05625f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, .0375f, 0.0f);
			glVertex3f(-.01875, .0375f, 0.0f);
			glVertex3f(-.01875, .0625f, 0.0f);
			glVertex3f(-.05f, .0625f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.025f, .035f, 0.0f);
			glVertex3f(.00625f, .035f, 0.0f);
			glVertex3f(.00625f, .05625f, 0.0f);
			glVertex3f(.025f, .05625f, 0.0f);
		glEnd();

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.5f, 0.1f, 0.4f);
			glVertex3f(-.0625f, -.0375f, 0.0f);
			glVertex3f(.0375f, -.0375f, 0.0f);
			glVertex3f(.0375f, .0375f, 0.0f);
			glVertex3f(-.0625f, .0375f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(0.5f, 0.1f, 0.4f);
			glVertex3f(.0625f, -.025f, 0.0f);
			glVertex3f(.0375f, -.025f, 0.0f);
			glVertex3f(.0375f, .025f, 0.0f);
			glVertex3f(.0625f, .025f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.7f, 0.8f);
			glVertex3f(.05625f, -.025f, 0.0f);
			glVertex3f(.04375f, -.025f, 0.0f);
			glVertex3f(.04375f, .025f, 0.0f);
			glVertex3f(.05625f, .025f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Izquierda) 2do Carro 2do Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[5], posicionYCarroIzquierda[5], 0.0f);
		glRotatef(180, 0, 0, 1);

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.01875, -.0625f, 0.0f);
			glVertex3f(-.01875, -.0375f, 0.0f);
			glVertex3f(-.05f, -.0375f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.025f, -.035f, 0.0f);
			glVertex3f(.00625f, -.035f, 0.0f);
			glVertex3f(.00625f, -.05625f, 0.0f);
			glVertex3f(.025f, -.05625f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.05f, .0375f, 0.0f);
			glVertex3f(-.01875, .0375f, 0.0f);
			glVertex3f(-.01875, .0625f, 0.0f);
			glVertex3f(-.05f, .0625f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.025f, .035f, 0.0f);
			glVertex3f(.00625f, .035f, 0.0f);
			glVertex3f(.00625f, .05625f, 0.0f);
			glVertex3f(.025f, .05625f, 0.0f);
		glEnd();

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.7f, 0.3f, 0.4f);
			glVertex3f(-.0625f, -.0375f, 0.0f);
			glVertex3f(.0375f, -.0375f, 0.0f);
			glVertex3f(.0375f, .0375f, 0.0f);
			glVertex3f(-.0625f, .0375f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(0.7f, 0.3f, 0.4f);
			glVertex3f(.0625f, -.025f, 0.0f);
			glVertex3f(.0375f, -.025f, 0.0f);
			glVertex3f(.0375f, .025f, 0.0f);
			glVertex3f(.0625f, .025f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.7f, 0.8f);
			glVertex3f(.05625f, -.025f, 0.0f);
			glVertex3f(.04375f, -.025f, 0.0f);
			glVertex3f(.04375f, .025f, 0.0f);
			glVertex3f(.05625f, .025f, 0.0f);
		glEnd();
	glPopMatrix();
	
	// (Izquierda) Carro Azul Fuerte 4to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[1], posicionYCarroIzquierda[1], 0.0f);
		glRotatef(180, 0.0f, 0.0f, 1.0f);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.0f, 0.2f, 0.4f);
			glVertex3f(-.0625f, -.0475f, 0.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(-.0625f, .0475f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0.0f, 0.2f, 0.4f);
			glVertex3f(.0625f, -.03f, 0.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.0625f, .03f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.6f, 0.8f);
			glVertex3f(.025f, -.04f, 0.0f);
			glVertex3f(.05f, -.04f, 0.0f);
			glVertex3f(.05f, .04f, 0.0f);
			glVertex3f(.025f, .04f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.04f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0475f, 0.0f);
			glVertex3f(-.04f, -.0475f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0625f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.04f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0625f, 0.0f);
			glVertex3f(-.04f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.025f, .0475f, 0.0f);
			glVertex3f(.025f, .0625f, 0.0f);
			glVertex3f(.05f, .0625f, 0.0f);
		glEnd();
	glPopMatrix();


	// (Izquierda) 2do Camion 4to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[4], posicionYCarroIzquierda[4], 0.0f);
		glRotatef(180, 0, 0, 1);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.9f, 0.05f, 0.25f);
			glVertex3f(-.125f, -.0475f, 0.0f);
			glVertex3f(.075f, -.0475f, 0.0f);
			glVertex3f(.075f, .0475f, 0.0f);
			glVertex3f(-.125f, .0475f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(0.9f, 0.05f, 0.25f);
			glVertex3f(.125f, -.0375f, 0.0f);
			glVertex3f(.075f, -.0375f, 0.0f);
			glVertex3f(.075f, .0375f, 0.0f);
			glVertex3f(.125f, .0375f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.2f, 0.8f);
			glVertex3f(.1125f, -.0375f, 0.0f);
			glVertex3f(.0875f, -.0375f, 0.0f);
			glVertex3f(.0875f, .0375f, 0.0f);
			glVertex3f(.1125f, .0375f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0475f, 0.0f);
			glVertex3f(-.1125f, -.0475f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0625, -.0475f, 0.0f);
			glVertex3f(.01875, -.0475f, 0.0f);
			glVertex3f(.01875, -.0625f, 0.0f);
			glVertex3f(.0625, -.0625f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, .0475f, 0.0f);
			glVertex3f(-.05f, .0475f, 0.0f);
			glVertex3f(-.05, .0625f, 0.0f);
			glVertex3f(-.1125f, .0625f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0625, .0475f, 0.0f);
			glVertex3f(.01875, .0475f, 0.0f);
			glVertex3f(.01875, .0625f, 0.0f);
			glVertex3f(.0625, .0625f, 0.0f);
		glEnd();
	glPopMatrix();

	// Camion Rojo 6to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[3], posicionYCarroIzquierda[3], 0.0f);
		glRotatef(180, 0, 0, 1);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.8f, 0.0f, 0.1f);
			glVertex3f(-.125f, -.0475f, 0.0f);
			glVertex3f(.075f, -.0475f, 0.0f);
			glVertex3f(.075f, .0475f, 0.0f);
			glVertex3f(-.125f, .0475f, 0.0f);
		glEnd();

		// Cara
		glBegin(GL_QUADS);
			glColor3f(0.8f, 0.0f, 0.1f);
			glVertex3f(.125f, -.0375f, 0.0f);
			glVertex3f(.075f, -.0375f, 0.0f);
			glVertex3f(.075f, .0375f, 0.0f);
			glVertex3f(.125f, .0375f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.2f, 0.8f);
			glVertex3f(.1125f, -.0375f, 0.0f);
			glVertex3f(.0875f, -.0375f, 0.0f);
			glVertex3f(.0875f, .0375f, 0.0f);
			glVertex3f(.1125f, .0375f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.05f, -.0475f, 0.0f);
			glVertex3f(-.1125f, -.0475f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0625, -.0475f, 0.0f);
			glVertex3f(.01875, -.0475f, 0.0f);
			glVertex3f(.01875, -.0625f, 0.0f);
			glVertex3f(.0625, -.0625f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.1125f, .0475f, 0.0f);
			glVertex3f(-.05f, .0475f, 0.0f);
			glVertex3f(-.05, .0625f, 0.0f);
			glVertex3f(-.1125f, .0625f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.0625, .0475f, 0.0f);
			glVertex3f(.01875, .0475f, 0.0f);
			glVertex3f(.01875, .0625f, 0.0f);
			glVertex3f(.0625, .0625f, 0.0f);
		glEnd();
	glPopMatrix();

	// (Izquierda) 2do Carro 6to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[2], posicionYCarroIzquierda[2], 0.0f);
		glRotatef(180, 0.0f, 0.0f, 1.0f);

		// Cuerpo
		glBegin(GL_QUADS);
			glColor3f(0.0f, 0.2f, 0.4f);
			glVertex3f(-.0625f, -.0475f, 0.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(-.0625f, .0475f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0.0f, 0.2f, 0.4f);
			glVertex3f(.0625f, -.03f, 0.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.0625f, .03f, 0.0f);
		glEnd();

		// Ventana
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.6f, 0.8f);
			glVertex3f(.025f, -.04f, 0.0f);
			glVertex3f(.05f, -.04f, 0.0f);
			glVertex3f(.05f, .04f, 0.0f);
			glVertex3f(.025f, .04f, 0.0f);
		glEnd();

		// Llanta Abajo Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.04f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0625f, 0.0f);
			glVertex3f(-.0125f, -.0475f, 0.0f);
			glVertex3f(-.04f, -.0475f, 0.0f);
		glEnd();

		// Llanta Abajo Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0475f, 0.0f);
			glVertex3f(.025f, -.0625f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
		glEnd();

		// Llanta Arriba Izquierda
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-.04f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0475f, 0.0f);
			glVertex3f(-.0125f, .0625f, 0.0f);
			glVertex3f(-.04f, .0625f, 0.0f);
		glEnd();

		// Llanta Arriba Derecha
		glBegin(GL_QUADS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(.05f, .0475f, 0.0f);
			glVertex3f(.025f, .0475f, 0.0f);
			glVertex3f(.025f, .0625f, 0.0f);
			glVertex3f(.05f, .0625f, 0.0f);
		glEnd();
	glPopMatrix();
}

void dibujarTroncos()
{
	//Tronco Segundo Carril
	// Inicio de la Matriz
	for (int x = 0; x < 9; x++)
	{
		glPushMatrix();
			glTranslatef(posicionXTronco[x], posicionYTronco[x], 0.0f);
			glBegin(GL_QUADS);
				glColor3f(.50f, .25f, .06f);
				glVertex3f(.1875, .0625f, 0.0f);
				glVertex3f(.1875, -.0625f, 0.0f);
				glVertex3f(-.1875, -.0625f, 0.0f);
				glVertex3f(-.1875, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0.737f, 0.5607f, 0.5607f);
				glVertex3f(.12, -.03f, 0.0f);
				glVertex3f(.12, -.04f, 0.0f);
				glVertex3f(-.12, -.04f, 0.0f);
				glVertex3f(-.12, -.03f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0.737f, 0.5607f, 0.5607f);
				glVertex3f(.12, -.03f, 0.0f);
				glVertex3f(.12, -.04f, 0.0f);
				glVertex3f(-.12, -.04f, 0.0f);
				glVertex3f(-.12, -.03f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0.737f, 0.5607f, 0.5607f);
				glVertex3f(.125, -.02f, 0.0f);
				glVertex3f(.125, -.01f, 0.0f);
				glVertex3f(0.0, -.01f, 0.0f);
				glVertex3f(0.0, -.02f, 0.0f);
			glEnd();

			glBegin(GL_POLYGON);
				glColor3f(0.737f, 0.5607f, 0.5607f);
				for (int i = 0; i < 360; i++)
				{
					glVertex3f(.04*cos((double)i*(3.1416 / 180)) + .18, .0625*sin((double)i*(3.14159 / 180.0)), 0.4);
				}
			glEnd();

			glBegin(GL_POLYGON);
				glColor3f(.50f, .25f, .06f);
				for (int i = 0; i < 360; i++)
				{
					glVertex3f(.04*cos((double)i*(3.1416 / 180)) - .18, .0625*sin((double)i*(3.14159 / 180.0)), 0.4);
				}
			glEnd();
		glPopMatrix();
	}
}

void dibujarMapa()
{
	// Banqueta
	glBegin(GL_QUADS);
		glColor3f(.8f, .8f, .8f);
		glVertex3f(-1.0f, -.875f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0, 0.0f);
		glVertex3f(1.0f, -.875f, 0.0f);
	glEnd();

	// Banqueta Central
	glBegin(GL_QUADS);
		glColor3f(.8f, .8f, .8f);
		glVertex3f(-1.0f, -.125, 0.0f);
		glVertex3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -.125, 0.0f);
	glEnd();
	
	// Líneas Banqueta de Abajo
	for (double x = -1; x < 1; x += .125)
	{
		glBegin(GL_LINES);
			glColor3f(.5078f, .5078f, .5078f);
			glVertex3f(x, -1.0f, 0.0f);
			glVertex3f(x, -.875f, 0.0f);
		glEnd();
	}
	
	// Líneas Banqueta Central
	for (double x = -1; x < 1; x += .125)
	{
		glBegin(GL_LINES);
			glColor3f(.5078f, .5078f, .5078f);
			glVertex3f(x, -.125f, 0.0f);
			glVertex3f(x, 0.0f, 0.0f);
		glEnd();
	}

	// Carriles
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -.875f, 0.0f);
		glVertex3f(-1.0f, -.125f, 0.0f);
		glVertex3f(1.0f, -.125f, 0.0f);
		glVertex3f(1.0f, -.875f, 0.0f);
	glEnd();

	// Líneas de los Carriles
	glPushAttrib(GL_ENABLE_BIT);
		glLineStipple(2, 0x00FF);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINES);
			for (float c = -.75; c < -.125; c += .125)
			{
				glColor3f(1.0f, 0.7f, 0.2f);
				glVertex3f(-1, c, -.5);
				glVertex3f(1, c, -.5);
			}
		glEnd();
	glPopAttrib();

	// Río
	glBegin(GL_QUADS);
		glColor3f(rAgua, gAgua, bAgua);
		glVertex3f(-1.0f, -0.0f, 0.0f);
		glVertex3f(-1.0f, .75, 0.0f);
		glVertex3f(1.0f, .75, 0.0f);
		glVertex3f(1.0f, -0.0f, 0.0f);
	glEnd();

	// Cesped
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.85f, 0.4f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, .75, 0.0f);
		glVertex3f(1.0f, .75, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();

	// Agua victoria  4
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(.8725f, .75f, 0.0f);
		glVertex3f(.6225f, .75f, 0.0f);
		glVertex3f(.6225f, .875f, 0.0f);
		glVertex3f(.8725f, .875f, 0.0f);
	glEnd();

	// Agua victoria  3
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(.3725f, .75f, 0.0f);
		glVertex3f(.1225f, .75f, 0.0f);
		glVertex3f(.1225f, .875f, 0.0f);
		glVertex3f(.3725f, .875f, 0.0f);
	glEnd();

	// Agua victoria 2
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(-.1225f, .75f, 0.0f); 
		glVertex3f(-.3775f, .75f, 0.0f);
		glVertex3f(-.3775f, .875f, 0.0f);
		glVertex3f(-.1225f, .875f, 0.0f);
	glEnd();

	// Agua victoria 1
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(-.8725f, .75f, 0.0f);
		glVertex3f(-.6225f, .75f, 0.0f);
		glVertex3f(-.6225f, .875f, 0.0f);
		glVertex3f(-.8725f, .875f, 0.0f);
	glEnd();

	dibujarAlcantarillas();
}

void dibujarNenunfar()
{
	for (int x=0; x < 4; x++)
	{
		glPushMatrix();
			glTranslatef(posicionXNenunfares[x], posicionYNenunfares, 0.0f);
			glBegin(GL_POLYGON);
				if (colorVictoriaNenunfar[x] == false)
					glColor3f(0.2f, 0.9f, 0.2f);
				else
					glColor3f(1.0f, 0.45f, 0.796f);
				for (int i = 0; i < 360; i++)
				{
					glVertex3f(.06*cos((double)i*(3.1416 / 180)), .06*sin((double)i*(3.14159 / 180.0)), 0.4);
				}
			glEnd();

			glBegin(GL_TRIANGLES);
				glColor3f(0.2f, 0.6f, 0.8f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(.03, -.0625, 0.0f);
				glVertex3f(-.03, -.0625, 0.0f);
			glEnd();
		glPopMatrix();
	}
}

void dibujarTortuga()
{
	for (int x = 0; x < 8; x++)
	{
		// Tortuga 1
		glPushMatrix();
			glTranslatef(posicionXTortugas[x], posicionYTortugas[x], 0.0f);
			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.03f, -.03125f, 0.0f);
				glVertex3f(.03f, -.0625f, 0.0f);
				glVertex3f(.01f, -.0625f, 0.0f);
				glVertex3f(.01f, -.03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(-.03f, -.03125f, 0.0f);
				glVertex3f(-.03f, -.0625f, 0.0f);
				glVertex3f(-.01f, -.0625f, 0.0f);
				glVertex3f(-.01f, -.03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.03f, .03125f, 0.0f);
				glVertex3f(.03f, .0625f, 0.0f);
				glVertex3f(.01f, .0625f, 0.0f);
				glVertex3f(.01f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(-.03f, .03125f, 0.0f);
				glVertex3f(-.03f, .0625f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
				glVertex3f(-.01f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(-.03f, .03125f, 0.0f);
				glVertex3f(-.03f, .0625f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
				glVertex3f(-.01f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_TRIANGLES);
				glColor3f(.2, .9, .2);
				glVertex3f(-.03125f, .025f, 0.0f);
				glVertex3f(-.03125f, -.025f, 0.0f);
				glVertex3f(-.0625f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.03125f, .018f, 0.0f);
				glVertex3f(.03125f, -.018f, 0.0f);
				glVertex3f(.0625f, -.018f, 0.0f);
				glVertex3f(.0625f, .018f, 0.0f);
			glEnd();

			glBegin(GL_POLYGON);
				glColor3f(.54, .27, .074);
				for (int i = 0; i < 360; i++)
				{
					glVertex3f(.046*cos((double)i*(3.1416 / 180)), .05*sin((double)i*(3.14159 / 180.0)), 0.4);
				}
			glEnd();

			//Tortuga2
			/*glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.155f, -.03125f, 0.0f);
				glVertex3f(.155f, -.0625f, 0.0f);
				glVertex3f(.135f, -.0625f, 0.0f);
				glVertex3f(.135f, -.03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.095f, -.03125f, 0.0f);
				glVertex3f(.095f, -.0625f, 0.0f);
				glVertex3f(.115f, -.0625f, 0.0f);
				glVertex3f(.115f, -.03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.155f, .03125f, 0.0f);
				glVertex3f(.155f, .0625f, 0.0f);
				glVertex3f(.135f, .0625f, 0.0f);
				glVertex3f(.135f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.095f, .03125f, 0.0f);
				glVertex3f(.095f, .0625f, 0.0f);
				glVertex3f(.115f, .0625f, 0.0f);
				glVertex3f(.115f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.095f, .03125f, 0.0f);
				glVertex3f(.095f, .0625f, 0.0f);
				glVertex3f(.115f, .0625f, 0.0f);
				glVertex3f(.115f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_TRIANGLES);
				glColor3f(.2, .9, .2);
				glVertex3f(.09375f, .025f, 0.0f);
				glVertex3f(.09375f, -.025f, 0.0f);
				glVertex3f(.0625f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(.2, .9, .2);
				glVertex3f(.15625f, .018f, 0.0f);
				glVertex3f(.15625f, -.018f, 0.0f);
				glVertex3f(.1875f, -.018f, 0.0f);
				glVertex3f(.1875f, .018f, 0.0f);
			glEnd();

			glBegin(GL_POLYGON);
				glColor3f(.54, .27, .074);
				for (int i = 0; i < 360; i++)
				{
					glVertex3f(.046*cos((double)i*(3.1416 / 180)) + .125, .05*sin((double)i*(3.14159 / 180.0)), 0.4);
				}
				glEnd();*/
		glPopMatrix();
	}
}

void dibujarAlcantarillas()
{
	// Circulo Exterior 1
	glPushMatrix();
		glTranslatef(posicionXAlcantarillas[0], posicionYAlcantarillas[0], 0.0f);
		glBegin(GL_POLYGON);
			glColor3f(0.2f, 0.2f, 0.2f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.06*cos((double)i*(3.1416 / 180)) + .0625, .06*sin((double)i*(3.14159 / 180.0)) - .0625, 0.4);
			}
		glEnd();
	glPopMatrix();

	// Círculo Interior 1
	glPushMatrix();
		glTranslatef(-0.625f, -.25, 0.0f);
		glScalef(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glColor3f(0.4f, 0.4f, 0.4f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.06*cos((double)i*(3.1416 / 180)) + .078, .06*sin((double)i*(3.14159 / 180.0)) - .078, 0.4);
			}
		glEnd();
	glPopMatrix();

	// Handle 1
	glPushMatrix();
		glTranslatef(-0.54f, -.3185, 0.0f);
		glScalef(0.155f, 0.15f, 0.15f);
		glBegin(GL_POLYGON);
			glColor3f(0.2f, 0.2f, 0.2f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.06*cos((double)i*(3.1416 / 180)) + .078, .06*sin((double)i*(3.14159 / 180.0)) - .078, 0.4);
			}
		glEnd();
	glPopMatrix();

	// Circulo Exterior 2
	glPushMatrix();
		glTranslatef(posicionXAlcantarillas[1], posicionYAlcantarillas[1], 0.0f);
		glBegin(GL_POLYGON);
			glColor3f(0.2f, 0.2f, 0.2f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.06*cos((double)i*(3.1416 / 180)) + .0625, .06*sin((double)i*(3.14159 / 180.0)) - .0625, 0.4);
			}
		glEnd();
	glPopMatrix();

	// Círculo Interior 2
	glPushMatrix();
		glTranslatef(0.125f, -0.0f, 0.0f);
		glScalef(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glColor3f(0.4f, 0.4f, 0.4f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.06*cos((double)i*(3.1416 / 180)) + .078, .06*sin((double)i*(3.14159 / 180.0)) - .078, 0.4);
			}
		glEnd();
	glPopMatrix();

	// Handle 2
	glPushMatrix();
		glTranslatef(0.21, -.06185f, 0.0f);
		glRotatef(110, 0, 0, 1);
		glScalef(0.15f, 0.15f, 0.15f);
		glBegin(GL_POLYGON);
			glColor3f(0.2f, 0.2f, 0.2f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.06*cos((double)i*(3.1416 / 180)) + .078, .06*sin((double)i*(3.14159 / 180.0)) - .078, 0.4);
			}
		glEnd();
	glPopMatrix();
}

void dibujarCocodrilo()
{
	for (int x = 0; x < 2; x++)
	{
		glPushMatrix();
			glTranslatef(posicionXCocodrilo[x], posicionYCocodrilo[x], 0.0f);
			glRotatef(180, 0, 0, 1);
			glRotatef(180, 1, 0, 0);

			glBegin(GL_POLYGON);
				glColor3f(0.0f, 0.7f, 0.0f);
				glVertex3f(.125f, 0.0f, 0.0f);
				glVertex3f(.1875f, -.0625f, 0.0f);
				glVertex3f(-.1875f, -.0625f, 0.0f);
				glVertex3f(-.0625f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_TRIANGLES);
				glColor3f(0.0f, 0.7f, 0.0f);
				glVertex3f(.1875f, 0.0625f, 0.0f);
				glVertex3f(.125, 0.0f, 0.0f);
				glVertex3f(-.0625f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex3f(.14f, 0.05f, 0.0f);
				glVertex3f(.14f, 0.03f, 0.0f);
				glVertex3f(.13f, 0.03f, 0.0f);
				glVertex3f(.13f, 0.05f, 0.0f);
			glEnd();
		glPopMatrix();
	}
}

void dibujarSerpiente()
{
	glPushMatrix();
		glTranslatef(posicionXSerpiente, posicionYSerpiente, 0.0f);
		glLineWidth(6);

		glBegin(GL_LINE_STRIP);
			glColor3f(0, .7, 0);
			glVertex3f(-.06f, -.02f, 0.0f);
			glVertex3f(-.06f, -.05f, 0.0f);
			glVertex3f(0.0f, -.05f, 0.0f);
			glVertex3f(0.0f, .01f, 0.0f);
			glVertex3f(0.07f, .01f, 0.0f);
			glVertex3f(0.07f, -.05f, 0.0f);
			glVertex3f(0.125f, -.05f, 0.0f);
			glVertex3f(0.125f, .04f, 0.0f);
			glVertex3f(0.16f, .04f, 0.0f);
		glEnd();

		glLineWidth(2);
		glColor3f(.7, 0, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.16f, .04f, 0.0f);
			glVertex3f(0.18f, .04f, 0.0f);
		glEnd();

		glLineWidth(2);
		glColor3f(1, 1, 1);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.14f, .04f, 0.0f);
			glVertex3f(0.15f, .04f, 0.0f);
		glEnd();

	glPopMatrix();
	glLineWidth(1);
}

void dibujarVidasRana()
{
	// Vida 1
	if (contadorVidasRana >= 1)
	{
		glPushMatrix();
			glTranslatef(-0.925f, 0.8f, 0.0f);
			glScalef(0.3f, 0.3f, 0.3f);
			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.042f, .03125f, 0.0f);
				glVertex3f(.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.0625f, 0.0f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.0625f, 0.0f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, .5, 0);
				glVertex3f(-.03125f, .048f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .048f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, .5, 0);
				glVertex3f(.03125f, .048f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .048f, 0.0f);
			glEnd();

		glPopMatrix();
	}


	// Vida rana 2
	if (contadorVidasRana >= 2)
	{
		glPushMatrix();
			glTranslatef(-0.925f, 0.8833f, 0.0f);
			glScalef(0.3f, 0.3f, 0.3f);
			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.042f, .03125f, 0.0f);
				glVertex3f(.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.0625f, 0.0f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.0625f, 0.0f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 1, 1);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, .5, 0);
				glVertex3f(-.03125f, .048f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .048f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, .5, 0);
				glVertex3f(.03125f, .048f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .048f, 0.0f);
			glEnd();

		glPopMatrix();
	}

	// Vida rana 3
	if (contadorVidasRana >= 3)
	{
		glPushMatrix();
		glTranslatef(-0.925f, 0.966f, 0.0f);
		glScalef(0.3f, 0.3f, 0.3f);

		glBegin(GL_QUADS);
		glColor3f(0, 1, 0);
		glVertex3f(.042f, .03125f, 0.0f);
		glVertex3f(.042f, -.03125f, 0.0f);
		glVertex3f(-.042f, -.03125f, 0.0f);
		glVertex3f(-.042f, .03125f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(0, 1, 0);
		glVertex3f(.0625f, 0.0f, 0.0f);
		glVertex3f(.0625f, -.0625f, 0.0f);
		glVertex3f(.03125f, -.0625f, 0.0f);
		glVertex3f(.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(0, 1, 0);
		glVertex3f(-.0625f, 0.0f, 0.0f);
		glVertex3f(-.0625f, -.0625f, 0.0f);
		glVertex3f(-.03125f, -.0625f, 0.0f);
		glVertex3f(-.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glVertex3f(.03125f, .0625f, 0.0f);
		glVertex3f(.03125f, .03125f, 0.0f);
		glVertex3f(.01f, .03128f, 0.0f);
		glVertex3f(.01f, .0625f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glVertex3f(-.03125f, .0625f, 0.0f);
		glVertex3f(-.03125f, .03125f, 0.0f);
		glVertex3f(-.01f, .03128f, 0.0f);
		glVertex3f(-.01f, .0625f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1, .5, 0);
		glVertex3f(-.03125f, .048f, 0.0f);
		glVertex3f(-.03125f, .03125f, 0.0f);
		glVertex3f(-.01f, .03128f, 0.0f);
		glVertex3f(-.01f, .048f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
		glColor3f(1, .5, 0);
		glVertex3f(.03125f, .048f, 0.0f);
		glVertex3f(.03125f, .03125f, 0.0f);
		glVertex3f(.01f, .03128f, 0.0f);
		glVertex3f(.01f, .048f, 0.0f);
		glEnd();

		glPopMatrix();
	}

	// Barra de Vida
	glPushMatrix();
		glTranslatef(-0.624, 1, 0.0);
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(1.245, 0.0);
		glVertex2f(1.245, -0.125);
		glVertex2f(0.0, -0.125);
		glVertex2f(0.0, 0.0);
	glEnd();

	if (contGolpes < 3)
	{
		glBegin(GL_QUADS);
			glColor3f(1, 0.0, 0.0);
			glVertex2f(0.5, -0.02);
			glVertex2f(0.5, -0.11);
			glVertex2f(0.02, -0.11);
			glVertex2f(0.02, -0.02);
		glEnd();
	}

	if (contGolpes < 2)
	{
		glBegin(GL_QUADS);
			glColor3f(0.8, 0.0, 0.0);
			glVertex2f(0.5, -0.02);
			glVertex2f(0.5, -0.11);
			glVertex2f(0.8, -0.11);
			glVertex2f(0.8, -0.02);
		glEnd();
	}

	if (contGolpes < 1 )
	{
		glBegin(GL_QUADS);
			glColor3f(0.5, 0.0, 0.0);
			glVertex2f(0.8, -0.02);
			glVertex2f(0.8, -0.11);
			glVertex2f(1.23, -0.11);
			glVertex2f(1.23, -0.02);
		glEnd();
	}
	glPopMatrix();
}

void dibujarVidasRanaCoop() {
	// Vida Rana 1
	if (contadorVidasRanaCoop >= 1)
	{
		glPushMatrix();
			glTranslatef(0.925f, 0.8f, 0.0f);
			glScalef(0.3f, 0.3f, 0.3f);

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.042f, .03125f, 0.0f);
				glVertex3f(.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.0625f, 0.0f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.0625f, 0.0f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.03125f, .048f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .048f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.03125f, .048f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .048f, 0.0f);
			glEnd();
		glPopMatrix();
	}

	//Vida Rana 2
	if (contadorVidasRanaCoop >= 2)
	{
		glPushMatrix();
			glTranslatef(0.925f, 0.8833f, 0.0f);
			glScalef(0.3f, 0.3f, 0.3f);

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.042f, .03125f, 0.0f);
				glVertex3f(.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.0625f, 0.0f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.0625f, 0.0f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.03125f, .048f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .048f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.03125f, .048f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .048f, 0.0f);
			glEnd();
		glPopMatrix();
	}

	//Vida Rana 3
	if (contadorVidasRanaCoop >= 3)
	{
		glPushMatrix();
			glTranslatef(0.925f, 0.966f, 0.0f);
			glScalef(0.3f, 0.3f, 0.3f);

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.042f, .03125f, 0.0f);
				glVertex3f(.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, -.03125f, 0.0f);
				glVertex3f(-.042f, .03125f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.0625f, 0.0f, 0.0f);
				glVertex3f(.0625f, -.0625f, 0.0f);
				glVertex3f(.03125f, -.0625f, 0.0f);
				glVertex3f(.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.0625f, 0.0f, 0.0f);
				glVertex3f(-.0625f, -.0625f, 0.0f);
				glVertex3f(-.03125f, -.0625f, 0.0f);
				glVertex3f(-.03125f, 0.0f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(.03125f, .0625f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(1, 0, 0);
				glVertex3f(-.03125f, .0625f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .0625f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(-.03125f, .048f, 0.0f);
				glVertex3f(-.03125f, .03125f, 0.0f);
				glVertex3f(-.01f, .03128f, 0.0f);
				glVertex3f(-.01f, .048f, 0.0f);
			glEnd();

			glBegin(GL_QUADS);
				glColor3f(0, 1, 0);
				glVertex3f(.03125f, .048f, 0.0f);
				glVertex3f(.03125f, .03125f, 0.0f);
				glVertex3f(.01f, .03128f, 0.0f);
				glVertex3f(.01f, .048f, 0.0f);
			glEnd();
		glPopMatrix();
	}	
}

void dibujarMuerteRana()
{
	glPushMatrix();
		glTranslatef(-0.125f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(.0625f, .048f, 0.0f);
			glVertex3f(.0625f, -.038f, 0.0f);
			glVertex3f(-.0625f, -.038f, 0.0f);
			glVertex3f(-.0625f, .048f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(.045f, .0625f, 0.0f);
			glVertex3f(.045f, -.01f, 0.0f);
			glVertex3f(-.045f, -.01f, 0.0f);
			glVertex3f(-.045f, .0625f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(.015f, -.06f, 0.0f);
			glVertex3f(.015f, 0.0f, 0.0f);
			glVertex3f(-.015f, 0.0f, 0.0f);
			glVertex3f(-.015f, -.06f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(.05f, 0.0f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(-.05f, 0.0f, 0.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -.0625f, 0.0f);
			glVertex3f(-.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(.04f, .03f, 0.0f);
			glVertex3f(.04f, -.02f, 0.0f);
			glVertex3f(.01f, -.02f, 0.0f);
			glVertex3f(.01f, .03f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(-.04f, .03f, 0.0f);
			glVertex3f(-.04f, -.02f, 0.0f);
			glVertex3f(-.01f, -.02f, 0.0f);
			glVertex3f(-.01f, .03f, 0.0f);
		glEnd();
	glPopMatrix();
}

void dibujarMuerteRanaCoop()
{
	glPushMatrix();
		glTranslatef(0.125f, 0.0f, 0.0f);

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(.0625f, .048f, 0.0f);
			glVertex3f(.0625f, -.038f, 0.0f);
			glVertex3f(-.0625f, -.038f, 0.0f);
			glVertex3f(-.0625f, .048f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(.045f, .0625f, 0.0f);
			glVertex3f(.045f, -.01f, 0.0f);
			glVertex3f(-.045f, -.01f, 0.0f);
			glVertex3f(-.045f, .0625f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(.015f, -.06f, 0.0f);
			glVertex3f(.015f, 0.0f, 0.0f);
			glVertex3f(-.015f, 0.0f, 0.0f);
			glVertex3f(-.015f, -.06f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(.05f, 0.0f, 0.0f);
			glVertex3f(.05f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(1, 0, 0);
			glVertex3f(-.05f, 0.0f, 0.0f);
			glVertex3f(-.05f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -.0625f, 0.0f);
			glVertex3f(-.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(.04f, .03f, 0.0f);
			glVertex3f(.04f, -.02f, 0.0f);
			glVertex3f(.01f, -.02f, 0.0f);
			glVertex3f(.01f, .03f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(-.04f, .03f, 0.0f);
			glVertex3f(-.04f, -.02f, 0.0f);
			glVertex3f(-.01f, -.02f, 0.0f);
			glVertex3f(-.01f, .03f, 0.0f);
		glEnd();

	glPopMatrix();
}

void dibujarTrofeo()
{
	glPushMatrix();
		glTranslatef(0.0, .125, 0.0);
		glScalef(2.0f, 2.0f, 0.0f);
		
		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.03125f, .03125f, 0.0f);
			glVertex3f(.03125f, -.03125f, 0.0f);
			glVertex3f(-.03125f, -.03125f, 0.0f);
			glVertex3f(-.03125f, .03125f, 0.0f);
		glEnd();

		// Patas Y Brazos
		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.045f, 0.0f, 0.0f);
			glVertex3f(.045f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.0625f, -0.04f, 0.0f);
			glVertex3f(.0625f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, -0.04f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(-.045f, 0.0f, 0.0f);
			glVertex3f(-.045f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -.0625f, 0.0f);
			glVertex3f(-.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(-.0625f, -0.04f, 0.0f);
			glVertex3f(-.0625f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -0.04f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.03125f, .0625f, 0.0f);
			glVertex3f(.03125f, .03125f, 0.0f);
			glVertex3f(.02f, .03128f, 0.0f);
			glVertex3f(.02f, .0625f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(-.03125f, .0625f, 0.0f);
			glVertex3f(-.03125f, .03125f, 0.0f);
			glVertex3f(-.02f, .03128f, 0.0f);
			glVertex3f(-.02f, .0625f, 0.0f);
		glEnd();

		//Ojitos
		glBegin(GL_QUADS);
			glColor3f(.8, .7, 0);
			glVertex3f(.02f, .02f, 0.0f);
			glVertex3f(.02f, .0f, 0.0f);
			glVertex3f(.01f, .0f, 0.0f);
			glVertex3f(.01f, .02f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.8, .7, 0);
			glVertex3f(-.02f, .02f, 0.0f);
			glVertex3f(-.02f, .0f, 0.0f);
			glVertex3f(-.01f, .0f, 0.0f);
			glVertex3f(-.01f, .02f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.011f, -.03125f, 0.0f);
			glVertex3f(.011f, -.1875f, 0.0f);
			glVertex3f(-.011f, -.1875f, 0.0f);
			glVertex3f(-.011f, -.03125f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.05f, -.16f, 0.0f);
			glVertex3f(.05f, -.1875f, 0.0f);
			glVertex3f(-.05f, -.1875f, 0.0f);
			glVertex3f(-.05f, -.16f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.015f, -.03125f, 0.0f);
			glVertex3f(.015f, -.05f, 0.0f);
			glVertex3f(-.015f, -.05f, 0.0f);
			glVertex3f(-.015f, -.03125f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.02f, -.1f, 0.0f);
			glVertex3f(.03f, -.1875f, 0.0f);
			glVertex3f(-.03f, -.1875f, 0.0f);
			glVertex3f(-.02f, -.1f, 0.0f);
		glEnd();

		// Corona
		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.0625f, .13f, 0.0f);
			glVertex3f(.0625f, .07f, 0.0f);
			glVertex3f(-.0625f, .07f, 0.0f);
			glVertex3f(-.0625f, .13f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.0625f, .1875f, 0.0f);
			glVertex3f(.0625f, .07f, 0.0f);
			glVertex3f(.035f, .07f, 0.0f);
			glVertex3f(.035f, .1875f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(-.0625f, .1875f, 0.0f);
			glVertex3f(-.0625f, .07f, 0.0f);
			glVertex3f(-.035f, .07f, 0.0f);
			glVertex3f(-.035f, .1875f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, .8, 0);
			glVertex3f(.02f, .165f, 0.0f);
			glVertex3f(.02f, .07f, 0.0f);
			glVertex3f(-.02f, .07f, 0.0f);
			glVertex3f(-.02f, .165f, 0.0f);
		glEnd();

	glPopMatrix();
}

void dibujarTrofeoCoop()
{
	glPushMatrix();
		glTranslatef(0.0, .125, 0.0);
		glScalef(2.0f, 2.0f, 0.0f);

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.03125f, .03125f, 0.0f);
			glVertex3f(.03125f, -.03125f, 0.0f);
			glVertex3f(-.03125f, -.03125f, 0.0f);
			glVertex3f(-.03125f, .03125f, 0.0f);
		glEnd();

		// Patas Y Brazos
		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.045f, 0.0f, 0.0f);
			glVertex3f(.045f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.0625f, -0.04f, 0.0f);
			glVertex3f(.0625f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, -0.04f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(-.045f, 0.0f, 0.0f);
			glVertex3f(-.045f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -.0625f, 0.0f);
			glVertex3f(-.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(-.0625f, -0.04f, 0.0f);
			glVertex3f(-.0625f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -.0625f, 0.0f);
			glVertex3f(-.03125f, -0.04f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.03125f, .0625f, 0.0f);
			glVertex3f(.03125f, .03125f, 0.0f);
			glVertex3f(.02f, .03128f, 0.0f);
			glVertex3f(.02f, .0625f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(-.03125f, .0625f, 0.0f);
			glVertex3f(-.03125f, .03125f, 0.0f);
			glVertex3f(-.02f, .03128f, 0.0f);
			glVertex3f(-.02f, .0625f, 0.0f);
		glEnd();

		//Ojitos
		glBegin(GL_QUADS);
			glColor3f(.8, .7, 0);
			glVertex3f(.02f, .02f, 0.0f);
			glVertex3f(.02f, .0f, 0.0f);
			glVertex3f(.01f, .0f, 0.0f);
			glVertex3f(.01f, .02f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.8, .7, 0);
			glVertex3f(-.02f, .02f, 0.0f);
			glVertex3f(-.02f, .0f, 0.0f);
			glVertex3f(-.01f, .0f, 0.0f);
			glVertex3f(-.01f, .02f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.011f, -.03125f, 0.0f);
			glVertex3f(.011f, -.1875f, 0.0f);
			glVertex3f(-.011f, -.1875f, 0.0f);
			glVertex3f(-.011f, -.03125f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.05f, -.16f, 0.0f);
			glVertex3f(.05f, -.1875f, 0.0f);
			glVertex3f(-.05f, -.1875f, 0.0f);
			glVertex3f(-.05f, -.16f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.015f, -.03125f, 0.0f);
			glVertex3f(.015f, -.05f, 0.0f);
			glVertex3f(-.015f, -.05f, 0.0f);
			glVertex3f(-.015f, -.03125f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.02f, -.1f, 0.0f);
			glVertex3f(.03f, -.1875f, 0.0f);
			glVertex3f(-.03f, -.1875f, 0.0f);
			glVertex3f(-.02f, -.1f, 0.0f);
		glEnd();

		// Corona
		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.0625f, .13f, 0.0f);
			glVertex3f(.0625f, .07f, 0.0f);
			glVertex3f(-.0625f, .07f, 0.0f);
			glVertex3f(-.0625f, .13f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.0625f, .1875f, 0.0f);
			glVertex3f(.0625f, .07f, 0.0f);
			glVertex3f(.035f, .07f, 0.0f);
			glVertex3f(.035f, .1875f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(-.0625f, .1875f, 0.0f);
			glVertex3f(-.0625f, .07f, 0.0f);
			glVertex3f(-.035f, .07f, 0.0f);
			glVertex3f(-.035f, .1875f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(.9, 0, 0);
			glVertex3f(.02f, .165f, 0.0f);
			glVertex3f(.02f, .07f, 0.0f);
			glVertex3f(-.02f, .07f, 0.0f);
			glVertex3f(-.02f, .165f, 0.0f);
		glEnd();
	glPopMatrix();
}

void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		restart();
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		xRana += saltoRana;
		if (giroRana != 0)
		{
			giroRana = 0;
			giroRana -= 90;
		}
		else
			giroRana -= 90;
	}
	else

		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			xRana -= saltoRana;
			if (giroRana != 0)
			{
				giroRana = 0;
				giroRana += 90;
			}
			else
				giroRana += 90;
		}
		else

			if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			{
				yRana += saltoRana;
				if (giroRana != 0)
				{
					giroRana = 0;
				}
			}
			else

				if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
				{
					yRana -= saltoRana;
					if (giroRana != 0)
					{
						giroRana = 0;
						giroRana += 180;
					}
					else
						giroRana += 180;
				}

}

void moverRanaCoop()
{
	int estadoTeclaD = glfwGetKey(window, GLFW_KEY_D);
	int estadoTeclaA = glfwGetKey(window, GLFW_KEY_A);
	int estadoTeclaW = glfwGetKey(window, GLFW_KEY_W);
	int estadoTeclaS = glfwGetKey(window, GLFW_KEY_S);

	if (estadoTeclaD == GLFW_PRESS)
	{

		if (anguloRotacionRanaCoop < -360)
			anguloRotacionRanaCoop += 360;
		else
			anguloRotacionRanaCoop -= velocidadAngular * (float)tiempoDiferencial;
	}
	if (estadoTeclaA == GLFW_PRESS)
	{
		if (anguloRotacionRanaCoop > 360)
			anguloRotacionRanaCoop -= 360;
		else
			anguloRotacionRanaCoop += velocidadAngular * (float)tiempoDiferencial;
	}
	if (estadoTeclaW == GLFW_PRESS)
	{
		xRanaCoop += cos((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidadRana * tiempoDiferencial;
		yRanaCoop += sin((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidadRana * tiempoDiferencial;
	}
	if (estadoTeclaS == GLFW_PRESS)
	{
		xRanaCoop -= cos((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidadRana * tiempoDiferencial;
		yRanaCoop -= sin((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidadRana * tiempoDiferencial;
	}

	if (xRanaCoop > 0.9375) {
		xRanaCoop = 0.9375;
	}

	if (xRanaCoop < -0.9375) {
		xRanaCoop = -0.9375;
	}

	if (yRanaCoop > 0.9375) {
		yRanaCoop = 0.9375;
	}

	if (yRanaCoop < -0.9375) {
		yRanaCoop = -0.9375;
	}
}

void moverCarrosDerecha()
{
	if (direccionDerecha == Direccion::Derecha)
	{
		for (int x = 0; x < 6; x++) {
			posicionXCarroDerecha[x] += velocidadCarros * tiempoDiferencial;
			if (posicionXCarroDerecha[x] > 1.125f)
			{
				posicionXCarroDerecha[x] = -1.125f;
			}
		}
	}
}

void moverCarrosIzquierda()
{
	if (direccionIzquierda == Direccion::Izquierda)
	{
		for (int y = 0; y < 6; y++) {
			posicionXCarroIzquierda[y] -= velocidadCarros * tiempoDiferencial;
			if (posicionXCarroIzquierda[y] < -1.125f)
			{
				posicionXCarroIzquierda[y] = 1.125f;
			}
		}
	}
}

void moverTroncos()
{
	if (direccionDerecha == Direccion::Derecha)
	{
		for (int x = 0; x < 9; x++) {
			posicionXTronco[x] += velocidadTroncos * tiempoDiferencial;
			if (posicionXTronco[x] > 1.375f)
			{
				posicionXTronco[x] = -1.375f;
			}
		}
	}
}

void moverTortugas()
{
	if (direccionIzquierda == Direccion::Izquierda)
	{
		for (int x = 0; x < 8; x++) {
			posicionXTortugas[x] -= velocidadTortugas * tiempoDiferencial;
			if (posicionXTortugas[x] < -1.125f)
			{
				posicionXTortugas[x] = 1.125f;
			}
		}
	}
}

void moverCocodrilos()
{
	if (direccionIzquierda == Direccion::Izquierda)
	{
		for (int x = 0; x < 2; x++) {
			posicionXCocodrilo[x] -= velocidadCocodrilo * tiempoDiferencial;
			if (posicionXCocodrilo[x] < -1.375f)
			{
				posicionXCocodrilo[x] = 1.375f;
			}
		}
	}
}

void moverSerpiente()
{
	if (direccionDerecha == Direccion::Derecha)
	{
		posicionXSerpiente += velocidadSerpiente * tiempoDiferencial;
		if (posicionXSerpiente > 1.25f)
		{
			posicionXSerpiente = -1.25f;
		}
	}
}

void colisionesVictoria()
{
	// Si la rana llega al nenunfar
	for (int c = 0; c < 4; c++)
	{
		if ((xRana - .0625f < posicionXNenunfares[c] + .0625f) && (xRana + .0625f > posicionXNenunfares[c] - .0625f) && (yRana - .0625f < posicionYNenunfares + .0625f) && (yRana + .0625f > posicionYNenunfares - .0625f))
		{
			colorVictoriaNenunfar[c] = true;
			xRana = -0.9375f;
			yRana = -0.9375f;
		}
	}
	if (colorVictoriaNenunfar[0] == true && colorVictoriaNenunfar[1] == true && colorVictoriaNenunfar[2] == true && colorVictoriaNenunfar[3] == true)
	{
		dibujarTrofeo();
		contadorVidasRanaCoop = 0;
	}
}

void colisionesDerrota()
{
	// Si la rana choca con el carro
	for (int c = 0; c < 6; c++)
	{
		if ((xRana - .0625f < posicionXCarroDerecha[c] + .0625f) && (xRana + .0625f > posicionXCarroDerecha[c] - .0625f) && (yRana - .0625f < posicionYCarroDerecha[c] + .0625f) && (yRana + .0625f > posicionYCarroDerecha[c] - .0625f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contadorVidasRana--;
		}
	}


	for (int c = 0; c < 6; c++)
	{
		if ((xRana - .0625f < posicionXCarroIzquierda[c] + .0625f) && (xRana + .0625f > posicionXCarroIzquierda[c] - .0625f) && (yRana - .0625f < posicionYCarroIzquierda[c] + .0625f) && (yRana + .0625f > posicionYCarroIzquierda[c] - .0625f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contadorVidasRana--;
		}
	}

	// Si la rana choca con las alcantarillas
	/*for (int c = 0; c < 2; c++)
	{
		if ((xRana - .012f < posicionXAlcantarillas[c] + .012f) && (xRana + .012f > posicionXAlcantarillas[c] - .012f) && (yRana - .012f < posicionYAlcantarillas[c] + .012f) && (yRana + .012f > posicionYAlcantarillas[c] - .012f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contadorVidasRana--;
		}
	}*/

	// Si la rana choca con la serpiente
	for (int c = 0; c < 6; c++)
	{
		if ((xRana - .0625f < posicionXSerpiente + .0625f) && (xRana + .0625f > posicionXSerpiente - .0625f) && (yRana - .0625f < posicionYSerpiente + .0625f) && (yRana + .0625f > posicionYSerpiente - .0625f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contadorVidasRana--;
		}
	}


	// COLISIONES ACUÍFERAS
	// Si la rana se sube al cocodrilo
	for (int c = 0; c < 2; c++)
	{
		if ((xRana - .0625f < posicionXCocodrilo[c] + .0625f) && (xRana + .0625f > posicionXCocodrilo[c] - .0625f) && (yRana - .0625f < posicionYCocodrilo[c] + .0625f) && (yRana + .0625f > posicionYCocodrilo[c] - .0625f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contGolpes = 0;
			contadorVidasRana--;
		}
	}
	// Si la rana se sube a un tronco
	for (int c = 0; c < 9; c++)
	{
		if ((xRana - .0625f < posicionXTronco[c] + .125f) && (xRana + .0625f > posicionXTronco[c] - .25f) && (yRana - .0625f < posicionYTronco[c] + .0625f) && (yRana + .0625f > posicionYTronco[c] - .0625f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contGolpes = 0;
			contadorVidasRana--;
		}
	}

	// Si la rana se sube a una tortuga
	for (int c = 0; c < 8; c++)
	{
		if ((xRana - .0625f < posicionXTortugas[c] + .0625f) && (xRana + .0625f > posicionXTortugas[c] - .0625f) && (yRana - .0625f < posicionYTortugas[c] + .0625f) && (yRana + .0625f > posicionYTortugas[c] - .0625f))
		{
			xRana = -0.9375f;
			yRana = -0.9375f;
			contGolpes = 0;
			contadorVidasRana--;
		}
	}


	// Si la rana muere
	if (contadorVidasRana <= 0)
	{
		dibujarMuerteRana();
		dibujarTrofeoCoop();
		contadorVidasRanaCoop = 3;
	}
}
void colisionesVictoriaCoop()
{
	// Si la ranacoop golpea la rana
	for (int c = 0; c < 4; c++)
	{
		if ((xRanaCoop - .0625f < xRana + .0625f) && (xRanaCoop + .0625f > xRana - .0625f) && (yRanaCoop - .0625f < yRana + .0625f) && (yRanaCoop + .0625f > yRana - .0625f))
		{
			xRanaCoop = 0.0f;
			yRanaCoop = 0.0f;
			contGolpes++;
			if (contGolpes == 3)
			{
				xRana = -0.9375f;
				yRana = -0.9375f;
				contGolpes = 0;
				contadorVidasRana--;
			}
		}
	}
}

void colisionesDerrotaCoop()
{
	// Si la ranacoop choca con un carro
	for (int c = 0; c < 6; c++)
	{
		if ((xRanaCoop - .03f < posicionXCarroDerecha[c] + .0625f) && (xRanaCoop + .03f > posicionXCarroDerecha[c] - .0625f) && (yRanaCoop - .03f < posicionYCarroDerecha[c] + .0625f) && (yRanaCoop + .03f > posicionYCarroDerecha[c] - .0625f))
		{
			xRanaCoop = 0.9375f;
			yRanaCoop = -0.9375f;
			contadorVidasRanaCoop--;
		}
	}

	for (int c = 0; c < 6; c++)
	{
		if ((xRanaCoop - .03f < posicionXCarroIzquierda[c] + .0625f) && (xRanaCoop + .03f > posicionXCarroIzquierda[c] - .0625f) && (yRanaCoop - .03f < posicionYCarroIzquierda[c] + .0625f) && (yRanaCoop + .03f > posicionYCarroIzquierda[c] - .0625f))
		{
			xRanaCoop = 0.9375f;
			yRanaCoop = -0.9375f;
			contadorVidasRanaCoop--;
		}
	}

	// Si la ranacoop choca con la serpiente
	for (int c = 0; c < 6; c++)
	{
		if ((xRanaCoop - .0625f < posicionXSerpiente + .0625f) && (xRanaCoop + .0625f > posicionXSerpiente - .0625f) && (yRanaCoop - .0625f < posicionYSerpiente + .0625f) && (yRanaCoop + .0625f > posicionYSerpiente - .0625f))
		{
			xRanaCoop = 0.9375f;
			yRanaCoop = -0.9375f;
			contadorVidasRanaCoop--;
		}
	}
	if (contadorVidasRanaCoop <= 0)
	{
		dibujarMuerteRanaCoop();
	}
}

void restart()
{
	if(contadorVidasRana <= 0 || contadorVidasRanaCoop <= 0){
		contadorVidasRana = 3;
		contadorVidasRanaCoop = 3;
		xRana = -.9375f;
		yRana = -.9375f;
		xRanaCoop = .9375f;
		yRanaCoop = -.9375f;
		contGolpes = 0;
		for (int x = 0; x < 4; x++)
		{
			colorVictoriaNenunfar[x] = false;
		}
	}
}