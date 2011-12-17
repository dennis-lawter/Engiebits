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

/*
 * ----------------------------------------------------------------
 * Internal data
 * ----------------------------------------------------------------
 */

NGBkeyListener* _ngbKeyListener = NULL;
NGBcamera* _ngbCamera = NULL;
NGBprofile* _ngbProfile = NULL;
NGBcoordinateSystem* _ngbCoordinates = NULL;
NGBint _differentialTime;
void (*_updateFunction)(NGBuint) = NULL;
unsigned long _timer1, _timer2;
NGBvector* _ngbWindowMetrics = NULL;

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

	glColor3f(1, 1, 1);

	_ngbDrawAll2D();
	_ngbDrawAll3D();

	if (_ngbProfile->doubleBuffer) {
		glutSwapBuffers();
	} else {
		glutPostRedisplay();
		glFlush();
	}
}

void _ngbReshape(NGBint w, NGBint h) {
	if (_ngbWindowMetrics == NULL) {
		_ngbWindowMetrics = malloc(sizeof(NGBvector));
		_ngbWindowMetrics->z = 0;
	}
	_ngbWindowMetrics->x = w;
	_ngbWindowMetrics->y = h;
	if (h == 0)
		h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (_ngbCamera != NULL && _ngbCamera->threeDimensional) {
		gluPerspective(_ngbCamera->fieldOfView, (GLfloat) w / (GLfloat) h,
				0.001f, 100.0f);
	}
	if (_ngbCoordinates) {
		glOrtho(_ngbCoordinates->minimum.x, _ngbCoordinates->maximum.x,
				_ngbCoordinates->minimum.y, _ngbCoordinates->maximum.y,
				_ngbCoordinates->minimum.z, _ngbCoordinates->maximum.z);
	}
	glMatrixMode(GL_MODELVIEW);
}

void _ngbKeyDown(unsigned char key, NGBint x, NGBint y) {
	usleep(100);
	if (_ngbKeyListener == NULL) {
		return;
	}
}

void _ngbKeyUp(unsigned char key, NGBint x, NGBint y) {
	usleep(100);
	if (_ngbKeyListener == NULL) {
		return;
	}
}

void _ngbSpecDown(NGBint key, NGBint x, NGBint y) {
	usleep(100);
	if (_ngbKeyListener == NULL) {
		return;
	}
	if (_ngbKeyListener->altf4 && key == GLUT_KEY_F4
			&& (glutGetModifiers() & GLUT_ACTIVE_ALT)) {
		exit(EXIT_SUCCESS);
	} else {

	}
}

void _ngbSpecUp(NGBint key, NGBint x, NGBint y) {
	usleep(100);
	if (_ngbKeyListener == NULL) {
		return;
	}
}

NGBprofile* ngbCreateProfile(void) {
	NGBprofile* newProfile = malloc(sizeof(NGBprofile));
	newProfile->resolutionX = 640;
	newProfile->resolutionY = 480;
	newProfile->colorDepth = 32;
	newProfile->fullscreen = NGB_FALSE;
	newProfile->doubleBuffer = NGB_FALSE;

	newProfile->mouseSensitivityX = 3.0;
	newProfile->mouseSensitivityY = 3.0;

	newProfile->masterVolume = 1.0;
	newProfile->musicVolume = 1.0;
	newProfile->entityVolume = 1.0;
	newProfile->ambiantVolume = 1.0;

	return newProfile;
}

NGBcamera* ngbCreateCamera(void) {
	NGBcamera* newCamera = malloc(sizeof(NGBcamera));

	newCamera->fieldOfView = 90;
	newCamera->position.x = 0;
	newCamera->position.y = 0;
	newCamera->position.z = 0;
	newCamera->rotation.x = 0;
	newCamera->rotation.y = 0;
	newCamera->rotation.z = 0;
	newCamera->threeDimensional = NGB_FALSE;

	return newCamera;
}

void ngbSetProfile(NGBprofile* newProfile) {
	_ngbProfile = newProfile;
}

void ngbSetCoordinateSystem(NGBcoordinateSystem* coord) {
	_ngbCoordinates = coord;
	if (_ngbWindowMetrics != NULL) {
		_ngbReshape(_ngbWindowMetrics->x, _ngbWindowMetrics->y);
	}
}

void ngbInit(int* argc, char** argv) {

	if (_ngbProfile == NULL) {
		exit(1);
	}

	glutInit(argc, argv);
	alutInit(argc, argv);

	srand(time(NULL));
	_ngbHT_hashInit();

	int bitmask = GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA;
	if (_ngbProfile->doubleBuffer) {
		bitmask = bitmask | GLUT_DOUBLE;
	} else {
		bitmask = bitmask | GLUT_SINGLE;
	}

	glutInitDisplayMode(bitmask);
}

int ngbInitWindowCentered(char* title) {
	int x, y;

	x = (glutGet(GLUT_SCREEN_WIDTH) - _ngbProfile->resolutionX) / 2;
	y = (glutGet(GLUT_SCREEN_HEIGHT) - _ngbProfile->resolutionY) / 2;

	return ngbInitWindowAtPosition(title, x, y);
}

int ngbInitWindowAtPosition(char* title, NGBuint x, NGBuint y) {
	glutInitWindowPosition(x, y);
	glutInitWindowSize(_ngbProfile->resolutionX, _ngbProfile->resolutionY);
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
	_ngbKeyListener = malloc(sizeof(NGBkeyListener));
	_ngbKeyListener->altf4 = altf4;
	int i;
	for (i = 0; i < 512; i++) {
		_ngbKeyListener->keys[i] = NGB_FALSE;
	}
	glutKeyboardFunc(&_ngbKeyDown);
	glutKeyboardUpFunc(&_ngbKeyUp);
	glutSpecialFunc(&_ngbSpecDown);
	glutSpecialUpFunc(&_ngbSpecUp);
	return _ngbKeyListener;
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

NGBrawImage* ngbLoadRAWImage(char* fileName, int w, int h) {
	NGBrawImage* image = (NGBrawImage *) malloc(sizeof(NGBrawImage));
	char* file;
	unsigned long size;

	size = w * h * 4;

	image->width = w;
	image->height = h;

	file = fopen(fileName, "rb");
	if (file == NULL)
		return NULL;

	image->data = (char *) malloc(size);

	fread(image->data, size, 1, file);
	fclose(file);

	return image;
}

GLuint* ngbLoadTextures(char** fileNames, int* widths, int* heights, int num) {
	GLuint* texture = (GLuint*) malloc(num * sizeof(GLuint));
	NGBrawImage** image;
	image = (NGBrawImage**) malloc(num * sizeof(NGBrawImage**));
	int i;

	for (i = 0; i < num; i++) {
		image[i] = ngbLoadRAWImage(fileNames[i], widths[i], heights[i]);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(num, texture);

	for (i = 0; i < num; i++) {
		if (image[i] == NULL) {
			printf("ERROR");
		} else {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_DECAL);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, image[i]->width, image[i]->height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
		}

		free(image[i]);
	}

	free(image);
	return texture;
}

