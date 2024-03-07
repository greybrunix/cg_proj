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

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;
int timebase, time, frames=0;
float fps;
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
	} cam;
	std::vector<struct prims> primitives;
} world;

typedef std::vector<struct triple> Primitive_Coords;
std::vector<Primitive_Coords> prims;

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
	gluPerspective(world.cam.proj.x,ratio, world.cam.proj.y ,world.cam.proj.z);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawfigs(void)
{
	int i, j;
	glBegin(GL_TRIANGLES);
	for (i = 0; i<prims.size(); i++) {
		for (j=0; j<prims[i].size();j++) {
			glVertex3f(prims[i][j].x,
				   prims[i][j].y,
				   prims[i][j].z);
		}
	}
	glEnd();

}

void renderScene(void) {

	// clear buffers
	char blah[64];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(world.cam.pos.x, world.cam.pos.y, world.cam.pos.z,
		world.cam.lookAt.x, world.cam.lookAt.y
		, world.cam.lookAt.z,
		world.cam.up.x, world.cam.up.y, world.cam.up.z);
	/*gluLookAt(camX, camY,camZ,
		  0.f,0.f,0.f,
		  0.f,1.f,0.f);
	*/

	glPolygonMode(GL_FRONT, GL_LINE);

	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
		glColor3f(1.f,1.f,1.f);
	glEnd();

	frames++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frames * 1000.0 / (time - timebase);
		timebase = time;
		frames = 0;
	}


	sprintf(blah, "%s-%d",world.win.title, (int)fps);

	glutSetWindowTitle(blah);

	drawfigs();
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here
 /*
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
	*/
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
	const char* tit;
	char tmp[1024];
	doc.LoadFile(xml_file);

	world_l = doc.RootElement();
	if (world_l) {
		window = world_l->FirstChildElement("window");
		if (!window) {
			return -1;
		}
		world.win.w = window->IntAttribute("width");
		world.win.h = window->IntAttribute("height");
		world.win.sx = 0;
		world.win.sy = 0;
		world.win.sx = window->IntAttribute("x");
		world.win.sy = window->IntAttribute("y");
		tit = window->Attribute("title");
		if (tit)
			strcpy(world.win.title, window->Attribute("title"));
		else
			strcpy(world.win.title, "CG-PROJ");
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
		printf("%.3f %.3f %.3f\n", world.cam.pos.x,
		       world.cam.pos.y,
		       world.cam.pos.z);
		lookAt = cam->FirstChildElement("lookAt");
		if (!lookAt) {
			return -4;
		}
		world.cam.lookAt.x = lookAt->FloatAttribute("x");
		world.cam.lookAt.y = lookAt->FloatAttribute("y");
		world.cam.lookAt.z = lookAt->FloatAttribute("z");
		printf("%.3f %.3f %.3f\n", world.cam.lookAt.x,
		       world.cam.lookAt.y,
		       world.cam.lookAt.z);
		up = cam->FirstChildElement("up");
		if (up) {
			world.cam.up.x = up->FloatAttribute("x");
			world.cam.up.y = up->FloatAttribute("y");
			world.cam.up.z = up->FloatAttribute("z");
		}
		else {
			world.cam.up.x = 0.f;
			world.cam.up.y = 1.f;
			world.cam.up.z = 0.f;
		}
		printf("%.3f %.3f %.3f\n", world.cam.up.x,
		       world.cam.up.y,
		       world.cam.up.z);
		proj = cam->FirstChildElement("projection");
		if (proj) {
			world.cam.proj.x = proj->FloatAttribute("fov");
			world.cam.proj.y = proj->FloatAttribute("near");
			world.cam.proj.z = proj->FloatAttribute("far");
		}
		else {
			world.cam.proj.x = 60.f; // fov
			world.cam.proj.y = 1.f; // neAR
			world.cam.proj.z = 1000.f; //far
		}
		printf("%.3f %.3f %.3f\n", world.cam.proj.x,
		       world.cam.proj.y,
		       world.cam.proj.z);
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
								struct prims tmp_p;
								strcpy(tmp, "../../prims/");
								strcpy(tmp_p.name, f);
								strcat(tmp, tmp_p.name);
								strcpy(tmp_p.name, tmp);
								tmp_p.count = 1;
								tmp_p.group = g;
								world.primitives.push_back(tmp_p);
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
	return i;
}

void read_words (FILE *f, int top) {
	char line[1024];
	char* num;
	int i = 0, j=0;
	int lnum = 0;
	float x,y,z;
	while (fgets(line,sizeof(line), f) != NULL) {
		num = strtok(line, " \n");
		x = atof(num);
		num = strtok(NULL, " \n");
		y = atof(num);
		num = strtok(NULL, " \n");
		z = atof(num);

		prims[top].push_back({.x=x,.y=y,.z=z});

		lnum++;
	}
}

int read_3d_files(int N)
{
	FILE* fd;
	int i,j;
	int flag = 0;
	struct tmp_s { int g; char*name;};
	std::vector<struct triple> aux;
	std::vector<struct tmp_s> already_read;
	for (i=0; i<N; i++) {
		flag = 0;
		for (j=0; j<already_read.size() && !flag;j++)
			if (!strcmp(world.primitives[i].name, already_read[j].name)&&
				world.primitives[i].group == already_read[j].g)
				flag = 1;
		if (!flag) {
			fd = fopen(world.primitives[i].name, "r");
			if (!fd) {
				return -1;
			}
			for (j=0; j < world.primitives[i].count; j++)
				prims.push_back(aux);
			read_words(fd, i);
			fclose(fd);
			already_read.push_back({.g=world.primitives[i].group,
									   .name=world.primitives[i].name});
									   
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	int res = 0, i;
	if (argc < 2) {
		return 1;
	}
	res = xml_init(argv[1]);
	if (res < 0)
		return res;
	i = res;
	res = read_3d_files(i);

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

	//spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
