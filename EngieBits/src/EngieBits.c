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
	NGBprofile* profile = ngbCreateProfile();
	ngbSetProfile(profile);
	ngbInit(&argc, argv);
	ngbInitWindowCentered("Test");
	keys = ngbInitKeyListener(NGB_TRUE);
	ngbInitGraphics();
	ngbUpdateFunc(testUpdate);
	frames = 0;
	timer = 0;
	ngbMainLoop();
	return EXIT_FAILURE;
}
