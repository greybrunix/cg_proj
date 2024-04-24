/* Output a 3d model file*/
#include <cmath>
#include <stddef.h>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <map>
#include <vector>
#include <float.h>
#include <math.h>

struct pair {
	float x,z;
};

std::map<std::string, unsigned int> vi;
std::vector<unsigned int> ind;
unsigned int i = 0;

void write_file(std::string coord, float x, float y, float z, FILE* output) { 
    char buff[512];
    size_t b_read;
    if (vi.find(coord) != vi.end()) {
        b_read = snprintf(buff, 512, "%u\n", vi[coord]);
        fwrite(buff, sizeof(int8_t), b_read, output);
    } else {
        b_read = snprintf(buff, 512, "%u %.3f, %.3f, %.3f\n", i, x, y, z);
        fwrite(buff, sizeof(int8_t), b_read, output);
        vi[coord] = i++;
    }
}

int32_t gen_sphere(float radius, int32_t slices, int32_t stacks, char*file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	float px, py, pz, alpha_diff = 2 * M_PI / slices,
	      beta_diff = M_PI / stacks, alpha = 0, beta = 0;
	size_t b_read;
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) { /* ifs */
			if(j!=0){
				b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n",
						  radius * cosf(beta - M_PI_2) * cosf(alpha),
						  radius * sinf(beta - M_PI_2),
						  radius*cosf(beta-M_PI_2)*sinf(alpha));
				fwrite(buff, sizeof (int8_t),b_read, output);
				b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n",
						  radius * cosf(beta - M_PI_2 + beta_diff)
							* cosf(alpha + alpha_diff),
						  radius * sinf(beta - M_PI_2 + beta_diff),
						  radius*cosf(beta-M_PI_2+beta_diff)*
							sinf(alpha + alpha_diff));
				fwrite(buff, sizeof (int8_t),b_read, output);
				b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n",
						  radius * cosf(beta - M_PI_2)
							* cosf(alpha + alpha_diff),
						  radius * sinf(beta - M_PI_2),
						  radius*cosf(beta-M_PI_2)
							*sinf(alpha + alpha_diff));
				fwrite(buff, sizeof (int8_t),b_read, output);
			}
			if(j!=stacks-1){
				b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n",
						  radius * cosf(beta - M_PI_2) * cosf(alpha),
						  radius * sinf(beta - M_PI_2),
						  radius*cosf(beta-M_PI_2)*sinf(alpha));
				fwrite(buff, sizeof (int8_t),b_read, output);
				b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n",
						  radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha),
						  radius * sinf(beta - M_PI_2 + beta_diff),
						  radius*cosf(beta-M_PI_2 + beta_diff)*sinf(alpha));
				fwrite(buff, sizeof (int8_t),b_read, output);
				b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n",
						  radius * cosf(beta - M_PI_2 + beta_diff)
							* cosf(alpha + alpha_diff),
						  radius * sinf(beta - M_PI_2 + beta_diff),
						  radius*cosf(beta-M_PI_2+beta_diff)*
							sinf(alpha + alpha_diff));
				fwrite(buff, sizeof (int8_t),b_read, output);
			}
			beta += beta_diff;
		}
		beta = 0;
		alpha += alpha_diff;
	}
	fclose(output);
	return 0;
}

