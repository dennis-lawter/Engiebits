#ifndef NGB_INTERNAL_H_
#define NGB_INTERNAL_H_

#include <stdlib.h>

void _ngbDraw(void);
void _ngbReshape(NGBint w, NGBint h);
void _ngbKeyDown(unsigned char key, NGBint x, NGBint y);
void _ngbKeyUp(unsigned char key, NGBint x, NGBint y);
void _ngbSpecDown(NGBint key, NGBint x, NGBint y);
void _ngbSpecUp(NGBint key, NGBint x, NGBint y);
void _ngbHT_hashInit(void);

#endif
