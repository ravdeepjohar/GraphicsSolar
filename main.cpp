//Alex Sokol, Ravdeep Johar, Will Washkuhn
// CS459 Final
// The Solar System

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h> 
#include <iostream>
#include <string>
#include <windows.h>

GLUquadric *sun1;
GLuint suntexture;
GLuint mercurytexture;
GLuint moontexture;
GLuint marstexture;
GLuint venustexture;
GLuint earthtexture;

using namespace std;
static int win;
static int menuExit;
static int menuT;
static int menuQ;
static int menuM;
static int val = 0;
float sred=1.0;
float sgreen=1.0;
float sblue=1.0;
float mred=0.2;
float mgreen=0.2;
float mblue=0.2;
void createMenu(void);
void menu(int value);

GLuint earthTex, mercuryTex, venusTex, sunTex;
void loadTexture (char *fileName, GLuint *texName, int w, int h, int d, GLenum colorType){
   GLubyte *bitmap ;
FILE *file;
file = fopen(fileName, "rb");
bitmap = (GLubyte *) malloc ( w * h * d * ( sizeof(GLubyte)) );
fread  (bitmap , w * h * d, 1 , file );
fclose (file);
glGenTextures(1, texName);
glBindTexture(GL_TEXTURE_2D, *texName);
glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, w, h, colorType, GL_UNSIGNED_BYTE, bitmap);
free (bitmap);
}

//Uses vector to define the movement of the planets.
class Vector {
	public:
		GLfloat dx, dy;
		GLfloat magnitude();
		Vector scale(GLfloat);
		Vector normalize();
		Vector add(Vector);
};


//Uses point to define the position of the planets.
class Point {
   public:
      GLfloat x, y;
	  GLfloat distance(Point);
		
};

//Planet are the objects displayed.
class Planet {
	public:
		std::string name;
		Point position;
		Vector velocity;
		GLfloat mass;
		void set(string, Point, Vector, GLfloat);
		Vector gravForceOn(Planet);
		void applyForce(Vector);
		void move();
};

//Returns the magniude of the vector.
GLfloat Vector::magnitude(){
	return sqrt(this->dx*this->dx + this->dy*this->dy);
}

//Returns a scaled vector, scaled to the input.
Vector Vector::scale(GLfloat s){
	Vector temp;
	temp.dx = this->dx*s;
	temp.dy = this->dy*s;
	return temp;
}

//Returns a normalized vector. 
Vector Vector::normalize(){
	Vector temp;
	temp.dx = this->dx;
	temp.dy = this->dy;
	return temp.scale(1.0/temp.magnitude());
}

//Returns the sum of the intput vector and the current vector.
Vector Vector::add(Vector other){
	Vector temp;
	temp.dx = this->dx + other.dx;
	temp.dy = this->dy + other.dy;
	return temp;
}

//Returns the distance between the current point and the input point.
GLfloat Point::distance (Point other){
	Vector test;
	test.dx = other.x - this->x;
	test.dy = other.y - this->y;
		return test.magnitude();
}

//Returns a vector of the force the input planet on the current planet.
Vector Planet::gravForceOn(Planet other){
	Vector temp;
	GLfloat dist = this->position.distance(other.position);
	temp.dx = this->position.x - other.position.x;
	temp.dy = this->position.y - other.position.y;
	return temp.normalize().scale(this->mass*other.mass/(dist*dist));
}

//Sets the fields of a Planet.
void Planet::set(std::string name, Point position, Vector velocity, GLfloat mass){
	this->name = name;
	this->position = position;
	this->velocity = velocity;
	this->mass = mass;
}

//Mutates the current velocity by adding the current velocity and the input force togther.
void Planet::applyForce(Vector force){
	this->velocity = this->velocity.add(force.scale(1.0/this->mass));
}

//Mutates the position of the Planet by adding the position with the velocity.
void Planet::move(){
	this->position.x += this->velocity.dx;
	this->position.y += this->velocity.dy;
	glTranslatef(this->position.x, this->position.y, 1.0);
}
Point position;
Vector velocity;
std::string name;
Planet sun, mercury, venus, earth, moon, mars;

bool fullscreen = false;
bool mouseDown = false;
bool rotate = true;
bool scale = false;
bool translate = false;
bool shift = false;
bool pause = true;

float xrot = 0.0f;
float yrot = 0.0f;
float zrot = 0.0f;
float zrot1 = 0.0f;
float zrot2 = 0.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;
float zdiff = 0.0f;
float ydiff1 = 0.0f;
float ydiff2 = 0.0f;

