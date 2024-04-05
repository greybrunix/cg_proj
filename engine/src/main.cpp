#include <cstdio>
#include <cstdlib>
#include <vector>
#include <tinyxml2.h>
#include <cstring>
#include "transforms.cpp.h"

using namespace tinyxml2;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;
int timebase, time, frames=0;
float fps;
int cur_mode = GL_LINE, cur_face = GL_FRONT;
int global = 0;
GLuint vertex_count, vertices;

struct triple {
	float x,y,z;
};

struct prims {
	int count;
	int group;
	char name[64];
};

struct trans {
	int group;
	transform* t;
};

struct ident_prim {
		char name[64];
		std::vector<struct triple> coords;
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
	std::vector<struct trans> transformations;
	std::vector<struct prims> primitives;
} world;

typedef std::vector<struct ident_prim> Primitive_Coords;
Primitive_Coords prims;

void spherical2Cartesian()
{
	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void read_words (FILE *f, int top, std::vector<struct triple>*thing) {
	char line[1024];
	char* num;
	int i = 0, j=0;
	float x,y,z;
	while (fgets(line,sizeof(line), f) != NULL) {
		num = strtok(line, " \n");
		x = atof(num);
		num = strtok(NULL, " \n");
		y = atof(num);
		num = strtok(NULL, " \n");
		z = atof(num);

		thing->push_back({.x=x,.y=y,.z=z});
	}
}

int read_3d_files(void)
{
	FILE* fd;
	int i,j;
	int flag = 0;
	int group = 0;
	struct ident_prim aux;
	for (i=0; i<world.primitives.size(); i++) {
		flag = 0;
		for (j=0; j<prims.size() && !flag;j++) {
			if (!strcmp(world.primitives[i].name, prims[j].name))
				flag = 1;
			}
		if (!flag) {
			fd = fopen(world.primitives[i].name, "r");
			if (!fd) {
				return -1;
			}
						std::vector<struct triple> coords;
			read_words(fd, i, &coords);
						aux.coords = coords;
						strcpy(aux.name, world.primitives[i].name);
			prims.push_back(aux);
			fclose(fd);
		}
	}
	return 0;
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

int group_read_models(int cur_parent, int cur_g, XMLElement*models,
					   bool reading=false, int i=0)
{
	struct prims tmp_p;
	int j;
	const char*f;
	char tmp[1024];
	XMLElement*mod = !reading ? models->FirstChildElement()
		: models->NextSiblingElement();
	if (!mod)
		return i;
	f = mod->Attribute("file");
	if ( not_in_prims_g(f, &j, cur_g, i)) {
		strcpy(tmp, "../../prims/");
		strcpy(tmp_p.name, f);
		strcat(tmp, tmp_p.name);
		strcpy(tmp_p.name, tmp);
		tmp_p.count = 1;
		tmp_p.group = cur_g;
		world.primitives.push_back(tmp_p);
		i += 1;
	}
	else
		world.primitives[j].count+=1;
	return group_read_models(cur_parent, cur_g, mod, true, i);
}
void group_read_transform(int cur_parent, int cur_g,
						  XMLElement*transform,
						  bool reading = false
                          )
{
	struct trans tmp;
	XMLElement*tran = !reading ? transform->FirstChildElement():
		transform->NextSiblingElement();
	if (!tran)
		return;
	tmp.group = cur_g;
	if (strcmp(tran->Name(), "translate") == 0){
		tmp.t = new translate(
			tran->FloatAttribute("x"),
			tran->FloatAttribute("y"),
			tran->FloatAttribute("z")
		);					      
	}
	else if (strcmp(tran->Name(), "rotate") == 0) {
		tmp.t = new rotate(
			tran->FloatAttribute("angle"),
			tran->FloatAttribute("x"),
			tran->FloatAttribute("y"),
			tran->FloatAttribute("z")
		);
	}
	else if (strcmp(tran->Name(), "scale") == 0) {
		tmp.t = new scale(
			tran->FloatAttribute("x"),
			tran->FloatAttribute("y"),
			tran->FloatAttribute("z")
		);
	}
	world.transformations.push_back(tmp);
	group_read_transform(cur_parent, cur_g, tran, true);
}
int group_read(int cur_parent, int cur_g, XMLElement*gr,
               bool reading = false,
               int i=0)
{
	XMLElement*elem = !reading ? gr->FirstChildElement():
		gr->NextSiblingElement();
	if (cur_g == -1 || !elem)
		return i;
    if (cur_parent > -1 && !reading) {
			for (int i=0; i<world.transformations.size(); i++) {
				struct trans copia;
				if (world.transformations[i].group == cur_parent) {
					copia.group = cur_g;
					copia.t = world.transformations[i].t;
					world.transformations.push_back(copia);
				}
			}
		}
	if (cur_g >= global)
		global = cur_g+1;
	if (!strcmp(elem->Name(),"models"))
		i+=group_read_models(cur_parent, cur_g, elem);
	else if (!strcmp(elem->Name(), "transform"))
		group_read_transform(cur_parent, cur_g, elem);
	else if (!strcmp(elem->Name(), "group"))
		group_read(cur_g, global+1, elem,  false, i);
	return group_read(cur_parent, cur_g, elem, true, i);
}

int xml_init(char* xml_file)
{
	XMLDocument doc;
	XMLElement* world_l, *window, *cam, *posi, *lookAt, *up, *proj,
		*group_R, *gr, *mod, *models, *tran, *trans;

	int i = 0, j, rs = 0, g;
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
		lookAt = cam->FirstChildElement("lookAt");
		if (!lookAt) {
			return -4;
		}
		world.cam.lookAt.x = lookAt->FloatAttribute("x");
		world.cam.lookAt.y = lookAt->FloatAttribute("y");
		world.cam.lookAt.z = lookAt->FloatAttribute("z");
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
		proj = cam->FirstChildElement("projection");
		if (proj) {
			world.cam.proj.x = proj->FloatAttribute("fov");
			world.cam.proj.y = proj->FloatAttribute("near");
			world.cam.proj.z = proj->FloatAttribute("far");
		}
		else {
			world.cam.proj.x = 60.f; // fov
			world.cam.proj.y = 1.f; // near
			world.cam.proj.z = 1000.f; //far
		}
		group_R = world_l->FirstChildElement("group");
		if (group_R) {
			i = group_read(-1, 0, group_R);
		}
	}
	return i;
}

int find_groups(void) 
{   
    int res = 0;

    for (int i=0; i<world.primitives.size(); i++) {
        if (world.primitives[i].group > res)
            res = world.primitives[i].group;
    }

    return res+1;
}

void changeSize(int w, int h)
{
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
	int i, j, k, l, g;
    for (g=0; g<global; g++) {
		glPushMatrix();
		for (l=0;l<world.transformations.size();l++) /* trans*/
			if (world.transformations[l].group == g) {
                //printf("%d %d\n", world.transformations[l].group, world.transformations[l].t->get_type());
				world.transformations[l].t->do_transformation();
            }
        for (k=0; k<world.primitives.size(); k++) {
            if (world.primitives[k].group == g) {
                for (i = 0; i<prims.size(); i++) {
                    if (!strcmp(prims[i].name, world.primitives[k].name)) {
		                glBegin(GL_TRIANGLES);
                        for (j=0; j<prims[i].coords.size();j++) {
                            glVertex3f(prims[i].coords[j].x,
                                   prims[i].coords[j].y,
                                   prims[i].coords[j].z);
                        }
		                glEnd();
                    }
                }
            }
        }
		glPopMatrix();
	}
}

void renderScene(void)
{
	// clear buffers
	char fps_c[64];
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


	sprintf(fps_c, "%s-%d",world.win.title, (int)fps);

	glutSetWindowTitle(fps_c);

	drawfigs();
	// End of frame
	glutSwapBuffers();
}


//void processKeys(unsigned char c, int xx, int yy);
//void processSpecialKeys(int key, int xx, int yy);

void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
}

int main(int argc, char **argv)
{
	int res = 0, i;
	if (argc < 2) {
		return 1;
	}
	xml_init(argv[1]);
	if (res < 0)
		return res;

    //global = find_groups();
    printf("GLOBAL %d\n", global);
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
//	glutKeyboardFunc(processKeys);
//	glutSpecialFunc(processSpecialKeys);


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
