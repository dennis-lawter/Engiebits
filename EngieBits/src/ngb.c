#include "NGB/ngb.h"

#include <unistd.h>

#include <stdlib.h>
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

/*
 * ----------------------------------------------------------------
 * Simple data structures provided by engine
 * ----------------------------------------------------------------
 */

//Linked List
NGBLL* ngbLL_create(void) {
	NGBLL* list = malloc(sizeof(NGBLL));
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}
void ngbLL_insertFront(NGBLL* list, void* newContent) {
	NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
	newNode->content = newContent;
	newNode->next = list->head;
	newNode->last = NULL;
	if (list->size > 0) {
		list->head->last = newNode;
		list->head = newNode;
	} else {
		list->head = newNode;
		list->tail = newNode;
	}
	list->size++;
}
void ngbLL_insertBack(NGBLL* list, void* newContent) {
	NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
	newNode->content = newContent;
	newNode->next = NULL;
	newNode->last = list->tail;
	if (list->size > 0) {
		list->tail->next = newNode;
		list->tail = newNode;
	} else {
		list->head = newNode;
		list->tail = newNode;
	}
	list->size++;
}
void* ngbLL_removeFront(NGBLL* list) {
	if (list->size > 0) {
		NGBLL_NODE* resultNode;
		void* result;
		resultNode = list->head;
		result = resultNode->content;

		if (list->size > 1) {
			list->head = list->head->next;
			list->head->last = NULL;
		} else {
			list->head = NULL;
			list->tail = NULL;
		}
		list->size--;

		free(resultNode);
		return result;
	} else {
		return NULL;
	}
}
void* ngbLL_removeBack(NGBLL* list) {
	if (list->size > 0) {
		NGBLL_NODE* resultNode;
		void* result;
		resultNode = list->tail;
		result = resultNode->content;

		if (list->size > 1) {
			list->tail = resultNode->last;
			list->tail->next = NULL;
		} else {
			list->head = NULL;
			list->tail = NULL;
		}
		list->size--;

		free(resultNode);
		return result;
	} else {
		return NULL;
	}
}
void* ngbLL_peakFront(NGBLL* list) {
	return list->head->content;
}
void* ngbLL_peakBack(NGBLL* list) {
	return list->tail->content;
}
void ngbLL_insertAt(NGBLL* list, NGBuint key, void* newContent) {
	if (key == 0) {
		ngbLL_insertFront(list, newContent);
	} else if (key == list->size) {
		ngbLL_insertBack(list, newContent);
	} else {
		NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
		newNode->content = newContent;

		NGBLL_NODE* newNext = list->head;
		int i;
		if (key > list->size / 2) {
			for (i = list->size; i > key + 1; i++) {
				newNext = newNext->last;
			}
		} else {
			for (i = 0; i < key; i++) {
				newNext = newNext->next;
			}
		}
		NGBLL_NODE* newLast = newNext->last;

		newNode->last = newLast;
		newNode->next = newNext;
		newLast->next = newNode;
		newNext->last = newNode;

		list->size++;
	}
}
void* ngbLL_removeAt(NGBLL* list, NGBuint key) {
	//TODO
	return NULL;
}
void* ngbLL_peakAt(NGBLL* list, NGBuint key) {
	//TODO
	return NULL;
}
void ngbLL_toArray(NGBLL* list, void* array, int* size) {
	//TODO
}
void ngbLL_destroy(NGBLL* list) {
	//TODO
}

//Dynamic Array

NGBDA* ngbDA_create(void) {
	//TODO
	return NULL;
}
void ngbDA_set(NGBDA* dynarr, NGBuint key, void* value) {
	//TODO
}
void* ngbDA_get(NGBDA* dynarr, NGBuint key) {
	//TODO
	return NULL;
}
void ngbDA_destroy(NGBDA* dynarr) {
	//TODO
}

//Associative Array

NGBAA* ngbAA_create(void) {
	//TODO
	return NULL;
}
void ngbAA_set(NGBAA* assarr, char* key, void* value) {
	//TODO
}
void* ngbAA_get(NGBAA* assarr, char* key) {
	//TODO
	return NULL;
}
void ngbAA_destroy(NGBAA* assarr) {
	//TODO
}

//Hash Table

NGBHT* ngbHT_create(void) {
	//TODO
	return NULL;
}
void ngbHT_insert(char* key, void* data) {
	//TODO
}
void* ngbHT_get(char* key) {
	//TODO
	return NULL;
}
char ngbHT_hash(char* key) {
	//TODO
	return '\0';
}
void ngbHT_destroy(void) {
	//TODO
}

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

void _ngbKeyDown(unsigned char key, int x, int y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
}

void _ngbKeyUp(unsigned char key, int x, int y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
}

void _ngbSpecDown(int key, int x, int y) {
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

void _ngbSpecUp(int key, int x, int y) {
	usleep(100);
	if (keyListener == NULL) {
		return;
	}
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