float xinit = 0.0f;
float yinit = 0.0f;
float zinit = 0.0f;
float yinit1 = 0.0f;
float yinit2 = 0.0f;

float xdelta = 1.0f;
float ydelta = 1.0f;
float zdelta = 1.0f;
float zdelta1 = 1.0f;
float zdelta2 = 1.0f;

float xchange = 0.0f;
float ychange = 0.0f;
float zchange = 0.0f;
float ychange1 = 0.0f;
float ychange2 = 0.0f;

float xtrans = 0.0f;
float ytrans = 0.0f;
float ztrans = 0.0f;
float ztrans1 = 0.0f;
float ztrans2 = 0.0f;

//Moves the planets by calculating the forces on each planet, applying the forces, and redrawing the planet.
void move() {
	if(pause == false){
	Vector force;
	
	force.dx = 0;
	force.dy = 0;

	force = force.add(mercury.gravForceOn(sun));
	force = force.add(venus.gravForceOn(sun));
	force = force.add(earth.gravForceOn(sun));
	force = force.add(moon.gravForceOn(sun));
	force = force.add(mars.gravForceOn(sun));
	sun.applyForce(force);

	force.dx = 0;
	force.dy = 0;

	force = force.add(sun.gravForceOn(mercury));
	force = force.add(venus.gravForceOn(mercury));
	force = force.add(earth.gravForceOn(mercury));
	force = force.add(moon.gravForceOn(mercury));
	force = force.add(mars.gravForceOn(mercury));
	mercury.applyForce(force);

	force.dx = 0;
	force.dy = 0;

	force = force.add(sun.gravForceOn(venus));
	force = force.add(mercury.gravForceOn(venus));
	force = force.add(earth.gravForceOn(venus));
	force = force.add(moon.gravForceOn(venus));
	force = force.add(mars.gravForceOn(venus));
	venus.applyForce(force);

	force.dx = 0;
	force.dy = 0;

	force = force.add(sun.gravForceOn(earth));
	force = force.add(mercury.gravForceOn(earth));
	force = force.add(venus.gravForceOn(earth));
	force = force.add(moon.gravForceOn(earth));
	force = force.add(mars.gravForceOn(earth));
	earth.applyForce(force);

	force.dx = 0;
	force.dy = 0;

	force = force.add(sun.gravForceOn(moon));
	force = force.add(mercury.gravForceOn(moon));
	force = force.add(venus.gravForceOn(moon));
	force = force.add(earth.gravForceOn(moon));
	force = force.add(mars.gravForceOn(moon));
	moon.applyForce(force);

	force.dx = 0;
	force.dy = 0;

	force = force.add(sun.gravForceOn(mars));
	force = force.add(mercury.gravForceOn(mars));
	force = force.add(venus.gravForceOn(mars));
	force = force.add(earth.gravForceOn(mars));
	force = force.add(moon.gravForceOn(mars));
	mars.applyForce(force);

	sun.move();
	mercury.move();
	venus.move();
	earth.move();
	moon.move();
	mars.move();
	glutPostRedisplay();
	}

}

bool init (void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	return true;
}

