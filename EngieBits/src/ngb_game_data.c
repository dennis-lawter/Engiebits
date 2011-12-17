/*
 * ----------------------------------------------------------------
 * Game data functions
 * ----------------------------------------------------------------
 */

#include "NGB/ngb.h"
#include "NGB/ngb_internal.h"

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

NGBdrawable2D** _ngbGameObjs2D = NULL;
NGBdrawable3D** _ngbGameObjs3D = NULL;

typedef struct Image Image;

char* _ngbLoadFile(char* filename) {
	FILE* file;
	long size;
	char* data;

	if ((file = fopen(filename, "rb")) == NULL) {
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	data = malloc(sizeof(char) * size);
	if (data == NULL) {
		return NULL;
	}

	if (fread(data, 1, size, file) != size) {
		return NULL;
	}

	return data;
}

void _ngbDrawAll2D(void) {
	int i;
	if (_ngbGameObjs2D != NULL) {
		for (i = 0; _ngbGameObjs2D[i] != NULL; i++) {
			ngbDraw2D(_ngbGameObjs2D[i]);
		}
	}
}
void _ngbDrawAll3D(void) {
	int i;
	if (_ngbGameObjs3D != NULL) {
		for (i = 0; _ngbGameObjs3D[i] != NULL; i++) {
			ngbDraw3D(_ngbGameObjs3D[i]);
		}
	}
}

void ngbSet2DDrawList(NGBdrawable2D** drawables) {
	_ngbGameObjs2D = drawables;
}
void ngbSet3DDrawList(NGBdrawable3D** drawables) {
	_ngbGameObjs3D = drawables;
}
void ngbDraw2D(NGBdrawable2D* drawable) {
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, drawable->spriteTexture);

	glTranslated(drawable->origin.x, drawable->origin.y, 0);
	glBegin(GL_POLYGON);
	{
		glVertex2d(0, 0);
		glVertex2d(drawable->width, 0);
		glVertex2d(drawable->width, drawable->height);
		glVertex2d(0, drawable->height);
	}
	glEnd();

	glPopMatrix();
}
void ngbDraw3D(NGBdrawable3D* drawable) {
	glPushMatrix();

	glPopMatrix();
}
