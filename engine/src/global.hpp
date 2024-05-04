#ifndef __GLOBAL_HPP
#define __GLOBAL_HPP
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glxew.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cfloat>

struct triple {
	float x, y, z;
};

struct prims {
	int count;
	int group;
	char name[64];
};


struct ident_prim {
	char name[64];
	GLuint vbo, ibo, vertex_count;
	unsigned int index_count;
};

typedef struct camera {
        float dist;
        float alfa;
        float beta;
		struct triple pos;
		struct triple lookAt;
		struct triple up; /* 0 1 0 */
		struct triple proj; /* 60 1 1000*/
} camera;

struct world {
	struct {
	    int h, w, sx, sy;
		char title[64];
	} win;
	camera cam;
	std::vector<struct trans> transformations;
	std::vector<struct prims> primitives;
};

typedef std::vector<struct ident_prim> Primitive_Coords;

#endif