int32_t gen_cone(float radius, float height, int32_t slices, int32_t stacks, char*file)
{
FILE* output = fopen(file, "w+");
char buff[512];
size_t b_read;
int i, j, r = 0;
float y=0;
float angle = 0;
float cur_rad = radius;
float angle_diff = 2*M_PI/slices;
float xz_diff = radius/stacks;
float y_diff = height/stacks;

for(i=0; i<stacks; i++) {
    for(j=0; j<slices; j++) {
        // Bottom face
        if (i==0) {
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle), 0.f, cur_rad*cosf(angle));
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", 0.f, 0.f, 0.f);
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle+angle_diff), 0.f, cur_rad*cosf(angle+angle_diff));
            fwrite(buff, sizeof (int8_t),b_read, output);
        }
       
        if (i==stacks-1){              
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle+angle_diff), y, cur_rad*cosf(angle+angle_diff));
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", 0.f, height, 0.f);
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle), y, cur_rad*cosf(angle));
            fwrite(buff, sizeof (int8_t),b_read, output);
        }
        else {
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", (cur_rad-xz_diff)*sinf(angle+angle_diff), y+y_diff,
                      (cur_rad-xz_diff)*cosf(angle+angle_diff));
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", (cur_rad-xz_diff)*sinf(angle), y+y_diff,
                      (cur_rad-xz_diff)*cosf(angle));
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle+angle_diff), y,
                      cur_rad*cosf(angle+angle_diff));
            fwrite(buff, sizeof (int8_t),b_read, output);

            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", (cur_rad-xz_diff)*sinf(angle), y+y_diff,
                      (cur_rad-xz_diff)*cosf(angle));
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle), y,
                      cur_rad*cosf(angle));
            fwrite(buff, sizeof (int8_t),b_read, output);
            b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", cur_rad*sinf(angle+angle_diff), y,
                      cur_rad*cosf(angle+angle_diff));
            fwrite(buff, sizeof (int8_t),b_read, output);
        }
        angle += angle_diff;
    }
    angle = 0;
    y += y_diff;
    cur_rad -= xz_diff;
}
fclose(output);
return r;
}


int32_t gen_box(float l, int32_t d, char* file) 
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	size_t b_read;
	int32_t i, j, r = 0;
	float x = -l/2;
	float y = l/2;
	float z = -l/2;
	float diff = l/d;

    // Bottom and Top Faces
	for (i=0; i<d; i++){
		for (j=0; j<d; j++) {
			

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, -y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, -y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff,-y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, -y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, -y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, -y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			z += diff;
		}
		z = - l/2;
		x += diff;
	}

	x = -l / 2;
	y = -l / 2;
	z = l / 2;

    // Front and Back faces
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++) {

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y+diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y+diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y+diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y+diff, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y+diff, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x+diff, y+diff, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			y += diff;
		}
		y = -l/2;
		x += diff;
	}
    
	z = - l/2;
	y = - l/2;
	x = l/2;

    // Right and Left faces
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++) {

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y+diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y+diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y+diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y+diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y+diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y+diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			y += diff;
		}
		y = -l/2;
		z += diff;
	}

	fclose(output);
	return 0;
}


int32_t gen_plane(float full_size, int32_t divs, char* file)
{
	FILE* output = fopen(file, "w+"); 
	char buff[512];
	size_t b_read;
	float x = full_size/2, z = -x, off=full_size/divs;
	int i,j, err = 0;

	for (i=0; i < divs; i++) {
		for (j=0; j < divs; j++) {
			//curr.x = x = i * div_len - div_len + off;
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x, z+off);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x-off, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x-off, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x-off, z+off);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x, z+off);
			fwrite(buff, sizeof (int8_t),b_read, output);

			x -= off;
		}
		x = full_size/ 2;
		z += off;
	}

	fclose(output);
	return err;
}


