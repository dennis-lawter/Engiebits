/*
 * ----------------------------------------------------------------
 * Euclidean geometry functions
 * ----------------------------------------------------------------
 */

#include "NGB/ngb.h"
#include "NGB/ngb_internal.h"

#include <math.h>
#include <float.h>

void NGBvector_normalize(NGBvector* vec) {
	NGBdouble magnitude;
	//euclidean distance = magnitude
	magnitude = sqrt((vec->x * vec->x) + (vec->y * vec->y) + (vec->z * vec->z));
	if (magnitude <= DBL_EPSILON && magnitude >= -DBL_EPSILON) {
		return; //Avoid division by zero!
	}
	//divide each orthogonal direction by the magnitude
	vec->x = vec->x / magnitude;
	vec->y = vec->y / magnitude;
	vec->z = vec->z / magnitude;
}

NGBvector* NGBvector_sum(NGBvector* a, NGBvector* b) {
	//The sum of two vectors has the sum of each component as its own.
	NGBvector* sum = (NGBvector*) malloc(sizeof(NGBvector));

	sum->x = a->x + b->x;
	sum->y = a->y + b->y;
	sum->z = a->z + b->z;

	return sum;
}

NGBvector* NGBvector_difference(NGBvector* a, NGBvector* b) {
	//The difference of two vectors has the difference of each component as its own.
	NGBvector* difference = (NGBvector*) malloc(sizeof(NGBvector));

	difference->x = a->x - b->x;
	difference->y = a->y - b->y;
	difference->z = a->z - b->z;

	return difference;
}

NGBvector* NGBvector_crossProduct(NGBvector* a, NGBvector* b) {
	//The cross product of two vectors as defined by simple matrices.
	NGBvector* cross = (NGBvector*) malloc(sizeof(NGBvector));

	cross->x = (a->y * b->z) - (a->z * b->y);
	cross->y = (a->z * b->x) - (a->x * b->z);
	cross->z = (a->x * b->y) - (a->y * b->x);

	return cross;
}

NGBdouble NGBvector_dotProduct(NGBvector* a, NGBvector* b) {
	return (a->x * b->x) + (a->y * b->y) * (a->z * b->z);
}
