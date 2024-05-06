#include <cstdio>
#include <cstdlib>
#include <tinyxml2.h>
#include <cstring>
#include <string>
#include <map>
#include "transforms.cpp.h"
#include <GL/glut.h>
#include <GL/glxew.h>
#include <vector>
#include <IL/il.h>

using namespace tinyxml2;

int timebase, time, frames = 0;
float fps;
int cur_mode = GL_LINE, cur_face = GL_FRONT;
int global = 0;
float tesselation = 100.F;
bool draw = true;

struct triple {
	float x, y, z;
};

struct rgb {
    float r, g, b;
};

struct doubles {
    float x, z;
};

struct colour {
    rgb diffuse;
    rgb ambient;
    rgb specular;
    rgb emissive;
    float shininess;
};

struct prims {
	int count;
	int group;
	char name[64];
    std::map<int, colour> color;
    std::map<int, std::string> texture;
    std::map<int, unsigned int> texID;
};

struct trans {
	int group;
	transform* t;
};

struct ident_prim {
	char name[64];
	GLuint vbo, ibo, 
           normals, normalsibo,
           texCoord, texCoordibo,
           vertex_count;
	unsigned int index_count;
};

struct light {
    char type[64];
    float posX, posY, posZ,
          dirX, dirY, dirZ,
          cutoff;
};

struct {
	struct {
	    int h, w, sx, sy;
		char title[64];
	} win;
	struct {
        float dist;
        float alfa;
        float beta;
		struct triple pos;
		struct triple lookAt;
		struct triple up; /* 0 1 0 */
		struct triple proj; /* 60 1 1000*/
	} cam;
	std::vector<struct trans> transformations;
	std::vector<struct prims> primitives;
    std::vector<struct light> lights;
} world;

typedef std::vector<struct ident_prim> Primitive_Coords;
Primitive_Coords prims;

void read_words(FILE *f, std::vector<struct triple>* coords, 
                std::vector<unsigned int>* ind, 
                std::vector<struct triple>* normals, 
                std::vector<struct doubles>* texCoord)
{
	char line[1024];
	char* num;
	unsigned int i;

	while (fgets(line, sizeof(line), f) != NULL) {
		num = strtok(line, " ");
		i = atoi(num);
		num = strtok(NULL, " ");
		if (num != NULL) {
			triple v;
            triple n;
            doubles t;
			v.x = atof(num);
			num = strtok(NULL, " ");
			v.y = atof(num);
			num = strtok(NULL, " ");
			v.z = atof(num);
			num = strtok(NULL, " ");
			n.x = atof(num);
			num = strtok(NULL, " ");
			n.y = atof(num);
			num = strtok(NULL, " ");
			n.z = atof(num);
			num = strtok(NULL, " ");
			t.x = atof(num);
			num = strtok(NULL, " ");
			t.z = atof(num);
			num = strtok(NULL, " \n");
			coords->push_back(v);
            normals->push_back(n);
            texCoord->push_back(t);
			ind->push_back(i);
		} else {
			ind->push_back(i);
		}
	}
}

int read_3d_files(void)
{
	FILE* fd;
	int i, j;
	int flag = 0;
	struct ident_prim aux;
	for (i = 0; i < world.primitives.size(); i++) {
		flag = 0;
		for (j = 0; j < prims.size() && !flag; j++) {
			if (!strcmp(world.primitives[i].name, prims[j].name))
				flag = 1;
		}
		if (!flag) {
			fd = fopen(world.primitives[i].name, "r");
			if (!fd) {
				return -1;
			}
			std::vector<struct triple> coords;
			std::vector<unsigned int> ind;
            std::vector<struct triple> normals;
            std::vector<struct doubles> texCoord;
			read_words(fd, &coords, &ind, &normals,
                       &texCoord);

			strcpy(aux.name, world.primitives[i].name);

			aux.vertex_count = coords.size();
			aux.index_count = ind.size();

			// Generate the VBO
			glGenBuffers(1, &aux.vbo);
			glGenBuffers(1, &aux.ibo);
            glGenBuffers(1, &aux.normals);
            glGenBuffers(1, &aux.normalsibo);
            glGenBuffers(1, &aux.texCoord);
            glGenBuffers(1, &aux.texCoordibo);

			// Bind the VBO
			glBindBuffer(GL_ARRAY_BUFFER, aux.vbo);
			glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(triple), coords.data(), GL_STATIC_DRAW);

			// Bind the IBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aux.ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ind.size(), ind.data(), GL_STATIC_DRAW);

            // Bind the normals
            glBindBuffer(GL_ARRAY_BUFFER, aux.normals);
            glBufferData(GL_ARRAY_BUFFER, sizeof(triple) * normals.size() , normals.data(), GL_STATIC_DRAW);
			
            // Bind the textures
            glBindBuffer(GL_ARRAY_BUFFER, aux.texCoord);
            glBufferData(GL_ARRAY_BUFFER, sizeof(doubles) * texCoord.size() , texCoord.data(), GL_STATIC_DRAW);

			// Store the VBO ID in the vertices vector
			prims.push_back(aux);

			fclose(fd);
		}
	}
	return 0;
}

