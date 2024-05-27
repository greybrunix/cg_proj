#include "vectors.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glxew.h>
#endif
#include <vector>
#include <cstdio>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>
#include "transforms.hpp"
#include "viewfrustum.hpp"
#include <tinyxml2.h>
#include <cstring>
#include <string>
#include <map>
#include <IL/il.h>

using namespace tinyxml2;

int timebase, time, frames = 0;
float fps;
int cur_mode = GL_LINE, cur_face = GL_FRONT;
int global = 0;
float tesselation = 100.F;
bool draw = true;
bool mipmapping = false;
bool vfc = true;
unsigned long drawn = 0, total_proc = 0;
Frustum frustum;

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

struct ident_prim {
	char name[64];
	GLuint vbo, ibo,
		normals,
		texCoord,
		vertex_count;
	unsigned int index_count;
	AABox box;
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
	camera cam;
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
			if (num != NULL) {
				n.x = atof(num);
				num = strtok(NULL, " ");
				n.y = atof(num);
				num = strtok(NULL, " ");
				n.z = atof(num);
				num = strtok(NULL, " ");
				t.x = atof(num);
				num = strtok(NULL, " ");
				t.z = atof(num);
				normals->push_back(n);
				texCoord->push_back(t);
			}
			num = strtok(NULL, " \n");
			coords->push_back(v);
			ind->push_back(i);
		} else {
			ind->push_back(i);
		}
	}
}

triple getBoxInfo(const std::vector<triple>& coords) {
    if (coords.empty()) {
        // Handle empty coords vector case
        return {0.F, 0.F, 0.F};
    }

    // Initialize min and max coordinates to the first point's coordinates
    triple minCoords = coords[0];
    triple maxCoords = coords[0];

    // Iterate over all points to find min and max coordinates
    for (const auto& coord : coords) {
        if (coord.x < minCoords.x) minCoords.x = coord.x;
        if (coord.y < minCoords.y) minCoords.y = coord.y;
        if (coord.z < minCoords.z) minCoords.z = coord.z;

        if (coord.x > maxCoords.x) maxCoords.x = coord.x;
        if (coord.y > maxCoords.y) maxCoords.y = coord.y;
        if (coord.z > maxCoords.z) maxCoords.z = coord.z;
    }

    // Calculate extents
    triple extents;
    extents.x = maxCoords.x - minCoords.x;
    extents.y = maxCoords.y - minCoords.y;
    extents.z = maxCoords.z - minCoords.z;

    return extents;
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
			std::vector<triple> coords;
			std::vector<unsigned int> ind;
			std::vector<triple> normals;
			std::vector<struct doubles> texCoord;
			read_words(fd, &coords, &ind, &normals,
				   &texCoord);

			strcpy(aux.name, world.primitives[i].name);

			aux.vertex_count = coords.size();
			aux.index_count = ind.size();

			glGenBuffers(1, &aux.vbo);
			glGenBuffers(1, &aux.ibo);
			glGenBuffers(1, &aux.normals);
			glGenBuffers(1, &aux.texCoord);

			glBindBuffer(GL_ARRAY_BUFFER, aux.vbo);
			glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(triple), coords.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aux.ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ind.size(), ind.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, aux.normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(triple) * normals.size() , normals.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, aux.texCoord);
			glBufferData(GL_ARRAY_BUFFER, sizeof(doubles) * texCoord.size() , texCoord.data(), GL_STATIC_DRAW);

			prims.push_back(aux);
			triple center = {0.F,0.F,0.F};
			triple extents = getBoxInfo(coords);
			prims[i].box = AABox(center, extents.x, extents.y, extents.z);

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


