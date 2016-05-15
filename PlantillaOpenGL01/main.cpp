#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>


using namespace std;

#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	10.0f
#define DEF_floorGridZSteps	10.0f

// Vectores de movimiento de la pelota
GLfloat veloHori = 0.0, veloVert = -4.0;
GLfloat despBase = 0.0, amplitudBase = 3.0;

GLfloat posxInicial = -7.5, posyInicial = 8.0;
bool cambioVertical = false;
bool cambioHorizontal = true;
bool visto = false;

// Posicion de los bloques
GLfloat posBloquesy[6];
GLfloat posBloquesx[6];


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

void dibujarGrid() {
	GLfloat zExtent, xExtent, xLocal, zLocal;
	int loopX, loopZ;

	/* Render Grid */
	glPushMatrix();
	glColor3f(0.0f, 0.7f, 0.7f);
	glBegin(GL_LINES);
	zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
	for (loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++)
	{
		xLocal = DEF_floorGridScale * loopX;
		glVertex3f(xLocal, -zExtent, 0.0f);
		glVertex3f(xLocal, zExtent, 0.0f);
	}
	xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
	for (loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++)
	{
		zLocal = DEF_floorGridScale * loopZ;
		glVertex3f(-xExtent, zLocal, 0.0f);
		glVertex3f(xExtent, zLocal, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

							 //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * 3.1415;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
			);
	}
	glEnd();
}


void changeViewport(int w, int h) {
	glViewport(0,0,w,h);
	glOrtho(-10.5, 10.0, -10.0, 10.5, -1.0, 1.0);
	//float aspectratio = (float)w / (float)h;

	/*if (w <= h)
		glViewport(0.0, 0.0, w/aspectratio,h/aspectratio);
	else
		glViewport(0.0, 0.0, aspectratio*w, aspectratio*h);*/

}

void dibujarBase(GLfloat tam) {
	// Crea una base sobre el origen. Centrada sobre el eje x y por encima.
	GLfloat amplitud = tam / 2;
	glBegin(GL_POLYGON);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(amplitud, 0.0);
		glVertex2f(amplitud, 0.5);
		glVertex2f(-amplitud, 0.5);
		glVertex2f(-amplitud, 0.0);
	glEnd();
}


void render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		ejesCoordenada(3.0);
		//glLineWidth(3.0);
		glTranslatef(-7.5, 8.0, 0.0);
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				//ejesCoordenada(1.0);
				// Almacenar la posiciones de cada bloque que se dibuje

				glPointSize(5.0);
				glBegin(GL_POINTS);
					glVertex2f(0.0, 0.0);
				glEnd();
				glBegin(GL_LINE_LOOP);
					glColor3f(0.3, 0.0, 0.3);
					glVertex2f(1.0, -0.25);
					glVertex2f(1.0, 0.25);
					glVertex2f(-1.0, 0.25);
					glVertex2f(-1.0, -0.25);
				glEnd();
				glTranslatef(3.0, 0.0, 0.0);
			}
			glTranslatef(-18.0, -1.0, 0.0);

		}
		
	glPopMatrix();

	// glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
	// Crear marco del juego
	glPushMatrix();
		glBegin(GL_LINE_LOOP);
			// Marco interior
			glColor3f(0.6, 0.6, 0.0);
			glVertex2f(-9.0, -9.0);
			glVertex2f(-9.0, 9.0);
			glVertex2f(9.0, 9.0);
			glVertex2f(9.0, -9.0);

			// Marco exterior
			glVertex2f(10.0, -9.0);
			glVertex2f(10.0, 10.0);
			glVertex2f(-10.0, 10.0);
			glVertex2f(-10.0, -9.0);
		glEnd();
	glPopMatrix();


	// Pelota
	glPushMatrix();
		glTranslatef(veloHori, veloVert, 0.0);
		drawFilledCircle(0.0, 0.0, .25);
	glPopMatrix();

	// Dibujar base.
	glPushMatrix();
		glTranslatef(despBase, -9.0, 0.0);
		dibujarBase(amplitudBase);
	glPopMatrix();

	glutSwapBuffers();
}



