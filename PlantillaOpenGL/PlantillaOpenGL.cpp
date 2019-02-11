#include "stdafx.h"
#include <stdio.h>
#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include<iostream>

using namespace std;

void dibujar();
void actualizar();
void dibujarRana();
void dibujarCarro();
void dibujarTroncos();
void dibujarMapa();
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void moverRana();
void moverCarrosDerecha();
void moverCarrosIzquierda();

float xRana = 0.0f;
float yRana = 0.0f;
float speed = 1.4f;

float posicionXCarroIzquierda = 0.0f;
float posicionXCarroDerecha = 0.0f;

double tiempoAnterior = 0.0f;
double tiempoActual = 0.0f;
double tiempoDiferencial = 0.0f;
bool destruirCarroDerecha = false;
bool destruirCarroIzquierda = false;
bool destruirTronco = false;

enum Direccion { Izquierda, Derecha };
Direccion direccionIzquierda = Direccion::Izquierda;
Direccion direccionDerecha = Direccion::Derecha;

//Declaramos apuntador de ventana
GLFWwindow *window;

int main()
{
	//Si no se pudo iniciar glfw
	//terminamos ejecución 
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	//Si se pudo inciar GLFW
	//Entonces inicializamos la ventana

	window = glfwCreateWindow(1024, 768, "Ventana", NULL, NULL);

	//Si no podemos iniciar la ventana 
	//Entonces terminamos ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecer el contexto
	glfwMakeContextCurrent(window);


	//Una vez establecido el contexto
	//Activamos fnciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK)
	{
		cout << glewGetErrorString(errorGlew);

	}


	const GLubyte *versionGL =
		glGetString(GL_VERSION);
	cout << "Version OpenGL: "
		<< versionGL;

	glfwSetKeyCallback(window, teclado_callback);
	tiempoAnterior = glfwGetTime();

	//ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0,1024, 768);
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
}

void actualizar() 
{
	tiempoActual = glfwGetTime();
	tiempoDiferencial = tiempoActual - tiempoAnterior;

	moverRana();
	moverCarrosDerecha();
	moverCarrosIzquierda();

	tiempoAnterior = tiempoActual;

	if (xRana > 0.9375) {
		xRana = 0.9375;
	}

	if (xRana < -0.9375) {
		xRana = -0.9375;
	}

	if (yRana > 1.875) {
		yRana = 1.875;
	}

	if (yRana < 0) {
		yRana = 0;
	}
}