int loadTexture(char*s) {
	unsigned int t,tw,th;
	unsigned char *texData;
	unsigned int texID;
	// Iniciar o DevIL
	ilInit();
	// Colocar a origem da textura no canto inferior esquerdo
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	// Carregar a textura para memória
	ilGenImages(1,&t);
	ilBindImage(t);
	ilLoadImage((ILstring)s);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// Assegurar que a textura se encontra em RGBA (Red, Green, Blue, Alpha) com um byte (0 -255) por componente
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();
	// Gerar a textura para a placa gráfica
	glGenTextures(1,&texID);
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (mipmapping)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Upload dos dados de imagem
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	if (mipmapping)
		glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

void group_read_model(int cur_g, struct prims* tmp_p,
		      XMLElement *color_xml, XMLElement *txt_xml)
{
	XMLElement* elem;
	rgb rgb;
	colour color;
	float shn;
	if (color_xml) {
		rgb.r = 0;
		rgb.g = 0;
		rgb.b = 0;
		color.specular = rgb;
		color.emissive = rgb;
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
		elem = color_xml->FirstChildElement("diffuse");
		if (elem) {
			rgb.r = elem->FloatAttribute("R");
			rgb.g = elem->FloatAttribute("G");
			rgb.b = elem->FloatAttribute("B");
			color.diffuse = rgb;
		}
		elem = color_xml->FirstChildElement("ambient");
		if (elem) {
			rgb.r = elem->FloatAttribute("R");
			rgb.g = elem->FloatAttribute("G");
			rgb.b = elem->FloatAttribute("B");
			color.ambient = rgb;
		}
		elem = color_xml->FirstChildElement("specular");
		if (elem) {
			rgb.r = elem->FloatAttribute("R");
			rgb.g = elem->FloatAttribute("G");
			rgb.b = elem->FloatAttribute("B");
			color.specular = rgb;
		}
		elem = color_xml->FirstChildElement("emissive");
		if (elem) {
			rgb.r = elem->FloatAttribute("R");
			rgb.g = elem->FloatAttribute("G");
			rgb.b = elem->FloatAttribute("B");
			color.emissive = rgb;
		}
		elem = color_xml->FirstChildElement("shininess");
		if (elem) {
			shn = elem->FloatAttribute("value");
			color.shininess = shn;
		}
		tmp_p->color[cur_g] = color;
	}

	if (txt_xml) {
		std::string path = "../../tsts/test_files_phase_4/",
			file = path + txt_xml->Attribute("file");
		tmp_p->texture[cur_g] = file;
		tmp_p->texID[cur_g] = loadTexture((char*)file.c_str());
	}
}

void lights_read(XMLElement* lights, bool reading = false)
{
	XMLElement* elem = !reading ? lights->FirstChildElement()
		: lights->NextSiblingElement();
	light light;
	const char* type;

	if (!elem)
		return;

	type = elem->Attribute("type");
	if (!type)
		return;

	if (!strcmp("point", type)) {
		strcpy(light.type, "point");
		light.posX = elem->FloatAttribute("posx");
		light.posY = elem->FloatAttribute("posy");
		light.posZ = elem->FloatAttribute("posz");
	}
	else if (!strcmp("directional", type)) {
		strcpy(light.type, "directional");
		light.dirX = elem->FloatAttribute("dirx");
		light.dirY = elem->FloatAttribute("diry");
		light.dirZ = elem->FloatAttribute("dirz");
	}
	else if (!strcmp("spot", type)) {
		strcpy(light.type, "spot");
		light.posX = elem->FloatAttribute("posx");
		light.posY = elem->FloatAttribute("posy");
		light.posZ = elem->FloatAttribute("posz");
		light.dirX = elem->FloatAttribute("dirx");
		light.dirY = elem->FloatAttribute("diry");
		light.dirZ = elem->FloatAttribute("dirz");
		light.cutoff = elem->FloatAttribute("cutoff");
	}
	world.lights.push_back(light);

	lights_read(elem, true);
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
	XMLElement* tran = !reading ? transform->FirstChildElement() :
		transform->NextSiblingElement();
	XMLElement* points_t;
	triple point_t;
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
				point_t.x = points_t->FloatAttribute("x");
				point_t.y = points_t->FloatAttribute("y");
				point_t.z = points_t->FloatAttribute("z");
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
		world.cam.dist = sqrt(world.cam.pos.x * world.cam.pos.x +
				      world.cam.pos.y * world.cam.pos.y +
				      world.cam.pos.z * world.cam.pos.z);
		world.cam.beta = asin(world.cam.pos.y / world.cam.dist);
		world.cam.alfa = atan2(world.cam.pos.x, world.cam.pos.z);
		if (world.win.h == 0)
			world.win.h = 1;
		world.cam.ratio = world.win.w * 1.F / world.win.h;

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
		frustum = Frustum(world.cam);
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
	world.cam.ratio = w * 1.0 / h;
	world.win.h = h;
	world.win.w = w;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(world.cam.proj.x, world.cam.ratio, world.cam.proj.y, world.cam.proj.z);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
	frustum = Frustum(world.cam);
}

void drawfigs(void)
{
	int i, k, l, g;
	drawn = total_proc = 0;
	for (g = 0; g < global; g++) {
		glPushMatrix();
		float matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		for (l = 0; l < world.transformations.size(); l++) /* trans*/
			if (world.transformations[l].group == g) {
				world.transformations[l].t->do_transformation();
				float m[16];
				world.transformations[l].t->get_matrix(m);
				mult_mat_vec(m,
										 matrix, matrix);
			}
		for (k = 0; k < world.primitives.size(); k++) {
				if (world.primitives[k].group == g)
					for (i = 0; i < prims.size(); i++)
						if (!strcmp(prims[i].name, world.primitives[k].name)) {
							if (world.lights.size() > 0){
								if (!world.primitives[k].color.empty()){
									// Apply color of model
									colour color = world.primitives[k].color.at(g);

									float diffuse[] = {color.diffuse.r/255.0f, color.diffuse.g/255.0f, color.diffuse.b/255.0f, 1.0f};
									float ambient[] = {color.ambient.r/255.0f, color.ambient.g/255.0f, color.ambient.b/255.0f, 1.0f};
									float specular[] = {color.specular.r/255.0f, color.specular.g/255.0f, color.specular.b/255.0f, 1.0f};
									float emissive[] = {color.emissive.r/255.0f, color.emissive.g/255.0f, color.emissive.b/255.0f, 1.0f};
									float shininess = color.shininess;

									glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
									glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
									glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
									glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
									glMaterialf(GL_FRONT, GL_SHININESS, shininess);
								} else {
									// Apply color of model
									float diffuse[] = {200.0f/255.0f, 200.0f/255.0f, 200.0f/255.0f, 1.0f/255.0f};
									float ambient[] = {50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f, 1.0f/255.0f};
									float specular[] = {0.0f, 0.0f, 0.0f, 1.0f/255.0f};
									float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f/255.0f};
									float shininess = 0.0f;

									glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
									glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
									glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
									glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
									glMaterialf(GL_FRONT, GL_SHININESS, shininess);
								}
							}

							glBindTexture(GL_TEXTURE_2D, world.primitives[k].texID[g]);

							glBindBuffer(GL_ARRAY_BUFFER, prims[i].vbo);
							glVertexPointer(3,GL_FLOAT,0,0);

							glBindBuffer(GL_ARRAY_BUFFER, prims[i].normals);
							glNormalPointer(GL_FLOAT, 0, 0);

							glBindBuffer(GL_ARRAY_BUFFER, prims[i].texCoord);
							glTexCoordPointer(2, GL_FLOAT, 0, 0);

							glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prims[i].ibo);
							total_proc++;
							if (vfc)
								prims[i].box.applyMVP(matrix);
							if (!vfc || frustum.BoxInFrustum(prims[i].box) != Frustum::OUTSIDE) {
								glDrawElements(GL_TRIANGLES,
												 prims[i].index_count, // número de índices a desenhar
												 GL_UNSIGNED_INT, // tipo de dados dos índices
												 0);// parâmetro não utilizado
								drawn++;
							}

							glBindTexture(GL_TEXTURE_2D, 0);
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
		sprintf(fps_c, "%s-FPS:%d-TOTAL %lu-DRAWN %lu", world.win.title, (int)fps,
						total_proc, drawn);
		glutSetWindowTitle(fps_c);
	}
}

void renderScene(void)
{
    // clear buffers
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
		frustum = Frustum(world.cam);

    // Set the polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Draw coordinate axes
    if (draw) {
	    float red[] = {1.0f, 0.0f, 0.0f};
	    float green[] = {0.0f, 1.0f, 0.0f};
	    float blue[] = {0.0f, 0.0f, 1.0f};
	    bool is = glIsEnabled(GL_LIGHTING);

	if (is)
		glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);

	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);

	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);

	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
	if (is)
		glEnable(GL_LIGHTING);
    }

    // Set up lights
    for (int i = 0; i < world.lights.size(); i++) {
	    if (!strcmp(world.lights[i].type, "point")) {
		    float pos[4] = {world.lights[i].posX, world.lights[i].posY, world.lights[i].posZ, 1.0};
		    glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
	    } else if (!strcmp(world.lights[i].type, "directional")) {
		    float dir[4] = {world.lights[i].dirX, world.lights[i].dirY, world.lights[i].dirZ, 0.0};
		    glLightfv(GL_LIGHT0 + i, GL_POSITION, dir);
	    } else if (!strcmp(world.lights[i].type, "spot")) {
		    float pos[4] = {world.lights[i].posX, world.lights[i].posY, world.lights[i].posZ, 1.0};
		    float dir[3] = {world.lights[i].dirX, world.lights[i].dirY, world.lights[i].dirZ};
		    glLightfv(GL_LIGHT0 + i, GL_POSITION, pos);
		    glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, dir);
		    glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, world.lights[i].cutoff);
	    }
    }

    // Draw primitives
    drawfigs();

    // Swap buffers
    glutSwapBuffers();

    // Update frame rate
    framerate();
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
		world.cam.dist -= 0.1;
		break;
	case 'x':
		world.cam.dist += 0.1;
		break;
	case 'o':
		world.cam.dist -= 10;
		break;
	case 'p':
		world.cam.dist += 10;
		break;
	case 'm':
		mipmapping = !mipmapping;
		for (int i=0; i<world.primitives.size(); i++) {
			for (int j=0; j<world.primitives[i].texture.size(); j++) {
				world.primitives[i].texID[j] = loadTexture((char *) world.primitives[i].texture[j].c_str());
			}
		}
		break;
	case 'F':
		vfc = vfc ? false: true;
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
	frustum = Frustum(world.cam);
}

void printInfo()
{
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("In explorer mode: %s\n%s\n\n",
	       "z/x change sphere radius",
	       "\ta/w/s/d move around the sphere");
	printf("Globally: %s\n%s\n",
	       "+/- change tesselation level of auxiliary drawings",
	       "\tl toggles auxiliary drawings");
}

int main(int argc, char **argv)
{
	int res = 0, i;
	if (argc < 2) {
		return 1;
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

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Enable depth testing and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_RESCALE_NORMAL);

	xml_init(argv[1]);
	if (res < 0) {
		return res;
	}
	res = read_3d_files();
	if (res < 0)
		return res;

	float white[4] = {1.0, 1.0, 1.0, 1.0};
	if (world.lights.size() > 0) {
		glEnable(GL_LIGHTING);
		for (int i=0; i<world.lights.size(); i++) {
			glEnable(GL_LIGHT0 + i);
			glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
			glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);
		}
		float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	}

	glutKeyboardFunc(processKeys);

	// OpenGL settings
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

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