void drawPlanets()
{
	glClear (GL_COLOR_BUFFER_BIT);
	if(val == 1){
		sred=0.4;
	    sgreen=0;
	    sblue=0;
	}
	else if(val == 2){
		sred=0;
	    sgreen=0;
	    sblue=0.4;
	}
	else if(val == 3){
		sred=0;
	    sgreen=0.4;
	    sblue=0;
	}
	else if(val == 4){
		sred=0.4;
	    sgreen=0.4;
	    sblue=0;
	}
	else if(val == 5){
		sred=0.4;
	    sgreen=0.4;
	    sblue=0.4;
	}
	else if(val == 6){
		sred=0;
	    sgreen=0;
	    sblue=0;
	}
	if(val == 7){
		mred=0.2;
	    mgreen=0;
	    mblue=0;
	}
	else if(val == 8){
		mred=0;
	    mgreen=0;
	    mblue=0.2;
	}
	else if(val == 9){
		mred=0;
	    mgreen=0.2;
	    mblue=0;
	}
	else if(val == 10){
		mred=0.2;
	    mgreen=0.2;
	    mblue=0;
	}
	else if(val == 11){
		mred=0.2;
	    mgreen=0.2;
	    mblue=0.2;
	}
	else if(val == 12){
		mred=0;
	    mgreen=0;
	    mblue=0;
	}
GLfloat ambient[4]	= { 0.1f , 0.1f, 0.1f, 0.1f };
GLfloat diffuse[4]	= { sred, sgreen, sblue, 0.8f };
GLfloat specular[4]	= { 0.5f, 0.5f, 0.5f, 0.5f };
GLfloat position[4]	= { sun.position.x, sun.position.y, 1.0f, 1.0f };
GLfloat ambient1[4]	= { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat diffuse1[4]	= { mred, mgreen, mblue, 0.2f };
GLfloat specular1[4]	= { 0.2f, 0.2f, 0.2f, 0.5f };
GLfloat position1[4]	= { moon.position.x, moon.position.y, 1.0f, 1.0f };
GLfloat specref[4]	= { 1.0f, 1.0f, 1.0f, 1.0f };
glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 200);

	loadTexture( "suntex.bmp", &(suntexture), 200, 100, 3, GL_BGR_EXT);
	loadTexture ( "earthtexsmall.bmp", &(earthtexture), 256, 128, 3, GL_BGR_EXT);
	loadTexture ( "marstexsmall.bmp", &(marstexture), 256, 128, 3, GL_BGR_EXT);
	loadTexture ( "venustexsmall.bmp", &(venustexture), 256, 128, 3, GL_BGR_EXT);
	loadTexture ( "moontexsmall.bmp", &(moontexture), 200, 100, 3, GL_BGR_EXT);
	loadTexture ( "mercurytexsmall.bmp", &(mercurytexture), 256, 128, 3, GL_BGR_EXT);
	
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,suntexture);
	GLUquadricObj*  s = gluNewQuadric ( );
    gluQuadricDrawStyle ( s, GLU_FILL );
    gluQuadricNormals ( s, GLU_SMOOTH );
    gluQuadricTexture ( s, GL_TRUE    );
	glColor3f (1.0, 1.0, 1.0); 
	glTranslatef(sun.position.x, sun.position.y, 1.0);
	gluSphere ( s, sqrt(sun.mass)*6, 50, 50 );
	gluDeleteQuadric ( s );
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,mercurytexture);
	GLUquadricObj*  s2 = gluNewQuadric ( );
    gluQuadricDrawStyle ( s2, GLU_FILL );
    gluQuadricNormals ( s2, GLU_SMOOTH );
    gluQuadricTexture ( s2, GL_TRUE    );
	glColor3f (1.0, 1.0, 1.0); 
	glTranslatef(mercury.position.x, mercury.position.y, 1.0);
	gluSphere ( s2, sqrt(mercury.mass)*6, 50, 50 );
	gluDeleteQuadric ( s2 );
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,venustexture);
	GLUquadricObj*  s3 = gluNewQuadric ( );
    gluQuadricDrawStyle ( s3, GLU_FILL );
    gluQuadricNormals ( s3, GLU_SMOOTH );
    gluQuadricTexture ( s3, GL_TRUE    );
	glPushMatrix();
	glColor3f (1.0, 1.0, 1.0); 
	glTranslatef(venus.position.x, venus.position.y, 1.0);
	gluSphere ( s3, sqrt(venus.mass)*6, 50, 50 );
	gluDeleteQuadric ( s3 );
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
    
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,earthtexture);
	GLUquadricObj*  s1 = gluNewQuadric ( );
    gluQuadricDrawStyle ( s1, GLU_FILL );
    gluQuadricNormals ( s1, GLU_SMOOTH );
    gluQuadricTexture ( s1, GL_TRUE    );
	glPushMatrix();
	glColor3f (1.0, 1.0, 1.0); 
	glTranslatef(earth.position.x, earth.position.y, 1.0);
	gluSphere ( s1, sqrt(earth.mass)*6, 50, 50 );
	gluDeleteQuadric ( s1 );
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,moontexture);
	GLUquadricObj*  s5 = gluNewQuadric ( );
    gluQuadricDrawStyle ( s5, GLU_FILL );
    gluQuadricNormals ( s5, GLU_SMOOTH );
    gluQuadricTexture ( s5, GL_TRUE    );
	glPushMatrix();
	glColor3f (1.0, 1.0, 1.0); 
	glTranslatef(moon.position.x, moon.position.y, 1.0);
	gluSphere ( s5, sqrt(moon.mass)*6, 50, 50 );
	gluDeleteQuadric ( s5 );
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
    
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,marstexture);
	GLUquadricObj*  s4 = gluNewQuadric ( );
    gluQuadricDrawStyle ( s4, GLU_FILL );
    gluQuadricNormals ( s4, GLU_SMOOTH );
    gluQuadricTexture ( s4, GL_TRUE    );
	glPushMatrix();
	glColor3f (1.0, 1.0, 1.0); 
	glTranslatef(mars.position.x, mars.position.y, 1.0);
	gluSphere ( s4, sqrt(mars.mass)*6, 50, 50 );
	gluDeleteQuadric ( s4 );
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glFlush ( );     
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(
		0.0f, 0.0f, 1000.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);

	glScalef(xdelta, ydelta, zdelta);

	glTranslatef(xtrans, ytrans, ztrans);

	drawPlanets();

	glFlush();
	glutSwapBuffers();
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45.0f, 1.0f * w / h, 1.0f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27 : 
			exit(1); break;

		case 'r' :
			rotate = true;
			scale = false;
			translate = false;
			shift = false; break;

		case 'R' :
			rotate = true;
			scale = false;
			translate = false;
			shift = true; break;

		case 's' :
			rotate = false;
			scale = true;
			translate = false; 
			shift = false; break;

		case 'S' :
			rotate = false;
			scale = true;
			translate = false; 
			shift = true; break;

		case 't' :
			rotate = false;
			scale = false;
			translate = true;
			shift = false; break;

		case 'T' :
			rotate = false;
			scale = false;
			translate = true;
			shift = true; break;

		case 'p' :
		case 'P' :
			if (pause == true){
				pause = false;
			}
			else {
				pause = true;
			}
			break;

		case 'x' :
		case 'X' :
			xrot = 0.0f;
			yrot = 0.0f;
			zrot = 0.0f;	
			xdelta = 1.0f;
			ydelta = 1.0f;
			zdelta = 1.0f;
			xtrans = 0.0f;
			ytrans = 0.0f;
			ztrans = 0.0f;
			glutPostRedisplay();			
			break;
	}
}

void specialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else
		{
			glutReshapeWindow(1000, 1000);
			glutPositionWindow(0, 0);
		}
	}
}

void mouse(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
		if (rotate == true)
		{
			//Rotates on all three axes when moving the mouse in the x direction, or on the z axis when moving the mouse
			//in the y direction.
			if (shift == true)
			{
				xdiff = x - xrot;
				ydiff1 = x - yrot;
				zdiff = x - zrot1;

				ydiff2 = -y + zrot2;
				
				ydiff = ydiff1 + ydiff2;
				zrot = zrot1 + zrot2;
			}
			//Rotates on the x axis when moving the mouse in the x direction, or on the y axis when moving the mouse
			//in the y direction.
			else 
			{
				xdiff = x - yrot;
				ydiff = -y + xrot;
			}
		}
		if (scale == true)
		{
			//Scales on all the axes when moving the mouse in the x direction, or on the z axis when moving the mouse
			//in the y direction.
			if (shift == true)
			{
				xinit = x * xdelta;
				yinit1 = x * ydelta;
				zinit = x * zdelta1;
				
				yinit2 = y * zdelta2;
				yinit = yinit1 * yinit2;
				zdelta = zdelta1 * zdelta2;
			} 
			//Scales on the x axis when moving the mouse in the x direction, or on the y axis when moving the mouse
			//in the y direction.
			else {
				xinit = x * xdelta;
				yinit = y * ydelta;
			}
		}
		if (translate == true)
		{
			//Translates on all the axes when moving the mouse in the x direction, or on the z axis when moving the mouse
			//in the y direction.
			if (shift == true)
			{
				xchange = x + xtrans;
				ychange1 = x + ytrans;
				zchange = x + ztrans1;

				ychange2 = y + ztrans2;
				ychange = ychange1 + ychange2;
				ztrans = ztrans1 + ztrans2;
			}
			//Translates on the x axis when moving the mouse in the x direction, or on the y axis when moving the mouse
			//in the y direction.
			else{
				xchange = x + xtrans;
				ychange = y + ytrans;
			}
		}
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		if (rotate == true)
		{
			//Rotates on all three axes when moving the mouse in the x direction, or on the z axis when moving the mouse
			//in the y direction.
			if (shift == true)
			{				
				xrot = x - xdiff;
				yrot = x - ydiff1;
				zrot1 = x - zdiff;

				zrot2 = y + ydiff2;
				ydiff = ydiff1 + ydiff2;
				zrot = zrot1 + zrot2;
			}
			//Rotates on the x axis when moving the mouse in the x direction, or on the y axis when moving the mouse
			//in the y direction.
			else {
				yrot = x - xdiff;
				xrot = y + ydiff;
			}
		}
		if (scale == true)
		{
			//Scales on all the axes when moving the mouse in the x direction, or on the z axis when moving the mouse
			//in the y direction.
			if (shift == true)
			{
				xdelta = xinit/x;
				ydelta = yinit1/x;
				zdelta1 = zinit/x;

				zdelta2 = yinit2/y;
				yinit = yinit1 * yinit2;
				zdelta = zdelta1 * zdelta2;
			}
			//Scales on the x axis when moving the mouse in the x direction, or on the y axis when moving the mouse
			//in the y direction.
			else{
				xdelta = xinit/x;
				ydelta = yinit/y;
			}
		}
		if (translate == true)
		{
			//Translates on all the axes when moving the mouse in the x direction, or on the z axis when moving the mouse
			//in the y direction.
			if (shift == true)
			{
				xtrans = -x + xchange;
				ytrans = -x + ychange1;
				ztrans1 = -x + zchange;

				ztrans2 = -y + ychange2;
				ychange = ychange1 + ychange2;
				ztrans = ztrans1 + ztrans2;
			}
			//Translates on the x axis when moving the mouse in the x direction, or on the y axis when moving the mouse
			//in the y direction.
			else {
				xtrans = -x + xchange;
				ytrans = -y + ychange;
			}

		}
		glutPostRedisplay();
	}
}



