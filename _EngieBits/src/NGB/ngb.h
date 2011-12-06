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

/*
 * ----------------------------------------------------------------
 * Simple data structures provided by engine
 * ----------------------------------------------------------------
 */

//Linked List
struct NGBLL_NODE {
	void *content;
	struct NGBLL_NODE *next;
	struct NGBLL_NODE *last;
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
	void* data;
	NGBuint size;
	NGBuint capacity;
}typedef NGBDA;

NGBDA* ngbDA_create(void);
void ngbDA_set(NGBDA* dynarr, NGBuint key, void* value);
void* ngbDA_get(NGBDA* dynarr, NGBuint key);
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
 * Main engine
 * ----------------------------------------------------------------
 */

struct NGBprofile {
	NGBuint resolutionX;
	NGBuint resolutionY;
	NGBuint colorDepth;
	NGBboolean fullscreen;

	NGBdouble mouseSensitivityX;
	NGBdouble mouseSensitivityY;

	NGBdouble masterVolume;
	NGBdouble musicVolume;
	NGBdouble entityVolume;
	NGBdouble ambiantVolume;
}typedef NGBprofile;

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

void ngbDraw2D(NGBdrawable2D* drawable);
void ngbDraw3D(NGBdrawable3D* drawable);

void ngbInit(int* argc, char** argv, NGBboolean doubleBuffer);
int ngbInitWindowCentered(char* title, NGBuint w, NGBuint h);
int ngbInitWindowAtPosition(char* title, NGBuint x, NGBuint y, NGBuint w, NGBuint h);
void ngbInitGraphics(void);
NGBkeyListener* ngbInitKeyListener(NGBboolean altf4);
void ngbUpdateFunc(void(*func)(NGBuint));

void ngbMainLoop(void);

#endif
