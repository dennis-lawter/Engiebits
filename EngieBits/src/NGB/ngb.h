/*
 * EngieBits (NGB) Video Game Engine
 * Version: 0.1
 *
 * Copyright (c) 2011 Dennis Lawter. All rights reserved.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * DENNIS LAWTER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef NGB_H_
#define NGB_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <sqlite3.h>
#include <stdio.h>

/*
 * ----------------------------------------------------------------
 * Basic type definitions
 * ----------------------------------------------------------------
 */

#define NGB_TRUE 1
#define NGB_FALSE 0

typedef int NGBboolean;
typedef unsigned int NGBuint;
typedef int NGBint;
typedef float NGBfloat;
typedef double NGBdouble;
typedef sqlite3 NGBSQL;

/*
 * ----------------------------------------------------------------
 * Simple data structures provided by engine
 * ----------------------------------------------------------------
 */

//Deletable node
struct NGB_NODE {
	void* content;
	void(*destroy)(void);
}typedef NGB_NODE;

//Linked List
struct NGBLL_NODE {
	struct NGB_NODE* content;
	struct NGBLL_NODE* next;
	struct NGBLL_NODE* last;
}typedef NGBLL_NODE;

struct NGBLL {
	struct NGBLL_NODE *head;
	struct NGBLL_NODE *tail;
	NGBuint size;
}typedef NGBLL;

NGBLL* ngbLL_create(void);
void ngbLL_insertFront(NGBLL* list, void* newhead);
void ngbLL_insertBack(NGBLL* list, void* newtail);
void* ngbLL_removeFront(NGBLL* list);
void* ngbLL_removeBack(NGBLL* list);
void* ngbLL_peakFront(NGBLL* list);
void* ngbLL_peakBack(NGBLL* list);
void ngbLL_insertAt(NGBLL* list, NGBuint key, void* newnode);
void* ngbLL_removeAt(NGBLL* list, NGBuint key);
void* ngbLL_peakAt(NGBLL* list, NGBuint key);
void** ngbLL_toArray(NGBLL* list);
void ngbLL_destroy(NGBLL* list);

//Dynamic Array

struct NGBDA {
	struct NGB_NODE *data;
	NGBuint size;
	NGBuint capacity;
}typedef NGBDA;

NGBDA* ngbDA_create(void);
void ngbDA_set(NGBDA* dynarr, NGBuint key, void* value);
void* ngbDA_get(NGBDA* dynarr, NGBuint key);
void** ngbDA_toArray(NGBDA* dynarr);
void ngbDA_destroy(NGBDA* dynarr);

//Associative Array

struct NGBAA {
	struct NGBDA* keys;
	struct NGBDA* values;
}typedef NGBAA;

NGBAA* ngbAA_create(void);
void ngbAA_set(NGBAA* assarr, char* key, void* value);
void* ngbAA_get(NGBAA* assarr, char* key);
void ngbAA_destroy(NGBAA* assarr);

//Hash Table

struct NGBHT {
	struct NGBLL** buckets;
}typedef NGBHT;

NGBHT* ngbHT_create(void);
void ngbHT_insert(NGBHT* table, char* key, void* data);
void* ngbHT_get(NGBHT* table, char* key);
unsigned char ngbHT_hash(char* key);
void ngbHT_destroy(NGBHT* table);

/*
 * ----------------------------------------------------------------
 * Game data
 * ----------------------------------------------------------------
 */

struct NGBkeyListener {
	NGBboolean altf4;
	NGBboolean keys[512];
}typedef NGBkeyListener;

struct NGBvertex {
	NGBdouble x;
	NGBdouble y;
	NGBdouble z;
}typedef NGBvertex;
typedef NGBvertex NGBvector;
typedef NGBvertex NGBrotation;
typedef NGBvertex NGBpoint;

struct NGBcoordinateSystem {
	NGBvertex minimum;
	NGBvertex maximum;
}typedef NGBcoordinateSystem;

struct NGBray {
	NGBpoint start;
	NGBvector unitVector;
}typedef NGBray;

struct NGBhitBox {
	NGBpoint origin;
	NGBpoint oppositeCorner;
}typedef NGBhitBox;

