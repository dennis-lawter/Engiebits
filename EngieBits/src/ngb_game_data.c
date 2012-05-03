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

NGBrawImage* ngbLoadRAWImage(char* fileName, int w, int h) {
	char* data = _ngbLoadFile(fileName);
	if (data == NULL) {
		return NULL;
	}
	NGBrawImage* image = (NGBrawImage *) malloc(sizeof(NGBrawImage));

	image->width = w;
	image->height = h;
	image->data = data;

	return image;
}

NGBuint* ngbLoadTextures(char** fileNames, NGBuint* widths, NGBuint* heights,
		NGBuint num) {
	NGBuint* texture = (NGBuint*) malloc(num * sizeof(NGBuint));
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

	glColor3d(1, 1, 1);
	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1);
		glVertex2d(0, 0);
		glTexCoord2d(1, 1);
		glVertex2d(drawable->width, 0);
		glTexCoord2d(1, 0);
		glVertex2d(drawable->width, drawable->height);
		glTexCoord2d(0, 0);
		glVertex2d(0, drawable->height);
	}
	glEnd();

	glPopMatrix();
}
void ngbDraw3D(NGBdrawable3D* drawable) {
	glPushMatrix();

	glPopMatrix();
}
