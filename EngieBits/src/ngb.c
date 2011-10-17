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
NGBLL_NODE* _ngbLL_find(NGBLL* list, NGBuint key) {
	NGBLL_NODE* findNode;
	int i;
	if (key > list->size / 2) {
		findNode = list->tail;
		for (i = list->size; i > key + 1; i--) {
			findNode = findNode->last;
		}
	} else {
		findNode = list->head;
		for (i = 0; i < key; i++) {
			findNode = findNode->next;
		}
	}
	return findNode;
}
void ngbLL_insertAt(NGBLL* list, NGBuint key, void* newContent) {
	if (key == 0) {
		ngbLL_insertFront(list, newContent);
	} else if (key == list->size) {
		ngbLL_insertBack(list, newContent);
	} else {
		NGBLL_NODE* newNode = malloc(sizeof(NGBLL_NODE));
		newNode->content = newContent;

		NGBLL_NODE* newLast = _ngbLL_find(list, key-1);
		NGBLL_NODE* newNext = newLast->next;

		newNode->last = newLast;
		newNode->next = newNext;

		newLast->next = newNode;
		newNext->last = newNode;

		list->size++;
	}
}
void* ngbLL_removeAt(NGBLL* list, NGBuint key) {
	if (list->size < 1) {
		return NULL;
	}
	if (key == 0) {
		return ngbLL_removeFront(list);
	} else if (key == list->size - 1) {
		return ngbLL_removeBack(list);
	} else {
		NGBLL_NODE* findNode = _ngbLL_find(list, key);
		findNode->last->next = findNode->next;
		findNode->next->last = findNode->last;
		void* content = findNode->content;
		free(findNode);
		return content;
	}
}
void* ngbLL_peakAt(NGBLL* list, NGBuint key) {
	if (list->size < 1) {
		return NULL;
	} else if (key == 0) {
		return ngbLL_peakFront(list);
	} else if (key == list->size - 1) {
		return ngbLL_peakBack(list);
	}
	NGBLL_NODE* findNode = _ngbLL_find(list, key);
	return findNode->content;
}
void** ngbLL_toArray(NGBLL* list) {
	if (list->size > 0) {
		void** result = malloc(list->size * sizeof(void*));
		NGBLL_NODE* current = list->head;
		int i;
		for (i = 0; current != NULL; i++) {
			result[i] = current->content;
			current = current->next;
		}
		return result;
	} else {
		return NULL;
	}
}
void ngbLL_destroy(NGBLL* list) {
	while (list->size > 0) {
		ngbLL_removeFront(list);
	}
	free(list);
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