struct NGBcamera {
	NGBpoint position;
	NGBrotation rotation;
	NGBboolean threeDimensional;
	NGBuint fieldOfView;
}typedef NGBcamera;

struct NGB2DphysicsObject {
	NGBpoint position;
	NGBpoint centroid;
	NGBboolean isSolid;
	NGBdouble width;
	NGBdouble height;
	NGBdouble depth;
	NGBdouble mass;
	NGBdouble coefficientOfFriction;
	NGBvector velocity;
	NGBvector acceleration;
	NGBvector jerk;
	NGBuint* bitMask;
}typedef NGB2DphysicsObject;

struct NGBrawImage {
	char* data;
	NGBuint width;
	NGBuint height;
}typedef NGBrawImage;

struct NGBdrawable2D {
	NGBpoint origin;
	NGBdouble width;
	NGBdouble height;
	NGBuint spriteTexture;
	NGBboolean isVisible;
	NGB2DphysicsObject physics;
}typedef NGBdrawable2D;

struct NGBdrawable3D {
	NGBpoint origin;
	NGBuint vertexBuffer;
	NGBuint numBoxes;
}typedef NGBdrawable3D;

NGBrawImage* ngbLoadRAWImage(char* fileName, int w, int h);
NGBuint* ngbLoadTextures(char** fileNames, NGBuint* widths, NGBuint* heights,
		NGBuint num);
NGBAA* ngbTexturesToAA(char** keys, NGBuint* textures, NGBuint num);
void ngbSet2DDrawList(NGBdrawable2D** drawables);
void ngbSet3DDrawList(NGBdrawable3D** drawables);
void ngbDraw2D(NGBdrawable2D* drawable);
void ngbDraw3D(NGBdrawable3D* drawable);

/*
 * ----------------------------------------------------------------
 * Game database
 * ----------------------------------------------------------------
 */

struct NGBlog {
	char* fileName;
	char* logStamp;
	FILE* file;
	NGBboolean timeStamp;
	NGBboolean openHeader;
}typedef NGBlog;
NGBlog* NGBlog_init(char* fileName, char* logStamp, NGBboolean timeStamp,
		NGBboolean openHeader);
void NGBlog_write(NGBlog* log, char* message);
void NGBlog_newLine(NGBlog* log);
void NGBlog_lineBreak(NGBlog* log);
void NGBlog_destroy(NGBlog* log);

NGBSQL* NGBSQL_init(void);
NGBboolean NGBSQL_updateQuery(NGBSQL* database, char* query);
NGBAA** NGBSQL_resultQuery(NGBSQL* database, char* query, int* sizeReturned);
void NGBSQL_destroy(NGBSQL* database);

/*
 * ----------------------------------------------------------------
 * Engiebits main engine
 * ----------------------------------------------------------------
 */

struct NGBprofile {
	NGBuint resolutionX;
	NGBuint resolutionY;
	NGBuint colorDepth;
	NGBboolean fullscreen;

	NGBuint modelDetail;
	NGBuint textureDetail;
	NGBuint shaderDetail;
	NGBuint reflectionDetail;
	NGBuint shadowDetail;
	NGBuint antialiasingMode;
	NGBuint filteringMode;
	NGBboolean motionBlur;
	NGBboolean doubleBuffer;
	NGBboolean colorCorrection;

	NGBdouble mouseSensitivityX;
	NGBdouble mouseSensitivityY;

	NGBdouble masterVolume;
	NGBdouble musicVolume;
	NGBdouble entityVolume;
	NGBdouble ambiantVolume;
}typedef NGBprofile;
NGBprofile* ngbCreateProfile(void);

void ngbSetProfile(NGBprofile* newProfile);
void ngbSetCoordinateSystem(NGBcoordinateSystem* coord);
void ngbInit(int* argc, char** argv);
int ngbInitWindowCentered(char* title);
int ngbInitWindowAtPosition(char* title, NGBuint x, NGBuint y);
void ngbInitGraphics(void);
NGBkeyListener* ngbInitKeyListener(NGBboolean altf4);
void ngbUpdateFunc(void(*func)(NGBuint));

void ngbMainLoop(void);

#endif
