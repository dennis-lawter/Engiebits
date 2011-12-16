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

struct Image{
	/*
	 * only local struct right now add prototype to header if needed outside.
	 */
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
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
Image* ngbLoadRAWImage(char* fileName, int w, int h) {
	Image* image = (Image *) malloc(sizeof(Image));
	FILE* file;
	unsigned long size;

	size = w * h * 4;

	image->sizeX = w;
	image->sizeY = h;

	file = fopen(fileName, "rb");
	if (file == NULL)
		return NULL;

	image->data = (char *) malloc(size);

	fread(image->data, size, 1, file);
	fclose(file);

	return image;
}

GLuint* _ngbLoadTextures(char** fileNames, int* widths, int* heights, int num) {
	GLuint* texture = (GLuint*) malloc(num * sizeof(GLuint));
	Image** image;
	image = (Image**) malloc(num * sizeof(Image**));
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
			glTexImage2D(GL_TEXTURE_2D, 0, 4, image[i]->sizeX, image[i]->sizeY,
					0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
		}

		free(image[i]);
	}

	free(image);
	return texture;
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