static int not_in_prims_g(const char* f, int* i, int g, int N)
{
	int r = 1;
	for (*i = 0; *i < N && r; *i += 1) {
		if (!strcmp(f, world.primitives[*i].name) &&
		    world.primitives[*i].group == g)
			r = 0;
	}
	return r;
}

int loadTexture(std::string s) {

	unsigned int t,tw,th;
	unsigned char *texData;
	unsigned int texID;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilGenImages(1,&t);
	ilBindImage(t);
	ilLoadImage((ILstring)s.c_str());
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1,&texID);
	
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,		GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,   	GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void group_read_model(int cur_g, struct prims* tmp_p,
                      XMLElement *color_xml, XMLElement *txt_xml)
{
    rgb rgb;
    colour color;
    float shn;
    if (color_xml) {
        rgb.r = 0;
        rgb.g = 0;
        rgb.b = 0;
        color.specular = color.emissive = rgb;
        rgb.r = 200;
        rgb.g = 200;
        rgb.b = 200;
        color.diffuse = rgb;
        rgb.r = 50;
        rgb.g = 50;
        rgb.b = 50;
        color.ambient = rgb;
        shn = 0;
        color.shininess = shn;
        if (color_xml->FirstChildElement("diffuse")) {
            rgb.r = color_xml->FloatAttribute("R");
            rgb.g = color_xml->FloatAttribute("G");
            rgb.b = color_xml->FloatAttribute("B");
        }
        if (color_xml->FirstChildElement("ambient")) {
            rgb.r = color_xml->FloatAttribute("R");
            rgb.g = color_xml->FloatAttribute("G");
            rgb.b = color_xml->FloatAttribute("B");
            color.ambient = rgb;
        }
        if (color_xml->FirstChildElement("specular")) {
            rgb.r = color_xml->FloatAttribute("R");
            rgb.g = color_xml->FloatAttribute("G");
            rgb.b = color_xml->FloatAttribute("B");
            color.specular = rgb;
        }
        if (color_xml->FirstChildElement("emissive")) {
            rgb.r = color_xml->FloatAttribute("R");
            rgb.g = color_xml->FloatAttribute("G");
            rgb.b = color_xml->FloatAttribute("B");
            color.emissive = rgb;
        }
        if (color_xml->FirstChildElement("shininess")) {
            shn = color_xml->FloatAttribute("value");
            color.shininess = shn;
        }
        tmp_p->color[cur_g] = color;
    }

    if (txt_xml) {
        std::string file = txt_xml->Attribute("file");
        tmp_p->texture[cur_g] = file;
        tmp_p->texID[cur_g] = loadTexture(file);
    }
}

void lights_read(XMLElement* lights, bool reading = false)
{
	XMLElement* elem = !reading ? lights->FirstChildElement() :
		lights->NextSiblingElement();

    light light;

    if (!elem)
        return;

    if (elem->FirstChildElement("light")) {
        if (elem->Attribute("point")) {
            strcpy(light.type, "point");
            light.posX = elem->FloatAttribute("posX");
            light.posY = elem->FloatAttribute("posY");
            light.posZ = elem->FloatAttribute("posZ");
        }
        if (elem->Attribute("directional")) {
            strcpy(light.type, "directional");
            light.posX = elem->FloatAttribute("posX");
            light.posY = elem->FloatAttribute("posY");
            light.posZ = elem->FloatAttribute("posZ");
            light.dirX = elem->FloatAttribute("dirX");
            light.dirY = elem->FloatAttribute("dirY");
            light.dirZ = elem->FloatAttribute("dirZ");
        }
        if (elem->Attribute("spotlight")) {
            strcpy(light.type, "spotlight");
            light.posX = elem->FloatAttribute("posX");
            light.posY = elem->FloatAttribute("posY");
            light.posZ = elem->FloatAttribute("posZ");
            light.dirX = elem->FloatAttribute("dirX");
            light.dirY = elem->FloatAttribute("dirY");
            light.dirZ = elem->FloatAttribute("dirZ");
            light.cutoff = elem->FloatAttribute("cutoff");
        }
    }
    world.lights.push_back(light);
    
    lights_read(lights, true);
}