void dibujarRana()
{
	glPushMatrix();
		glTranslatef(xRana, yRana, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(0, 1, 0);
			glVertex3f(-.0625f, -.875f, 0.0f);
			glVertex3f(-.0625f, -1.0f, 0.0f);
			glVertex3f(.0625f, -1.0, 0.0f);
			glVertex3f(.0625f, -.875f, 0.0f);
		glEnd();
	glPopMatrix();
}

void dibujarCarro()
{
	// Carro Rosa 1er Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.45f, 0.796f);
			glVertex3f(-.8125f, -.875f, 0.0f);
			glVertex3f(-.8125f, -.75f, 0.0f);
			glVertex3f(-.9375f, -.75f, 0.0f);
			glVertex3f(-.9375f, -.875f, 0.0f);
		glEnd();
	glPopMatrix();
	
	// Camión Morado 2do Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(0.5f, 0.8f, 0.8f);
			glVertex3f(.6875f, -.75f, 0.0f);
			glVertex3f(.5625f, -.75f, 0.0f);
			glVertex3f(.5625f, -.625f, 0.0f);
			glVertex3f(.6875f, -.625f, 0.0f);
		glEnd();
	glPopMatrix();

	// Carro Morado 3er Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(1.0f, 0.7f, 0.2f);
			glVertex3f(-.3125f, -.625f, 0.0f);
			glVertex3f(-.5625f, -.625f, 0.0f);
			glVertex3f(-.5625f, -.5f, 0.0f);
			glVertex3f(-.3125f, -.5f, 0.0f);
		glEnd();
	glPopMatrix();

	// Carro Rosa 4to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(0.5f, 0.1f, 0.4f);
			glVertex3f(-.0625f, -.375f, 0.0f);
			glVertex3f(.0625f, -.375f, 0.0f);
			glVertex3f(.0625f, -.5f, 0.0f);
			glVertex3f(-.0625f, -.5f, 0.0f);
		glEnd();
	glPopMatrix();

	// Carro Azul 5to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroDerecha, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.2f, 0.6f);
			glVertex3f(-.5625f, -.375f, 0.0f);
			glVertex3f(-.6875f, -.375f, 0.0f);
			glVertex3f(-.6875f, -.25f, 0.0f);
			glVertex3f(-.5625f, -.25f, 0.0f);
		glEnd();
	glPopMatrix();

	// Camion Rojo 6to Carril
	glPushMatrix();
		glTranslatef(posicionXCarroIzquierda, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glColor3f(0.8f, 0.0f, 0.1f);
			glVertex3f(.4375f, -.125f, 0.0f);
			glVertex3f(.6875f, -.125f, 0.0f);
			glVertex3f(.6875f, -.25f, 0.0f);
			glVertex3f(.4375f, -.25f, 0.0f);
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

	//Agua victoria  4
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(.8725f, .75f, 0.0f);
		glVertex3f(.6225f, .75f, 0.0f);
		glVertex3f(.6225f, .875f, 0.0f);
		glVertex3f(.8725f, .875f, 0.0f);
	glEnd();

	//Agua victoria  3
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(.3725f, .75f, 0.0f);
		glVertex3f(.1225f, .75f, 0.0f);
		glVertex3f(.1225f, .875f, 0.0f);
		glVertex3f(.3725f, .875f, 0.0f);
	glEnd();
	//Agua victoria 2
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(-.1225f, .75f, 0.0f);
		glVertex3f(-.3775f, .75f, 0.0f);
		glVertex3f(-.3775f, .875f, 0.0f);
		glVertex3f(-.1225f, .875f, 0.0f);
	glEnd();
	//Agua victoria 4
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.6f, 0.8f);
		glVertex3f(-.8725f, .75f, 0.0f);
		glVertex3f(-.6225f, .75f, 0.0f);
		glVertex3f(-.6225f, .875f, 0.0f);
		glVertex3f(-.8725f, .875f, 0.0f);
	glEnd();
}

void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void moverRana()
{
	int estadoTeclaDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	int estadoTeclaIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	int estadoTeclaArriba = glfwGetKey(window, GLFW_KEY_UP);
	int estadoTeclaAbajo = glfwGetKey(window, GLFW_KEY_DOWN);

	int estadoTeclaD = glfwGetKey(window, GLFW_KEY_D);
	int estadoTeclaA= glfwGetKey(window, GLFW_KEY_A);
	int estadoTeclaW = glfwGetKey(window, GLFW_KEY_W);
	int estadoTeclaS = glfwGetKey(window, GLFW_KEY_S);
	
	if (estadoTeclaDerecha == GLFW_PRESS || estadoTeclaD == GLFW_PRESS)
	{
		xRana += speed * tiempoDiferencial;
	}
	if (estadoTeclaIzquierda == GLFW_PRESS || estadoTeclaA == GLFW_PRESS)
	{
		xRana -= speed * tiempoDiferencial;
	}
	if (estadoTeclaArriba == GLFW_PRESS || estadoTeclaW == GLFW_PRESS)
	{
		yRana += speed * tiempoDiferencial;
	}
	if (estadoTeclaAbajo == GLFW_PRESS || estadoTeclaS == GLFW_PRESS)
	{
		yRana -= speed * tiempoDiferencial;
	}
}

void moverCarrosDerecha()
{
	if (direccionDerecha == Direccion::Derecha)
	{
		posicionXCarroDerecha += 0.7f * tiempoDiferencial;
		// posicionXTronco -= 0.001f;
		if (posicionXCarroDerecha > 1.905f)
		{
			posicionXCarroDerecha = -.875f;
			destruirCarroDerecha = true;
			// destruirTronco = true;
		}
	}
}

void moverCarrosIzquierda()
{
	if (direccionIzquierda == Direccion::Izquierda)
	{
		posicionXCarroIzquierda -= 0.7f * tiempoDiferencial;
		if (posicionXCarroIzquierda < -1.905f)
		{
			posicionXCarroIzquierda = 1.0f;
		}
	}
}