void accion(unsigned char tecla, int x, int y) {

	/* Configurar los limites del espacio del juego */
	// Desplazamiento de la base


	switch (tecla) {
		case 'a':
			if (despBase > -7.5) despBase -= .2;
			break;
		case 's':
			// Limite: MarcoInterior - tamBase/2. Configurar como macro
			if (despBase < 7.5) despBase += .2;
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
	//printf("Desp de la base %f\n", despBase);

	render();

}

void colision_base() {
	if ((-8.5 >= veloVert - .25) && (-8.5 <= veloVert)) {
		if (((despBase + (amplitudBase / 2)) >= veloHori - .25) && ((despBase + (amplitudBase / 2)) <= veloHori)) {
			// Ejecutar el cambio de direccion
			printf("Golpeo en la esquina de la base\n");
			cambioHorizontal = true;
			cambioVertical = true;
		}
		if (((despBase - (amplitudBase / 2)) <= veloHori + .25) && ((despBase - (amplitudBase / 2)) >= veloHori)) {
			// Ejecutar el cambio de direccion
			printf("Golpeo en la esquina de la base\n");
			cambioHorizontal = false;
			cambioVertical = true;
		}
	}


	if (((despBase + (amplitudBase / 2)) >= veloHori) && ((despBase - (amplitudBase / 2)) <= veloHori)) {
		if (-8.5 >= veloVert - .25) {
			// Ejecutar el cambio de direccion
			printf("Golpeo en la base\n");
			cambioVertical = false;
		}
	}
}


void colisionBloques() {
	// posicion central de cada bloque + distancia al borde + radio de la pelota
	
	for (size_t i = 0; i < 6; i++) {
		// Colision general
		for (size_t j = 0; j < 6; j++) {
			if (((posBloquesy[i] + 0.25) >= veloVert) && (veloVert >= (posBloquesy[i] - .25))) {
				if ((veloHori + .25 >= (posBloquesx[j] - 1.0)) && ((posBloquesx[j] + 1.0) >= veloHori - .25)) {
					printf("Colision con bloque lado horizontal (%f,%f)\n", posBloquesx[j], posBloquesy[i]);
					// Rebote entre bloques
					cambioHorizontal = true;
				}
				
			}

			// Colisiones verticales
			if ((veloHori >= (posBloquesx[j] - 1.0)) && ((posBloquesx[j] + 1.0) >= veloHori)) {
				if (((posBloquesy[i] + 0.25) >= veloVert - .25) && (veloVert + .25 >= (posBloquesy[i] - .25))) {
					printf("Colision con bloque lado vertical (%f,%f)\n", posBloquesx[j], posBloquesy[i]);
					cambioVertical = true;
				}
			}
			// Prueba de esquinas inferiores
			if (((posBloquesy[i] - .25) <= veloVert + .25) && ((posBloquesy[i] - .25) >= veloVert)) {
				if ((veloHori - .25 <= (posBloquesx[j] + 1.0)) && (veloHori >= (posBloquesx[j] + 1.0))) {
					printf("Colision en esquina der,inf con bloque (%f,%f)\n", posBloquesx[j], posBloquesy[i]);
					cambioVertical = true;
					cambioHorizontal = true;
				}
				if ((veloHori + .25 >= (posBloquesx[j] - 1.0)) && (veloHori <= (posBloquesx[j] - 1.0))) {
					printf("Colision esquina izq,inf con bloque (%f,%f)\n", posBloquesx[j], posBloquesy[i]);
					cambioVertical = true;
					cambioHorizontal = false;
				}
			}

			// Prueba de esquinas superiores
			if (((posBloquesy[i] + .25) >= veloVert - .25) && ((posBloquesy[i] + .25) <= veloVert)) {
				if ((veloHori - .25 <= (posBloquesx[j] + 1.0)) && (veloHori >= (posBloquesx[j] + 1.0))) {
					printf("Colision en esquina der,sup con bloque (%f,%f)\n", posBloquesx[j], posBloquesy[i]);
					cambioVertical = false;
					cambioHorizontal = true;
				}
				if ((veloHori + .25 >= (posBloquesx[j] - 1.0)) && (veloHori <= (posBloquesx[j] - 1.0))) {
					printf("Colision esquina izq,sup con bloque (%f,%f)\n", posBloquesx[j], posBloquesy[i]);
					cambioVertical = false;
					cambioHorizontal = false;
				}
			}
			// Esquinas superiores
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
	if (veloVert >= 8.75)
		cambioVertical = true;

	// Condicion: La pelota no cae en la base
	if (veloVert <= -9.75) {
		printf("Perdiste\n");
		cambioVertical = false;
	}

	if (veloHori >= 8.75)
		cambioHorizontal = true;

	if (veloHori <= -8.75)
		cambioHorizontal = false;

	colisionBloques();
	colision_base();
	//printf("Pelota (%f,%f)\n", veloHori, veloVert);
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

	// Posiciones del centro de los bloques 
	for (size_t i = 0; i < 6; i++)
	{
		posBloquesx[i] = posxInicial + i*3.0;
		posBloquesy[i] = posyInicial - i*1.0;

	}


	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}


	glutMainLoop();
	return 0;

}