void group_read_models(int cur_parent, int cur_g, XMLElement* models,
		       bool reading = false, int i = 0)
{
	struct prims tmp_p;
	int j;
	const char* f;
	char tmp[1024];
	XMLElement* mod = !reading ? models->FirstChildElement()
		: models->NextSiblingElement();

	if (!mod)
	    return;

    group_read_model(cur_g, &tmp_p, mod->FirstChildElement("color"),
                     mod->FirstChildElement("texture"));

	f = mod->Attribute("file");
	if (not_in_prims_g(f, &j, cur_g, i)) {
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
		world.primitives[j].count += 1;
	group_read_models(cur_parent, cur_g, mod, true, i);
}

void group_read_transform(int cur_parent, int cur_g,
			  XMLElement* transform,
			  bool reading = false)
{
	struct trans tmp;
	std::vector<point> points;
	XMLElement* tran = !reading ? transform->FirstChildElement() :
		transform->NextSiblingElement();
	XMLElement* points_t;
	float point_t[4] = {1.F};
	if (!tran)
		return;
	tmp.group = cur_g;
	if (strcmp(tran->Name(), "translate") == 0) {
		if (!tran->IntAttribute("time")) {
			tmp.t = new translate_static(
				tran->FloatAttribute("x"),
				tran->FloatAttribute("y"),
				tran->FloatAttribute("z"));
		}
		else {
			tmp.t = new translate_catmull_rom(
				tran->IntAttribute("time"),
				tran->BoolAttribute("align"));
			points_t = tran->FirstChildElement();
			while (points_t) {
				point_t[0] = points_t->FloatAttribute("x");
				point_t[1] = points_t->FloatAttribute("y");
				point_t[2] = points_t->FloatAttribute("z");
				tmp.t->add_point(point_t);
				points_t = points_t->NextSiblingElement();
			}
		}
	}
	else if (strcmp(tran->Name(), "rotate") == 0) {
		if (tran->FloatAttribute("angle")) {
			tmp.t = new rotate(
				tran->FloatAttribute("angle"),
				tran->FloatAttribute("x"),
				tran->FloatAttribute("y"),
				tran->FloatAttribute("z"));
		}
		else {
			tmp.t = new rotate(
				tran->IntAttribute("time"),
				tran->FloatAttribute("x"),
				tran->FloatAttribute("y"),
				tran->FloatAttribute("z"));
		}
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

void group_read(int cur_parent, int cur_g, XMLElement* gr,
		bool reading = false,
		int i = 0)
{
	XMLElement* elem = !reading ? gr->FirstChildElement() :
		gr->NextSiblingElement();
	if (cur_g == -1 || !elem)
		return;
	if (cur_parent > -1 && !reading) {
		for (int i = 0; i < world.transformations.size(); i++) {
			struct trans copia;
			if (world.transformations[i].group == cur_parent) {
				copia.group = cur_g;
				copia.t = world.transformations[i].t;
				world.transformations.push_back(copia);
			}
		}
	}
	if (cur_g >= global)
	    global = cur_g + 1;
	if (!strcmp(elem->Name(), "models"))
		group_read_models(cur_parent, cur_g, elem);
	else if (!strcmp(elem->Name(), "transform"))
		group_read_transform(cur_parent, cur_g, elem);
	else if (!strcmp(elem->Name(), "group"))
		group_read(cur_g, global, elem, false, i);
    
	group_read(cur_parent, cur_g, elem, true, i);
}

int xml_init(char* xml_file)
{
	XMLDocument doc;
	XMLElement* world_l, * window, * cam, * posi, * lookAt, * up, * proj,
		* group_R, * gr, * mod, * models, * tran, * trans, * lights;

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
		world.cam.dist = sqrt(world.cam.pos.x*world.cam.pos.x +
				      world.cam.pos.y*world.cam.pos.y+
				      world.cam.pos.z*world.cam.pos.z);
		world.cam.alfa = asin(world.cam.pos.x / (world.cam.dist * cos(world.cam.beta)));
		world.cam.beta = asin(world.cam.pos.y / world.cam.dist);
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
        lights = world_l->FirstChildElement("lights");
        if (lights) {
            lights_read(lights); 
        }
		group_R = world_l->FirstChildElement("group");
		if (group_R) {
            group_read(-1, 0, group_R);
		}
	}
	return i;
}

int find_groups(void)
{
	int res = 0;
	for (int i = 0; i < world.primitives.size(); i++) {
		if (world.primitives[i].group > res)
			res = world.primitives[i].group;
	}
	return res + 1;
}

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
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
	gluPerspective(world.cam.proj.x, ratio, world.cam.proj.y, world.cam.proj.z);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawfigs(void)
{
	int i, k, l, g;
	for (g = 0; g < global; g++) {
		glPushMatrix();
		for (l = 0; l < world.transformations.size(); l++) /* trans*/
			if (world.transformations[l].group == g) {
				world.transformations[l].t->do_transformation();
			}

		for (k = 0; k < world.primitives.size(); k++) {
			if (world.primitives[k].group == g) {
				for (i = 0; i < prims.size(); i++) {
					if (!strcmp(prims[i].name, world.primitives[k].name)) {
                        // Apply color of model
                        colour color = world.primitives[k].color.at(g);
                        float diffuse[] = {color.diffuse.r, color.diffuse.g, color.diffuse.b};
                        float ambient[] = {color.ambient.r, color.ambient.g, color.ambient.b};
                        float specular[] = {color.diffuse.r, color.diffuse.g, color.diffuse.b};
                        float emissive[] = {color.diffuse.r, color.diffuse.g, color.diffuse.b};
                        float shininess = color.shininess;
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
                        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
                        glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
                        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

                        // TODO Add light, glLightfv e glMaterialfv
                        
                        glBindTexture(GL_TEXTURE_2D, world.primitives[k].texID[g]);

						glBindBuffer(GL_ARRAY_BUFFER, prims[i].vbo);
						glVertexPointer(3,GL_FLOAT,0,0);

                        glBindBuffer(GL_ARRAY_BUFFER, prims[i].normals);
                        glNormalPointer(GL_FLOAT, 0, 0);

                        glBindBuffer(GL_ARRAY_BUFFER, prims[i].texCoord);
                        glTexCoordPointer(2, GL_FLOAT, 0, 0);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prims[i].ibo);
						glDrawElements(GL_TRIANGLES,
							       prims[i].index_count, // número de índices a desenhar
							       GL_UNSIGNED_INT, // tipo de dados dos índices
							       0);// parâmetro não utilizado
                        
                        glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
			}
		}
		glPopMatrix();
	}
}

void framerate()
{
	char fps_c[50];
	frames++;
	double time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		double fps = frames * 1000.0 / (time - timebase);
		timebase = time;
		frames = 0;
		sprintf(fps_c, "%s-%d", world.win.title, (int)fps);
		glutSetWindowTitle(fps_c);
	}
}

void renderScene(void)
{
	// clear buffers
	char fps_c[64];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// calculate camera pos
	world.cam.pos.x = world.cam.dist * cos(world.cam.beta) * sin(world.cam.alfa);
	world.cam.pos.y = world.cam.dist * sin(world.cam.beta);
	world.cam.pos.z = world.cam.dist * cos(world.cam.beta) * cos(world.cam.alfa);

	// set the camera
	glLoadIdentity();
	gluLookAt(world.cam.pos.x, world.cam.pos.y, world.cam.pos.z,
		  world.cam.lookAt.x, world.cam.lookAt.y,
		  world.cam.lookAt.z,
		  world.cam.up.x, world.cam.up.y, world.cam.up.z);

	glPolygonMode(GL_FRONT, GL_LINE);

	if (draw) {
		glBegin(GL_LINES);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
		glEnd();
	}

	drawfigs();

	framerate();
	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy)
{
	switch (c) {
	case 'a':
		world.cam.alfa -= 0.1;
		break;
	case 'd':
		world.cam.alfa += 0.1;
		break;
	case 's':
		world.cam.beta -= 0.1;
		break;
	case 'w':
		world.cam.beta += 0.1;
		break;
	case 'l':
		draw -= 1;
		break;
	case '+':
		tesselation += 10;
		break;
	case '-':
		tesselation -= 10;
		break;
    case 'z':
        world.cam.dist -= 10;
        break;
    case 'x':
        world.cam.dist += 10;
    }


	if (tesselation <= 0.F) {
		tesselation = 100.F;
		draw = false;
	}
	else if (tesselation >= 1000000.F)
		tesselation = 1000000.F;
	if (world.cam.beta < -1.5f) {
		world.cam.beta = -1.5f;
	}
	else if (world.cam.beta > 1.5f) {
		world.cam.beta = 1.5f;
	}
}

//void processSpecialKeys(int key, int xx, int yy);

void printInfo()
{
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
	if (res < 0) {
		return res;
	}
	draw = true;
	tesselation = 100.F;

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(world.win.sx, world.win.sy);
	glutInitWindowSize(world.win.w, world.win.h);
	glutCreateWindow(world.win.title);
	timebase = glutGet(GLUT_ELAPSED_TIME);

	// Init GLEW
	glewInit();

	res = read_3d_files();
	if (res < 0)
		return res;

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Enable depth testing and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	glutKeyboardFunc(processKeys);

    // OpenGL settings
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	printInfo();

	// Disable V-Sync
	Display *dpy = glXGetCurrentDisplay();
	GLXDrawable drawable = glXGetCurrentDrawable();
	const int interval = 0;

	if (drawable) {
		glXSwapIntervalEXT(dpy, drawable, interval);
	}

	// Enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

