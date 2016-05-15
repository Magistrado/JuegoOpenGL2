#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>


using namespace std;

#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	10.0f
#define DEF_floorGridZSteps	10.0f

// Cosas de Luis

/////////////////// Marco
GLfloat anchoMundo = 14;
GLfloat altoMundo = 18;
// Colores
float morado[3] = {0.3,0,0.3};
float amarillo[3] = {0.6,0.6,0.0};

// Pelota
float tamPelota = 0.3;
GLfloat xpelota = 0;

// Alto y ancho de la base
GLfloat width = 1.0;


// Vectores de movimiento de la pelota
GLfloat veloHori;
GLfloat veloVert;
GLfloat veloHoriPasada = 0.0, veloVertPasada = 0.0;
GLfloat despBase = 0.0, anchoBase = 3, altoBase = 0.5;
bool cambioVertical = false;
bool cambioHorizontal = false;
bool inicio = FALSE;


void ejesCoordenada(float w) {

	glLineWidth(w);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(w-1.0);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {
				if ((i%2)==0){
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}

	glEnd();

	glLineWidth(1.0);
}
class Base {
public:
	GLfloat ancho;
	GLfloat alto;
	GLfloat xcord;
	GLfloat ycord;

	Base(GLfloat anch, GLfloat alt) {
		ancho = anch;
		alto = alt;
	};

	Base() {};

	void dibuja(GLfloat y) {
		// Crea una base sobre el origen. Centrada sobre el eje x y por encima.
		GLfloat amplitud = ancho / 2;
		glPushMatrix();
			glTranslatef(despBase, y, 0.0);
			xcord = despBase;
			ycord = y;
			glBegin(GL_POLYGON);
				glColor3f(0, 0, 1);
				glVertex2f(amplitud, 0.0);
				glVertex2f(amplitud, alto);
				glVertex2f(-amplitud, alto);
				glVertex2f(-amplitud, 0.0);
			glEnd();
		glPopMatrix();
	};

	float* posicion(){
		float centro[2];
		centro[0] = xcord;
		centro[1] = ycord;
		printf("(%f','%f)\n", xcord, ycord);
		return centro;
	};
};
class Pelota {
public:
	GLfloat radio;
	GLfloat xcord;
	GLfloat ycord;

	Pelota(GLfloat r, float x, float y) {
		radio = r;
		xcord = x;
		ycord = y;
	};

	Pelota() {};

	void dibuja(GLfloat x, GLfloat y) {
		int i;
		int triangleAmount = 20;
		GLfloat twicePi = 2.0f * 3.1415;


		glPushMatrix();
			glBegin(GL_TRIANGLE_FAN);
			glColor3f(1,0,0);
			glVertex2f(x, y); // Centro del circulo
			for (i = 0; i <= triangleAmount; i++) {
				glVertex2f(
					x + (radio * cos(i *  twicePi / triangleAmount)),
					y + (radio * sin(i * twicePi / triangleAmount))
					);
			}
			glEnd();
		glPopMatrix();
	};

};


void changeViewport(int w, int h) {

	glViewport(0,0,w,h); // Cambia el aspecto de la pantalla.

	// Matriz de proyeccion.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();			// Identidad

	glOrtho(-10.5, 10.0, -10.0, 10.5, -1.0, 1.0);

};

class Bloque {
public:
	float* color;
	GLfloat ancho;
	GLfloat alto;
	int cantGolpes;
	GLfloat radioExplo;
	float xcord;
	float ycord;

	Bloque(float* c, GLfloat anch, GLfloat alt, int golpes, GLfloat explo, float x, float y) {
		color = c;
		ancho = anch;
		alto = alt;
		cantGolpes = golpes;
		radioExplo = explo;
		xcord = x;
		ycord = y;
	};
	Bloque() {};

	void dibuja(GLfloat x, GLfloat y) {
		glPushMatrix();
			glTranslatef(x,y,0);
			glBegin(GL_LINE_LOOP);
				glColor3f(color[0], color[1], color[2]);
				glVertex2f(ancho, 0.0);
				glVertex2f(ancho, -alto);
				glVertex2f(0.0, -alto);
				glVertex2f(0.0, 0.0);
			glEnd();
		glPopMatrix();
	};
};

Bloque bosque[7][5]; // Almacena la posicion de todos los bloques

void dibujaMundo(GLfloat ancho, GLfloat alto, float* color) {
	glPushMatrix();
		glBegin(GL_LINE_LOOP);
			// Marco interior
			glColor3f(color[0], color[1], color[2]);
			glVertex2f(-ancho/2, -alto/2);
			glVertex2f(-ancho/2, alto/2);
			glVertex2f(ancho/2, alto/2);
			glVertex2f(ancho/2, -alto/2);

			// Marco exterior
			glColor3f(color[0], color[1], color[2]);
			glVertex2f(-ancho/2-1, -alto/2-1);
			glVertex2f(-ancho/2-1, alto/2+1);
			glVertex2f(ancho/2+1, alto/2+1);
			glVertex2f(ancho/2+1, -alto/2-1);
		glEnd();

		// Se pintan los bloques
		GLfloat x = -anchoMundo/2+2 ;
		GLfloat y = altoMundo/2-1 ;

		int nbloquesV = 5;
		int nbloquesH = 7;
		float separacionH = .5;
		float separacionV = 1;

		for (int i = 0; i < nbloquesV; i++) {
			for (int j = 0; j < nbloquesH; j++) {
				Bloque bloque(morado,1.0,.5,0.0,0.0,x/2,y/2);
				bosque[i][j] = bloque;
				bloque.dibuja(x,y);
				x += bloque.ancho + separacionH;
			};
			x = -anchoMundo/2+2;
			y -= separacionV + bosque[0][0].alto;
		};
	glPopMatrix();

	// Dibuja base.
	Base base(anchoBase,altoBase);
	base.dibuja(-9.0);
	// Dibuja Pelota

	if (inicio == false) {
		veloHori = base.posicion()[0];
		veloVert = -8;
	}

	Pelota p(tamPelota,0.0, 0.0);
	p.dibuja(veloHori, veloVert);
};


void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	dibujaMundo(anchoMundo,altoMundo,amarillo);

	glutSwapBuffers();
};