int main (int argc, char** argv)
{
	
	position.x = 0.0;
	position.y = 0.0;
	velocity.dx = 0.01;
	velocity.dy = 0.01;
	sun.set("Sun", position, velocity, 100.0);
	
	position.x = 0.0;
	position.y = -100.0;
	velocity.dx = 1.0;
	velocity.dy = 0.0;
	mercury.set("Mercury", position, velocity, 1.0);

	position.x = -200.0;
	position.y = 0.0;
	velocity.dx = 0.0;
	velocity.dy = -0.75;
	venus.set("Venus", position, velocity, 4.0);

	position.x = 0.0;
	position.y = 300.0;
	velocity.dx = -0.6;
	velocity.dy = 0.01;
	earth.set("Earth", position, velocity, 4.0);

	position.x = 30.0;
	position.y = 300.0;
	velocity.dx = -0.6;
	velocity.dy = 0.41;
	moon.set("Moon", position, velocity, 1.0);

	position.x = 400.0;
	position.y = 0.0;
	velocity.dx = 0.01;
	velocity.dy = 0.54;
	mars.set("Mars", position, velocity, 1.0);
	

	glutInit (&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);   // Set display mode.
	glutInitWindowPosition (0, 0);   // Set top-left display-window position.
	glutInitWindowSize (1000, 1000);      // Set display-window width and height.
	glutCreateWindow ("Solar System"); // Create display window.

	init ( );                            // Execute initialization procedure.
	glutDisplayFunc (display);
	createMenu();
	glutSpecialFunc(specialKeyboard) ;
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);
	glutIdleFunc(move);
	if (!init())
		return 1;
 
	glutMainLoop ( );                    // Display everything and wait.



}
void createMenu(void){
	//////////
	// MENU //
	//////////
	
	
	
	menuQ = glutCreateMenu(menu);
	
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue",2);
	glutAddMenuEntry("Green",3);
	glutAddMenuEntry("Voilet",4);
	glutAddMenuEntry("Default",5);	
    glutAddMenuEntry("OFF",6);
	menuExit = glutCreateMenu(menu);
	
	menuM = glutCreateMenu(menu);
	
	glutAddMenuEntry("Red", 7);
	glutAddMenuEntry("Blue",8);
	glutAddMenuEntry("Green",9);
	glutAddMenuEntry("Voilet",10);
	glutAddMenuEntry("Default",11);	
    glutAddMenuEntry("OFF",12);
	menuExit = glutCreateMenu(menu);
	// Create an entry
	
	glutAddSubMenu("Sun Color", menuQ);
	glutAddSubMenu("Moon Color", menuM);
	glutAddMenuEntry("Exit", 0);
	
	// Let the menu respond on the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	
}

void menu(int value){
	if(value == 0){
		exit(0);
	}else{
		val=value;
	}
	
	// you would want to redraw now
	glutPostRedisplay();
}