int32_t gen_torus(float inner_radius, float outer_radius,
		  int32_t slices, int32_t stacks, char* file)
{
	FILE* output = fopen(file, "w+");
	float alfa = 0, beta = 0;
	float alfa_diff = 2 * M_PI / slices;
	float beta_diff = 2*M_PI / stacks;
	float px, py, pz;
	char buff[512];
	size_t b_read;
	int32_t rr = 0;

	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {

			px = (inner_radius + outer_radius * cos(beta -M_PI_2)) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa);
			b_read = sprintf(buff, "%.3f %.3f %.3f\n", px, py, pz);
			fwrite(buff, sizeof (int8_t),b_read, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2+ beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			b_read = sprintf(buff, "%.3f %.3f %.3f\n", px, py, pz);
			fwrite(buff, sizeof (int8_t),b_read, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 )) * cos(alfa+alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2);
			pz= (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa+alfa_diff);
			b_read = sprintf(buff, "%.3f %.3f %.3f\n", px, py, pz);
			fwrite(buff, sizeof (int8_t),b_read, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			py =  outer_radius * sin(beta - M_PI_2 + beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			b_read = sprintf(buff, "%.3f %.3f %.3f\n", px, py, pz);
			fwrite(buff, sizeof (int8_t),b_read, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa + alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2 + alfa_diff);
			pz = (inner_radius +outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa + alfa_diff);
			b_read = sprintf(buff, "%.3f %.3f %.3f\n", px, py, pz);
			fwrite(buff, sizeof (int8_t),b_read, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2)) * cos(alfa + alfa_diff);
			py = outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa + alfa_diff);
			b_read = sprintf(buff, "%.3f %.3f %.3f\n", px, py, pz);
			fwrite(buff, sizeof (int8_t),b_read, output);

			beta += beta_diff;
		}
		beta = 0;
		alfa += alfa_diff;
	
	}
	fclose(output);
	return rr;
}

int32_t gen_cylinder(float radius, float height, int32_t slices, char* file) {
    FILE* output = fopen(file, "w+");
    if (output == NULL) {
        perror("Error opening file");
        return -1;
    }

    float angle_diff = 2 * M_PI / slices;

    // Generate vertices for the side faces
    for (int i = 0; i < slices; i++) {
        float angle = i * angle_diff;
        float next_angle = (i + 1) * angle_diff;

        float x0 = radius * sinf(angle);
        float z0 = radius * cosf(angle);
        float x1 = radius * sinf(next_angle);
        float z1 = radius * cosf(next_angle);

        // Generate vertices for the bottom face
        fprintf(output, "0.0 0.0 0.0\n");
        fprintf(output, "%.3f 0.0 %.3f\n", x0, z0);
        fprintf(output, "%.3f 0.0 %.3f\n", x1, z1);

        // Generate vertices for the top face
        fprintf(output, "0.0 %.3f 0.0\n", height);
        fprintf(output, "%.3f %.3f %.3f\n", x1, height, z1);
        fprintf(output, "%.3f %.3f %.3f\n", x0, height, z0);

        // Generate vertices for the side faces
        fprintf(output, "%.3f 0.0 %.3f\n", x0, z0);
        fprintf(output, "%.3f %.3f %.3f\n", x0, height, z0);
        fprintf(output, "%.3f %.3f %.3f\n", x1, 0.0, z1);

        fprintf(output, "%.3f %.3f %.3f\n", x1, 0.0, z1);
        fprintf(output, "%.3f %.3f %.3f\n", x0, height, z0);
        fprintf(output, "%.3f %.3f %.3f\n", x1, height, z1);
    }

    fclose(output);
    return 0;
}
void multMV(float m[4][4], float* v, float* res) {

	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j][k];
		}
	}

}

// calculo de p(u,v) para cada ponto da patch
float puv(float U, float V, float m[4][4]) {

	// matriz onde terá o resultado de m * v
	float res[4];

	// matriz v
	float v[4];

	// Resultado final
	float r;
	
	//Criar a matriz v da
	v[0] = V*V*V;
	v[1] = V*V;
	v[2] = V;
	v[3] = 1;

	//m * v
	multMV(m, v, res);

	// U * m * V
	r = U*U*U * res[0] + U*U * res[1] + U * res[2] + res[3];

	return r;
}

void check_map(std::string coord) {
    if (vi.find(coord) != vi.end()) {
        ind.push_back(vi[coord]);
    } else {
        vi[coord] = i++;
    }
}

