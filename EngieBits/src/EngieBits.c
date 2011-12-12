#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "NGB/ngb.h"

//----------test suite includes---------
#include "tests/test_suite.h";
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
	ngbInitWindowCentered("Test");
	keys = ngbInitKeyListener(NGB_TRUE);
	ngbInitGraphics();
	ngbUpdateFunc(testUpdate);
	frames = 0;
	timer = 0;

	//-------test suite--------------
	test_suite_add_test(NGBHT_test);
	test_suite_add_test(NGBLL_test);
	test_suite();
	//-------------------------------
	ngbMainLoop();
	return EXIT_FAILURE;
}
