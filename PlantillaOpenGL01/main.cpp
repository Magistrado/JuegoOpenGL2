#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>


using namespace std;

#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	10.0f
#define DEF_floorGridZSteps	10.0f

/////////////////// Marco
GLfloat anchoMundo = 14;
GLfloat altoMundo = 18;
// Cantidad de bloques 

int nbloquesV = 5;
int nbloquesH = 7;

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
			glTranslatef(x, y, 0);
			glColor3f(color[0], color[1], color[2]);
			glPointSize(4.0);
			glBegin(GL_POINTS);
				glVertex2f(0.0, 0.0);
			glEnd();
			glBegin(GL_LINE_LOOP);
				/*glVertex2f(ancho, 0.0);
				glVertex2f(ancho, -alto);
				glVertex2f(0.0, -alto);
				glVertex2f(0.0, 0.0);*/
				glVertex2f(ancho, alto);
				glVertex2f(ancho, -alto);
				glVertex2f(-ancho, -alto);
				glVertex2f(-ancho, alto);
				
			glEnd();
		glPopMatrix();
	};
};

Bloque bosque[5][7]; // Almacena la posicion de todos los bloques

void dibujaMundo(GLfloat ancho, GLfloat alto, float* color) {
	ejesCoordenada(3.0);
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
		GLfloat x = -anchoMundo/2+2.5;
		GLfloat y = altoMundo/2-1 ;

		float separacionH = 1.0;
		float separacionV = 1;

		for (int i = 0; i < nbloquesV; i++) {
			for (int j = 0; j < nbloquesH; j++) {
				Bloque bloque(morado,.5,.25,0.0,0.0,x,y);
				bosque[i][j] = bloque;
				bloque.dibuja(x,y);
				x += bloque.ancho + separacionH;
			};
			x = -anchoMundo/2+2.5;
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
	if ((-8.5 >= veloVert - tamPelota) && (-8.5 <= veloVert)) {
		// Colisiones de esquina
		if (((despBase + (anchoBase / 2)) >= veloHori - tamPelota) && ((despBase + (anchoBase/ 2)) <= veloHori)) {
			// Ejecutar el cambio de direccion
			printf("Golpeo en la esquina de la base\n");
			cambioHorizontal = true;
			cambioVertical = true;
		}
		if (((despBase - (anchoBase / 2)) <= veloHori + tamPelota) && ((despBase - (anchoBase/ 2)) >= veloHori)) {
			
			printf("Golpeo en la esquina de la base\n");
			cambioHorizontal = false;
			cambioVertical = true;
		}
	}

	// Colisiones vertical
	if (((despBase + (anchoBase / 2)) >= veloHori) && ((despBase - (anchoBase / 2)) <= veloHori)) {
		if (-8.5 >= veloVert - tamPelota) {
			printf("Golpeo en la base\n");
			cambioVertical = false;
		}
	}
}


void colisionBloques() {
	// posicion central de cada bloque + distancia al borde + radio de la pelota
	
	for (int i = 0; i < nbloquesV; i++) {
		for (int j = 0; j < nbloquesH ; j++) {
			// Colision horizontales
			if (((bosque[i][j].ycord + bosque[i][j].alto) >= veloVert) && (veloVert >= (bosque[i][j].ycord - bosque[i][j].alto))) {
				if ((veloHori + tamPelota >= (bosque[i][j].xcord - bosque[i][j].ancho)) 
						&& ((bosque[i][j].xcord + bosque[i][j].ancho) >= veloHori - tamPelota)) {
					printf("Colision con bloque lado horizontal (%f,%f)\n", bosque[i][j].xcord, bosque[i][j].ycord);
					// Rebote entre bloques
					cambioHorizontal = true;
				}
			}

			// Colisiones verticales
			if ((veloHori >= (bosque[i][j].xcord - bosque[i][j].ancho)) && ((bosque[i][j].xcord + bosque[i][j].ancho) >= veloHori)) {
				if (((bosque[i][j].ycord + bosque[i][j].alto) >= veloVert - tamPelota)
					&& (veloVert + tamPelota >= (bosque[i][j].ycord - bosque[i][j].alto))) {
					printf("Colision con bloque lado vertical (%f,%f)\n", bosque[i][j].xcord, bosque[i][j].ycord);
					cambioVertical = true;
				}
			}
			// Prueba de esquinas inferiores
			if (((bosque[i][j].ycord- bosque[i][j].alto) <= veloVert + tamPelota) && ((bosque[i][j].ycord - bosque[i][j].alto) >= veloVert)) {
				if ((veloHori - tamPelota <= (bosque[i][j].xcord + bosque[i][j].ancho)) && (veloHori >= (bosque[i][j].xcord + bosque[i][j].ancho))) {
					printf("Colision en esquina der,inf con bloque (%f,%f)\n", bosque[i][j].xcord, bosque[i][j].ycord);
					cambioVertical = true;
					cambioHorizontal = true;
				}
				if ((veloHori + tamPelota >= (bosque[i][j].xcord - bosque[i][j].ancho)) && (veloHori <= (bosque[i][j].xcord - bosque[i][j].ancho))) {
					printf("Colision esquina izq,inf con bloque (%f,%f)\n", bosque[i][j].xcord, bosque[i][j].ycord);
					cambioVertical = true;
					cambioHorizontal = false;
				}
			}

			// Prueba de esquinas superiores
			if (((bosque[i][j].ycord + bosque[i][j].alto) >= veloVert - tamPelota) && ((bosque[i][j].ycord + bosque[i][j].alto) <= veloVert)) {
				if ((veloHori - tamPelota <= (bosque[i][j].xcord + bosque[i][j].ancho)) && (veloHori >= (bosque[i][j].xcord + bosque[i][j].ancho))) {
					printf("Colision en esquina der,sup con bloque (%f,%f)\n", bosque[i][j].xcord, bosque[i][j].ycord);
					cambioVertical = false;
					cambioHorizontal = true;
				}
				if ((veloHori + tamPelota >= (bosque[i][j].xcord - bosque[i][j].ancho)) && (veloHori <= (bosque[i][j].xcord - bosque[i][j].ancho))) {
					printf("Colision esquina izq,sup con bloque (%f,%f)\n", bosque[i][j].xcord, bosque[i][j].ycord);
					cambioVertical = false;
					cambioHorizontal = false;
				}
			}
		}
	}
}

void moverPelota() {
	// Modelar un comportamiento predeterminado
	if (!cambioVertical)
		//veloVert += 0.1;
		veloVert += 0.005;
	else
		//veloVert -= 0.1;
		veloVert -= 0.005;

	if (!cambioHorizontal)
		veloHori += 0.005;
		//veloHori += 0.1;
	else
	//	veloHori -= 0.1;
		veloHori -= 0.005;

	// Limites del juego
	if (veloVert >= altoMundo / 2 - tamPelota)
		 cambioVertical = true;
	
	// Condicion: La pelota no cae en la base
	if (veloVert <= -(altoMundo / 2 - tamPelota)) {
		printf("Perdiste\n");
		cambioVertical = false;
	}

	if (veloHori >= anchoMundo / 2 - tamPelota)
		cambioHorizontal = true;
	if (veloHori <= -(anchoMundo / 2 - tamPelota)) {
		cambioHorizontal = false;
		
	}

	colisionBloques();
	colision_base();
	//printf("Pelota (%f,%f)\n", veloHori, veloVert);
	render();

}


void accion(unsigned char tecla, int x, int y) {

	/* Configurar los limites del espacio del juego */
	// Desplazamiento de la base


	switch (tecla) {
	case 'a':
		if (despBase > -anchoMundo / 2 + 1.5) despBase -= .5;
		break;
	case 's':
		// Limite: MarcoInterior - tamBase/2. Configurar como macro
		if (despBase < anchoMundo / 2 - 1.5) despBase += .5;
		break;
	case 'p':
		inicio = true;
		break;
		// Desplazamiento de pelota (Prueba)
	case 'j':
		veloHori -= 0.1;
		break;
	case 'l':
		veloHori += 0.1;
		break;
	case 'i':
		veloVert += 0.1;
		break;
	case 'k':
		veloVert -= 0.1;
		break;
	}
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