void colision_base() {

	if ((despBase + (anchoBase / 2) <= veloHori) && (despBase - (anchoBase / 2) >= veloHori)) {
		printf("Esta en la base");
		if (-8.25 >= veloVert) {
			// Ejecutar el cambio de direccion
			printf("Golpeo en la base");
			cambioVertical = false;
		}
	}

}

void moverPelota() {
	if (inicio == true) {
		veloVertPasada = veloVert;
		veloHoriPasada = veloHori;
		GLfloat pendiente = 0;
		// Modelar un comportamiento predeterminado
		if (!cambioVertical)
			veloVert += 0.005;
		else
			veloVert -= 0.005;

		if (!cambioHorizontal)
			veloHori += 0.005;
		else
			veloHori -= 0.005;

		if ((veloHori - veloHoriPasada) > 0)
			pendiente = (veloVert - veloVertPasada) / (veloHori - veloHoriPasada);

		// Limites del juego
		if (veloVert >= altoMundo/2-tamPelota)
			cambioVertical = true;

		// Condicion: La pelota no cae en la base
		if (veloVert <= -(altoMundo/2-tamPelota)) {
			printf("Perdiste\n");
			cambioVertical = false;
		}

		if (veloHori >= anchoMundo/2-tamPelota)
			cambioHorizontal = true;
		if (veloHori <= -(anchoMundo/2-tamPelota)) {
			cambioHorizontal = false;
		}



		// Colision con la base
		if (( (despBase + (anchoBase / 2)) >= veloHori)  && ( (despBase - (anchoBase / 2)) <= veloHori)) {
			//printf("Esta en la base\n");
			if (-8.25 >= veloVert) {
				// Ejecutar el cambio de direccion
				printf("Golpeo en la base\n");
				cambioVertical = false;
				if (pendiente > 0)
					cambioHorizontal = false;

				if (pendiente < 0)
					cambioHorizontal = true;

			}
		}


		colision_base();
		//printf("Pelota (%f,%f)\n", veloHori, veloVert);
		render();
	}
}


void accion(unsigned char tecla, int x, int y) {

	/* Configurar los limites del espacio del juego */
	// Desplazamiento de la base


	switch (tecla) {
		case 'a':
			if (despBase > -anchoMundo/2+1.5) despBase -= .5;

		break;
		case 's':
			// Limite: MarcoInterior - tamBase/2. Configurar como macro
			if (despBase < anchoMundo/2-1.5) despBase += .5;
		break;
		case 'p':
			inicio = true;
			//moverPelota();
		break;
	}
	//printf("Desp de la base %f\n", despBase);

	render();

}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(800,600);

	glutCreateWindow("Opengl");

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(accion);
	glutIdleFunc(moverPelota);


	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	glutMainLoop();
	return 0;

}
