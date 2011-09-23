#include "NGB/ngb.h"

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

/*
 * ----------------------------------------------------------------
 * Simple data structures provided by engine
 * ----------------------------------------------------------------
 */

//Linked List



//Dynamic Array



//Associative Array



//Hash Table



/*
 * ----------------------------------------------------------------
 * Main engine
 * ----------------------------------------------------------------
 */

NGBboolean _doubleBuffering;
int _differentialTime;
void (*_updateFunction)(int) = NULL;
unsigned long _timer1, _timer2;

void _ngbDraw(void) {
	_timer2 = glutGet(GLUT_ELAPSED_TIME);
	_differentialTime = _timer2 - _timer1;
	_timer1 = glutGet(GLUT_ELAPSED_TIME);

	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0);
	glClearColor(1, 0, 0, 1);

	if (_updateFunction != NULL) {
		_updateFunction(_differentialTime);
	}

	if (_doubleBuffering) {
		glutSwapBuffers();
	} else {
		glutPostRedisplay();
	}
}

void _ngbReshape(int w, int h) {

}

void _ngbKeyDown(char key, int x, int y) {

}

void _ngbKeyUp(char key, int x, int y) {

}

void _ngbSpecDown(int key, int x, int y) {

}

void _ngbSpecUp(int key, int x, int y) {

}

void ngbInit(int* argc, char** argv, NGBboolean doubleBuffer) {
	glutInit(argc, argv);
	alutInit(argc, argv);

	_doubleBuffering = doubleBuffer;

	int bitmask = GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA;
	if (doubleBuffer) {
		bitmask = bitmask | GLUT_DOUBLE;
	} else {
		bitmask = bitmask | GLUT_SINGLE;
	}

	glutInitDisplayMode(bitmask);
}

int ngbInitWindowCentered(char* title, int w, int h) {
	int x, y;

	x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;

	return ngbInitWindowAtPosition(title, x, y, w, h);
}

int ngbInitWindowAtPosition(char* title, int x, int y, int w, int h) {
	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);
	return glutCreateWindow(title);
}

void ngbGraphicsInit(void) {
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		exit(EXIT_FAILURE);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ngbUpdateFunc(void(*func)(int)) {
	_updateFunction = func;
}

void ngbMainLoop(void) {
	_timer1 = glutGet(GLUT_ELAPSED_TIME);
	glutDisplayFunc(&_ngbDraw);
	glutIdleFunc(&_ngbDraw);
	glutReshapeFunc(&_ngbReshape);
	glutMainLoop();
}
