#include "stdafx.h"
#include <stdio.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include<iostream>

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
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void moverRanaCoop();
void moverCarrosDerecha();
void moverCarrosIzquierda();

// Variables Globales
float xRana = -.9375f;
float yRana = -.9375f;
float xRanaCoop = .9375f;
float yRanaCoop = -.9375f;
float saltoRana = .125f;
float giroRana = 0.0f;
float velocidad = 1.2f;
float velocidadActual = 0.0f;
float aceleracion = 0.8f;
float desaceleracion = 0.4f;
float anguloRotacionRanaCoop = 0.0f;
float anguloTrayectoria = 0.0f;
float velocidadAngular = 180.0f;
float posicionXCarroIzquierda[3] = {-.575f, 0.5f, 0.375f};
float posicionYCarroIzquierda[3] = {-.6875f, -0.4375f, -0.1875f};
float posicionXCarroDerecha[3] = {.8125f, 0.0f, -0.1875};
float posicionYCarroDerecha[3] = {-.8125f, -.5625f, -0.3125f};
float posicionXNenunfares[4] = {-0.7475f, -0.25f, 0.2475f, 0.7475f};
double tiempoAnterior = 0.0f;
double tiempoActual = 0.0f;
double tiempoDiferencial = 0.0f;
bool destruirCarroDerecha = false;
bool destruirCarroIzquierda = false;
bool destruirTronco = false;
enum Direccion { Izquierda, Derecha };
Direccion direccionIzquierda = Direccion::Izquierda;
Direccion direccionDerecha = Direccion::Derecha;

// Declaramos apuntador de ventana
GLFWwindow *window;

int main()
{
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

	//ciclo de dibujo (Draw loop)
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
	dibujarRana();
	dibujarRanaCoop();
	dibujarNenunfar();
	//dibujarTortuga();
}

