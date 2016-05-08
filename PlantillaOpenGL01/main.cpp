#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>


using namespace std;

#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	10.0f
#define DEF_floorGridZSteps	10.0f

// Vectores de movimiento de la pelota
GLfloat veloHori = 0.0, veloVert = 5.0;
GLfloat veloHoriPasada = 0.0, veloVertPasada = 0.0;
GLfloat despBase = 0.0, amplitudBase = 3.0;
bool cambioVertical = false;
bool cambioHorizontal = false;


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
		//glLineWidth(3.0);
		glTranslatef(-7.5, 8.0, 0.0);
		for (size_t i = 0; i < 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				//ejesCoordenada(1.0);
				glBegin(GL_LINE_LOOP);
					glColor3f(0.3, 0.0, 0.3);
					glVertex2f(1.0, 0.0);
					glVertex2f(1.0, 0.5);
					glVertex2f(-1.0, 0.5);
					glVertex2f(-1.0, 0.0);
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
		// Punto de prueba
		/*glPointSize(5.0);
		glBegin(GL_POINTS);
			glVertex2f(0.0, -8.0);
		glEnd();*/
		drawFilledCircle(0.0, 0.0, .25);
	glPopMatrix();

	// Dibujar base. 
	glPushMatrix();
		glTranslatef(despBase, -9.0, 0.0);
		ejesCoordenada(1.0);
		dibujarBase(amplitudBase);
	glPopMatrix();

	glutSwapBuffers();
}



void accion(unsigned char tecla, int x, int y) {

	/* Configurar los limites del espacio del juego */
	// Desplazamiento de la base

	
	switch (tecla) {
		case 'a':
			if (despBase > -7.5) despBase -= .1;
			break;
		case 's':
			// Limite: MarcoInterior - tamBase/2. Configurar como macro
			if (despBase < 7.5) despBase += .1;
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

	if ((despBase + (amplitudBase / 2) <= veloHori) && (despBase - (amplitudBase / 2) >= veloHori)) {
		printf("Esta en la base");
		if (-8.25 >= veloVert) {
			// Ejecutar el cambio de direccion 
			printf("Golpeo en la base");
			cambioVertical = false;
		}
	}

}

void moverPelota() {
	
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
	if (veloVert >= 8.75)
		cambioVertical = true;

	// Condicion: La pelota no cae en la base
	if (veloVert <= -9.75) {
		printf("Perdiste\n");
		cambioVertical = false;
	}

	if (veloHori >= 8.75)
		cambioHorizontal = true;
	if (veloHori <= -8.75) {
		cambioHorizontal = false;
	}
	


	// Colision con la base
	if (( (despBase + (amplitudBase / 2)) >= veloHori)  && ( (despBase - (amplitudBase / 2)) <= veloHori)) {
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


	//colision_base();
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


	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}