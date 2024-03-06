#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/cdefs.h>
#include <vector>
#include <tinyxml2.h>
#include <string.h>
using namespace tinyxml2;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

FILE* fd;
float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;
int timebase, frames;
int cur_mode = GL_LINE, cur_face = GL_FRONT;
GLuint vertex_count, vertices;

struct triple {
	float x,y,z;
};
struct prims {
	int count;
	int group;
	char name[64];
};
/* { {.count=2, .group=0, .name="sphere.3d"},
 *   {.count=1, .group=0, .name="plane.3d"},
 *   {.count=1, .group=1, .name="plane.3d"}
 * }*/
/* { 
 *   { {.count=2, .name="sphere.3d"},
 *     {.count=1, .name="plane.3d"}
 *   },
 *   { {.count=1, .name="plane.3d"}
 *   }
 * }*/
/* { {.count=2, .name="sphere.3d"},
 *   {.count=2, .name="plane.3d"},
 * }*/
struct {
	struct {
		int h,w,sx,sy;
		char title[64];
	} win;
	struct {
		struct triple pos;
		struct triple lookAt;
		struct triple up; /* 0 1 0 */
		struct triple proj; /* 60 1 1000*/
  f = open()} cam;
	struct prims primitives[512];
} world;

typedef struct triple* Primitive_Coords;

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

static int not_in_prims_g(const char* f, int* i, int g, int N)
{
	int r = 1;
	for (*i=0; *i< N && r; *i+=1) {
		if (!strcmp(f, world.primitives[*i].name) &&
		    world.primitives[*i].group == g)
		    r = 0;
	}
	return r;
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawfigs(int N) {
    
    glBegin(GL_TRIANGLES);
    for (int i = 0; i<N; i++) {
        glVertex3f(array[i].x, array[i].y, array[i].z);
    }
    glEnd;

}

void renderScene(void) {

	// clear buffers
	int time; float fps; int frame = 0;
	char blah[64];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);



	drawfigs();
	frames++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = (frames * 1000.f) / (time - timebase);
		timebase = time;
		frames = 0;
	}
	sprintf(blah, "CG@DI-UM-%.3f", fps);
	glutSetWindowTitle(blah);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
	switch (c) {
	case 'w':
		break;
	case 's':
		break;
	case '1':
		cur_face = GL_FRONT;
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case '2':
		cur_face = GL_BACK;
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	case '3':
		cur_face = GL_FRONT_AND_BACK;
		glDisable(GL_CULL_FACE);
		break;
	case '4':
		cur_mode = GL_LINE;
		break;
	case '5':
		cur_mode = GL_POINT;
		break;
	case '6':
		cur_mode = GL_FILL;
		break;
	}
	glPolygonMode(cur_face,cur_mode);
	spherical2Cartesian();
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN:
		radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP:
		radius += 0.1f; break;
	}
	spherical2Cartesian();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n\
Page Up and Page Down control the distance from the camera to the origin\n");
}

int xml_init(char* xml_file)
{
	XMLDocument doc;
	XMLElement* world_l, *window, *cam, *posi, *lookAt, *up, *proj,
		*group_R, *gr, *mod, *models;
	int i = 0, g, j, rs = 0;
	const char* f;
	doc.LoadFile(xml_file);

	world_l = doc.RootElement();
	if (world_l) {
		window = world_l->FirstChildElement("window");
		if (!window) {
			return -1;
		}
		world.win.w = window->IntAttribute("width");
		world.win.h = window->IntAttribute("height");
		world.win.sx = window->IntAttribute("x");
		world.win.sy = window->IntAttribute("y");
		strcpy(world.win.title, window->Attribute("title"));
		cam = world_l->FirstChildElement("camera");
		if (!cam) {
			return -2;
		}
		posi = cam->FirstChildElement("position");
		if (!posi) {
			return -3;
		}
		world.cam.pos.x = posi->FloatAttribute("x");
		world.cam.pos.y = posi->FloatAttribute("y");
		world.cam.pos.z = posi->FloatAttribute("z");
		lookAt = cam->FirstChildElement("lookAt");
		if (!lookAt) {
			return -4;
		}
		world.cam.lookAt.x = posi->FloatAttribute("x");
		world.cam.lookAt.y = posi->FloatAttribute("y");
		world.cam.lookAt.z = posi->FloatAttribute("z");
		up = cam->FirstChildElement("up");
		if (up) {
			world.cam.up.x = posi->FloatAttribute("x");
			world.cam.up.y = posi->FloatAttribute("y");
			world.cam.up.z = posi->FloatAttribute("z");
		}
		else {
			world.cam.up.x = 0.f;
			world.cam.up.y = 1.f;
			world.cam.up.z = 0.f;
		}
		proj = cam->FirstChildElement("projection");
		if (proj) {
			world.cam.proj.x = posi->FloatAttribute("x");
			world.cam.proj.y = posi->FloatAttribute("y");
			world.cam.proj.z = posi->FloatAttribute("z");
		}
		else {
			world.cam.proj.x = 60.f;
			world.cam.proj.y = 1.f;
			world.cam.proj.z = 1000.f;
		}
		group_R = world_l->FirstChildElement("group");
		if (group_R){
			for (gr=group_R, g=0;
			     gr;
			     gr = gr->FirstChildElement("group"),
			     g += 1) {

				models = gr->FirstChildElement("models");
				if (models) {
					for (mod=models->FirstChildElement("model");
					     mod;
					     mod=mod->NextSiblingElement("model")
					     ) {

						if (mod){
							f = mod->Attribute("file");
							if ( not_in_prims_g(f, &j, g, i)) {
								strcpy(world.primitives[i].name, f);
								world.primitives[i].count = 1;
								world.primitives[i].group = g;
								i += 1;
							}
							else
								world.primitives[j].count+=1;
						}
					}

				}
			}

		}
	}
	return 0;
}

struct Coords {
    float x;
    float y;
    float z;
};

void read_words (FILE *f) {
    char line[1024];
    struct Coords *array = NULL;
    int i = 0;
    array = (struct Coords * )malloc(i*sizeof(struct Coords));
    while (fgets(line,sizeof(line), f) != NULL) {        
        array = (struct Coords * )realloc(i*sizeof(struct Coords));
        char *token = strtok(line, " \t\n");
        
        array[i].x = atof(token);
        token = strtok(NULL, " \t\n");
        
        array[i].y = atof(token);
        token = strtok(NULL, " \t\n");

        array[i].z = atof(token);
        token = strtok(NULL, " \t\n");

        i++;
    }
}

int read_3d_files(int N)
{
    for (int i=0; i<N; i++) {
        fd = fopen(world.primitives[i].name, "r");
        read_words(fd);
    }
	return 0;
}

int main(int argc, char **argv)
{
	int res = 0;
	if (argc < 2) {
		return 1;
	}
	res = xml_init(argv[1]);
	if (res)
		return res;
	res = read_3d_files();

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(world.win.sx,world.win.sy);
	glutInitWindowSize(world.win.w,world.win.h);
	glutCreateWindow(world.win.title);
	timebase = glutGet(GLUT_ELAPSED_TIME);
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);


	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif


//  OpenGL settings
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1, &vertices);*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(cur_face, cur_mode);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
