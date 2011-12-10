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

void ngbDraw2D(NGBdrawable2D* drawable) {
	glPushMatrix();

	glPopMatrix();
}
void ngbDraw3D(NGBdrawable3D* drawable) {
	glPushMatrix();

	glPopMatrix();
}