int32_t bezieraux(float px[4][4], float py[4][4], float pz[4][4], int tesselation,char* file) {

	FILE* output = fopen(file, "w+");
	if (output == NULL) {
		perror("Error opening file");
		return -1;
	}
	float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
	float t = 1.0f / tesselation;
    std::string coord;

	for (float i = 0; i < 1; i += t) {
		for (float j = 0; j < 1; j += t) {


			x1 = puv(i, j, px);
			x2 = puv(i + t, j, px);
			x3 = puv(i + t, j + t, px);
			x4 = puv(i, j + t, px);


			y1 = puv(i, j, py);
			y2 = puv(i + t, j, py);
			y3 = puv(i + t, j + t, py);
			y4 = puv(i, j + t, py);

			z1 = puv(i, j, pz);
			z2 = puv(i + t, j, pz);
			z3 = puv(i + t, j + t, pz);
			z4 = puv(i, j + t, pz);

            coord = std::to_string(x1) + std::to_string(y1) + std::to_string(z1);
            check_map(coord);
            coord = std::to_string(x2) + std::to_string(y2) + std::to_string(z2);
            check_map(coord);
            coord = std::to_string(x4) + std::to_string(y4) + std::to_string(z4);
            check_map(coord);
            coord = std::to_string(x2) + std::to_string(y2) + std::to_string(z2);
            check_map(coord);
            coord = std::to_string(x3) + std::to_string(y3) + std::to_string(z3);
            check_map(coord);
            coord = std::to_string(x4) + std::to_string(y4) + std::to_string(z4);
            check_map(coord);
		}
	}

	fclose(output);
	return 0;
}

int32_t gen_bezier(char* patch, int tesselation,char* file) {

	bezieraux(px,py,pz,tesselation,file);
	
	return 0;
}
int32_t main(int32_t argc, char**argv)
{
	int32_t err = 0;
	char tmp[1024];
	strcpy(tmp, "../../prims/");
	if (argc < 2) {
		err = -1;
		goto clean;
	}
	if (!strcmp(argv[1], "plane")) {
		if (argc != 5) {
			err = -1;
			goto clean;	
		}
		strcat(tmp, argv[4]);
		strcpy(argv[4], tmp);
		err = gen_plane(atof(argv[2]),
				atoi(argv[3]),
				argv[4]);
	}
	else if (!strcmp(argv[1], "box")) {
		if (argc != 5) {
			err = -1;
			goto clean;
		}
		strcat(tmp, argv[4]);
		strcpy(argv[4], tmp);
		err = gen_box(atof(argv[2]),
			      atoi(argv[3]),
			      argv[4]);
	}
	else if (!strcmp(argv[1], "cone")) {
		if (argc != 7) {
			err = -1;
			goto clean;
		}
		strcat(tmp, argv[6]);
		strcpy(argv[6], tmp);
		err = gen_cone(atof(argv[2]),
				atof(argv[3]),
				atoi(argv[4]),
				atoi(argv[5]),
				argv[6]);
	}
	else if (!strcmp(argv[1], "sphere")) {
		if (argc != 6) {
			err = -1;
			goto clean;
		}
		strcat(tmp, argv[5]);
		strcpy(argv[5], tmp);
		err = gen_sphere(atof(argv[2]),
			      atoi(argv[3]),
			      atoi(argv[4]),
			      argv[5]);
	}
	else if (!strcmp(argv[1], "torus")) {
		if (argc != 7) {
			err = -1;
			goto clean;
		}
		strcat(tmp, argv[6]);
		strcpy(argv[6], tmp);
		err = gen_torus(atof(argv[2]),
			      atof(argv[3]),
			      atoi(argv[4]),
			      atoi(argv[5]),
			      argv[6]);
	}
 	else if (!strcmp(argv[1], "cylinder")) {
        if (argc != 6) {
            err = -1;
            goto clean;
        }
		strcat(tmp, argv[5]);
		strcpy(argv[5], tmp);
		err = gen_cylinder(atof(argv[2]),
			      atoi(argv[3]),
			      atoi(argv[4]),
			      argv[5]);
  	}
	else if (!strcmp(argv[1], "patch")) {
		if (argc != 4) {
			err = -1;
			goto clean;
		}
		//strcat(tmp,arg)
		err = gen_bezier(argv[2], atoi(argv[3]), argv[4]);
	}


clean:
	return err;
}
