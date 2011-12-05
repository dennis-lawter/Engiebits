#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "NGB/ngb.h"

int frames;
int timer;
NGBkeyListener* keys;

void testUpdate(NGBuint dt) {
	frames++;
	timer += dt;
	while (timer >= 1000) {
		printf("FPS: %i\n", frames);
		timer -= 1000;
		frames = 0;
	}
}

int main(int argc, char** argv) {
	ngbInit(&argc, argv, NGB_FALSE);
	ngbInitWindowCentered("Test", 1280, 720);
	keys = ngbInitKeyListener(NGB_TRUE);
	ngbInitGraphics();
	ngbUpdateFunc(testUpdate);
	frames = 0;
	timer = 0;
	ngbMainLoop();
	return EXIT_FAILURE;
}
/*
int main(int argc, char** argv){
	ngbInit(&argc, argv, NGB_FALSE);
	NGBHT* hashTable = ngbHT_create();
	ngbHT_insert(hashTable, "urmom", "This is a printable string.\n");
	ngbHT_insert(hashTable, "urface", "This is also a printable string.\n");
	printf(ngbHT_get(hashTable, "urface"));
	printf(ngbHT_get(hashTable, "urmom"));
	return EXIT_SUCCESS;
}
*/
