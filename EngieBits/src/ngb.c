/*
 * ----------------------------------------------------------------
 * Engiebits main engine
 * ----------------------------------------------------------------
 */

#include "NGB/ngb.h"
#include "NGB/ngb_internal.h"

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

/*
 * ----------------------------------------------------------------
 * Internal data
 * ----------------------------------------------------------------
 */

NGBkeyListener* keyListener = NULL;
NGBdrawable2D* gameObjs2D = NULL;
NGBdrawable3D* gameObjs3D = NULL;
NGBcamera* camera = NULL;

NGBboolean _doubleBuffering;
int _differentialTime;
void (*_updateFunction)(NGBuint) = NULL;
unsigned long _timer1, _timer2;

/*
 * ----------------------------------------------------------------
 * Main engine procedures
 * ----------------------------------------------------------------
 */

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

void _ngbReshape(NGBint w, NGBint h) {
	//TODO
}

void _ngbKeyDown(unsigned char key, NGBint x, NGBint y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
}

void _ngbKeyUp(unsigned char key, NGBint x, NGBint y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
}

void _ngbSpecDown(NGBint key, NGBint x, NGBint y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
	if (keyListener->altf4 && key == GLUT_KEY_F4
			&& (glutGetModifiers() & GLUT_ACTIVE_ALT)) {
		exit(EXIT_SUCCESS);
	} else {

	}
}

void _ngbSpecUp(NGBint key, NGBint x, NGBint y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
}

void ngbInit(int* argc, char** argv, NGBboolean doubleBuffer) {
	glutInit(argc, argv);
	alutInit(argc, argv);

	srand(time(NULL));
	_ngbHT_hashInit();

	_doubleBuffering = doubleBuffer;

	int bitmask = GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA;
	if (doubleBuffer) {
		bitmask = bitmask | GLUT_DOUBLE;
	} else {
		bitmask = bitmask | GLUT_SINGLE;
	}

	glutInitDisplayMode(bitmask);
}

int ngbInitWindowCentered(char* title, NGBuint w, NGBuint h) {
	int x, y;

	x = (glutGet(GLUT_SCREEN_WIDTH) - w) / 2;
	y = (glutGet(GLUT_SCREEN_HEIGHT) - h) / 2;

	return ngbInitWindowAtPosition(title, x, y, w, h);
}

int ngbInitWindowAtPosition(char* title, NGBuint x, NGBuint y, NGBuint w, NGBuint h) {
	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);
	return glutCreateWindow(title);
}

void ngbInitGraphics(void) {
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

NGBkeyListener* ngbInitKeyListener(NGBboolean altf4) {
	keyListener = malloc(sizeof(NGBkeyListener));
	keyListener->altf4 = altf4;
	int i;
	for (i = 0; i < 512; i++) {
		keyListener->keys[i] = NGB_FALSE;
	}
	glutKeyboardFunc(&_ngbKeyDown);
	glutKeyboardUpFunc(&_ngbKeyUp);
	glutSpecialFunc(&_ngbSpecDown);
	glutSpecialUpFunc(&_ngbSpecUp);
	return keyListener;
}

void ngbUpdateFunc(void(*func)(NGBuint)) {
	_updateFunction = func;
}

void ngbMainLoop(void) {
	_timer1 = glutGet(GLUT_ELAPSED_TIME);
	glutDisplayFunc(&_ngbDraw);
	glutIdleFunc(&_ngbDraw);
	glutReshapeFunc(&_ngbReshape);
	glutMainLoop();
}