void actualizar()
{
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;

	//moverRana();
	moverRanaCoop();
	moverCarrosDerecha();
	moverCarrosIzquierda();

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
	glPushMatrix();
		glTranslatef(xRana, yRana, 0.0f);
		glRotatef(giroRana, 0.0f, 0.0f, 1.0f);

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

void dibujarRanaCoop() {
	glPushMatrix();
		glTranslatef(xRanaCoop, yRanaCoop, 0.0f);
		glRotatef(anguloRotacionRanaCoop, 0.0f, 0.0f, 1.0f);

		glBegin(GL_QUADS);
			glColor3f(0, 0, 1);
			glVertex3f(.042f, .03125f, 0.0f);
			glVertex3f(.042f, -.03125f, 0.0f);
			glVertex3f(-.042f, -.03125f, 0.0f);
			glVertex3f(-.042f, .03125f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 0, 1);
			glVertex3f(.0625f, 0.0f, 0.0f);
			glVertex3f(.0625f, -.0625f, 0.0f);
			glVertex3f(.03125f, -.0625f, 0.0f);
			glVertex3f(.03125f, 0.0f, 0.0f);
		glEnd();

		glBegin(GL_QUADS);
			glColor3f(0, 0, 1);
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

	// (Izquierda) Carro Azul 5to Carril
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

	// Camion Rojo 6to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda[2], posicionYCarroIzquierda[2], 0.0f);
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
}

void dibujarTroncos()
{
	// Inicio de la Matriz
	glPushMatrix();

		// Tronco carril lago 1
		glBegin(GL_QUADS);
			glColor3f(0.737f, 0.5607f, 0.5607f);
			glVertex3f(.4375f, .0f, 0.0f);
			glVertex3f(.0625f, .0f, 0.0f);
			glVertex3f(.0625f, .125f, 0.0f);
			glVertex3f(.4375f, .125f, 0.0f);
		glEnd();

		// Tronco carril lago 2
		glBegin(GL_QUADS);
			glColor3f(0.737f, 0.5607f, 0.5607f);
			glVertex3f(-.3125f, .25f, 0.0f);
			glVertex3f(-.8125f, .25f, 0.0f);
			glVertex3f(-.8125f, .125f, 0.0f);
			glVertex3f(-.3125f, .125f, 0.0f);
		glEnd();

		// Tronco carril lago 3
		glBegin(GL_QUADS);
			glColor3f(0.737f, 0.5607f, 0.5607f);
			glVertex3f(.5625f, .25f, 0.0f);
			glVertex3f(.8125f, .25f, 0.0f);
			glVertex3f(.8125f, .375f, 0.0f);
			glVertex3f(.5625f, .375f, 0.0f);
		glEnd();


		// Tronco carril lago 4
		glBegin(GL_QUADS);
			glColor3f(0.737f, 0.5607f, 0.5607f);
			glVertex3f(.1875f, .5f, 0.0f);
			glVertex3f(-.1875f, .5f, 0.0f);
			glVertex3f(-.1875f, .375f, 0.0f);
			glVertex3f(.1875f, .375f, 0.0f);
		glEnd();

		// Tronco carril lago 4
		glBegin(GL_QUADS);
			glColor3f(0.737f, 0.5607f, 0.5607f);
			glVertex3f(-.4375f, .5f, 0.0f);
			glVertex3f(-.6875f, .5f, 0.0f);
			glVertex3f(-.6875f, .625f, 0.0f);
			glVertex3f(-.4375f, .625f, 0.0f);
		glEnd();

		// Tronco carril lago 4
		glBegin(GL_QUADS);
			glColor3f(0.737f, 0.5607f, 0.5607f);
			glVertex3f(.3725f, .75f, 0.0f);
			glVertex3f(.8725f, .75f, 0.0f);
			glVertex3f(.8725f, .625f, 0.0f);
			glVertex3f(.3725f, .625f, 0.0f);
		glEnd();

	// Fin de la Matriz
	glPopMatrix();
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
		glColor3f(0.2f, 0.6f, 0.8f);
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
}

void dibujarNenunfar()
{
	/* ESTOY MUY DORMIDO PARA SABER QUE CARAJOS SUCEDE AQUÍ

		glBegin(GL_POLYGON);
			glTranslatef(posicionXNenunfares[0], 0.8125f, 0.0f);
			glColor3f(0.2f, 0.9f, 0.2f);
			for (int i = 0; i < 360; i++)
			{
				glVertex3f(.0625*cos((double)i*(3.1416 / 180)), .0625*sin((double)i*(3.14159 / 180.0)), 0.4);
			}
		glEnd();

		glBegin(GL_TRIANGLES);
			glColor3f(0.2f, 0.6f, 0.8f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(.03, -.0625, 0.0f);
			glVertex3f(-.03, -.0625, 0.0f);
		glEnd();
	*/
}

void dibujarTortuga()
{
	glPushMatrix();

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
	glPopMatrix();

}

void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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

/*void moverRana()
{
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	int estadoTeclaAbajo = glfwGetKey(window, GLFW_KEY_DOWN);


	if (estadoTeclaDerecha == GLFW_PRESS)
	{
		xRana += velocidad * tiempoDiferencial;
	}
	if (estadoTeclaIzquierda == GLFW_PRESS)
	{
		xRana -= velocidad * tiempoDiferencial;
	}
	if (estadoTeclaArriba == GLFW_PRESS)
	{
		yRana += velocidad * tiempoDiferencial;
	}
	if (estadoTeclaAbajo == GLFW_PRESS)
	{
		yRana -= velocidad * tiempoDiferencial;
	}
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
}*/

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
		xRanaCoop += cos((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidad * tiempoDiferencial;
		yRanaCoop += sin((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidad * tiempoDiferencial;
	}
	if (estadoTeclaS == GLFW_PRESS)
	{
		xRanaCoop -= cos((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidad * tiempoDiferencial;
		yRanaCoop -= sin((anguloRotacionRanaCoop + 90.0f) * (3.14159f / 180.0f)) * velocidad * tiempoDiferencial;
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
		for (int x = 0; x < 3; x++) {
			posicionXCarroDerecha[x] += 0.7f * tiempoDiferencial;
			if (posicionXCarroDerecha[x] > 1.125f)
			{
				posicionXCarroDerecha[x] = -1.125f;
				destruirCarroDerecha = true;
			}
		}
	}
}

void moverCarrosIzquierda()
{
	if (direccionIzquierda == Direccion::Izquierda)
	{
		for (int y = 0; y < 3; y++) {
			posicionXCarroIzquierda[y] -= 0.7f * tiempoDiferencial;
			if (posicionXCarroIzquierda[y] < -1.125f)
			{
				posicionXCarroIzquierda[y] = 1.125f;
				destruirCarroIzquierda = true;
			}
		}
	}
}