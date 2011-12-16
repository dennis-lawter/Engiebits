#ifndef NGB_INTERNAL_H_
#define NGB_INTERNAL_H_

/*
 * ----------------------------------------------------------------
 * Simple data structures provided by engine
 * ----------------------------------------------------------------
 */

void _ngbHT_hashInit(void);

/*
 * ----------------------------------------------------------------
 * Game data
 * ----------------------------------------------------------------
 */

GLuint* _ngbLoadTextures(char** fileNames, int* widths, int* heights, int num);
char* _ngbLoadFile(char* filename);
void _ngbDrawAll2D(void);
void _ngbDrawAll3D(void);

/*
 * ----------------------------------------------------------------
 * Game database
 * ----------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------
 * Engiebits main engine
 * ----------------------------------------------------------------
 */

void _ngbDraw(void);
void _ngbReshape(NGBint w, NGBint h);
void _ngbKeyDown(unsigned char key, NGBint x, NGBint y);
void _ngbKeyUp(unsigned char key, NGBint x, NGBint y);
void _ngbSpecDown(NGBint key, NGBint x, NGBint y);
void _ngbSpecUp(NGBint key, NGBint x, NGBint y);

#endif
