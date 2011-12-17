#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "NGB/ngb.h"

//----------test suite includes---------
#include "tests/test_suite.h"
#include "tests/NGBHT_test.h"
#include "tests/NGBLL_test.h"
//--------------------------------------

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
	/*
	 NGBdatabase* db = ngbSQL_Init();
	 ngbSQL_UpdateQuery(db, "CREATE TABLE test (id int, login varchar(255), password varchar(255))");
	 int temp;
	 ngbSQL_ResultQuery(db, "SELECT * FROM test", &temp);
	 */
	NGBprofile* profile = ngbCreateProfile();
	ngbSetProfile(profile);
	ngbInit(&argc, argv);

	NGBdrawable2D** tests = malloc(2 * sizeof(NGBdrawable2D*));
	tests[0] = malloc(sizeof(NGBdrawable2D));
	tests[0]->origin.x = -0.5;
	tests[0]->origin.y = -0.5;
	tests[0]->width = 1;
	tests[0]->height = 1;

	// texture test------------
	int num = 1;
	char** fileNames = (char**) malloc(num * sizeof(char*));
	NGBuint* widths = (NGBuint*) malloc(num * sizeof(int));
	NGBuint* heights = (NGBuint*) malloc(num * sizeof(int));

	fileNames[0] = "data/Fugue.raw";
	widths[0] = 32;
	heights[0] = 64;
	//fileNames[1] = "data/Ground.raw";
	//widths[1] = 32;
	//heights[1] = 32;
	NGBuint* textures;
	textures = ngbLoadTextures(fileNames, widths, heights, num);
	printf("%i\n", textures[0]);
	tests[0]->spriteTexture = textures[0];
	//-------------------------------------------------------------

	tests[1] = NULL;
	ngbSet2DDrawList(tests);

	ngbInitWindowCentered("Test");
	keys = ngbInitKeyListener(NGB_TRUE);
	ngbInitGraphics();
	ngbUpdateFunc(testUpdate);
	frames = 0;
	timer = 0;

	//-------test suite--------------
	test_suite_add_test(NGBHT_test);
	test_suite_add_test(NGBLL_test);
	test_suite_execute_tests();
	//-------------------------------
	ngbMainLoop();
	return EXIT_FAILURE;
}
