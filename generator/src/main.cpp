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
struct triple {
	float x,y,z;
};


std::map<std::string, unsigned int> vi;
unsigned int i = 0;

// Formato linha: 
// indice posx posy posz norx nory norz texx texy texz
// OU 
// indice

void write_file(std::string coord,
	float x,
	float y,
	float z,
	float nx,
	float ny,
	float nz,
	float texture1,
	float texture2,
	FILE* output)
{
	char buff[512];
	size_t b_read;
	if (vi.find(coord) != vi.end()) {
		b_read = snprintf(buff, 512, "%u\n", vi[coord]);
		fwrite(buff, sizeof(int8_t), b_read, output);
	}
	else {
		b_read = snprintf(buff, 512, "%u %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", i, x, y, z, nx, ny, nz, texture1, texture2);
		fwrite(buff, sizeof(int8_t), b_read, output);
		vi[coord] = i++;
	}
}

int32_t gen_sphere(float radius,
	int32_t slices,
	int32_t stacks,
	char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	float px, py, pz, alpha_diff = 2 * M_PI / slices,
		beta_diff = M_PI / stacks, alpha = 0, beta = 0;
	std::string coord;
	write_file(coord, px, py, pz, 0, 1, 0, j * texture_diff, i * texture_diff, output);
	float pn[3];
	float texture[2];
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {
				if (j != 0) {
					texture[0] = (float)(i+1) / slices;
					texture[1] = (float)(j+1) / stacks;
					px = radius * cosf(beta - M_PI_2) * cosf(alpha); //p1n 0
					pn[0] = cosf(beta - M_PI_2) * cosf(alpha);
					py = radius * sinf(beta - M_PI_2);//p1n 1
					pn[1] = sinf(beta - M_PI_2);
					pz = radius * cosf(beta - M_PI_2) * sinf(alpha);//p1n 2
					pn[2] = cosf(beta - M_PI_2) * sinf(alpha);
					normalize(pn);
					coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
					write_file(coord, px, py, pz,pn[1],pn[2],pn[3],texture[0],texture[1], output);

					px = radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha + alpha_diff);//p3n 0
					pn[0] = cosf(beta - M_PI_2 + beta_diff) * cosf(alpha + alpha_diff);
					py = radius * sinf(beta - M_PI_2 + beta_diff);//p3n 1
					pn[1] = sinf(beta - M_PI_2 + beta_diff);
					pz = radius * cosf(beta - M_PI_2 + beta_diff) * sinf(alpha + alpha_diff);//p3n 2
					pn[2] = cosf(beta - M_PI_2 + beta_diff) * sinf(alpha + alpha_diff);
					normalize(pn);
					coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
					write_file(coord, px, py, pz, pn[1], pn[2], pn[3], texture[0], texture[1], output);

					px = radius * cosf(beta - M_PI_2) * cosf(alpha + alpha_diff);//p4n 0
					pn[0]= cosf(beta - M_PI_2) * cosf(alpha + alpha_diff);
					py = radius * sinf(beta - M_PI_2);//p4n 1
					pn[1] = sinf(beta - M_PI_2);
					pz = radius * cosf(beta - M_PI_2) * sinf(alpha + alpha_diff);//p3n 2
					pn[2] = cosf(beta - M_PI_2) * sinf(alpha + alpha_diff);
					normalize(pn);
					coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
					write_file(coord, px, py, pz, pn[1], pn[2], pn[3], texture[0], texture[1], output);
				}
			if (j != stacks - 1) {
				texture[0] = (float)i / slices;
				texture[1] = (float)j / stacks;
				px = radius * cosf(beta - M_PI_2) * cosf(alpha);//p1n 0
				pn[0]= cosf(beta - M_PI_2) * cosf(alpha);
				py = radius * sinf(beta - M_PI_2);//p1n 1
				pn[1]= sinf(beta - M_PI_2);
				pz = radius * cosf(beta - M_PI_2) * sinf(alpha);//p1n 2
				pn[2] = cosf(beta - M_PI_2) * sinf(alpha);
				normalize(pn);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, pn[1], pn[2], pn[3], texture[0], texture[1], output);

				px = radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha);//p2n 0
				pn[0]= cosf(beta - M_PI_2 + beta_diff) * cosf(alpha);
				py = radius * sinf(beta - M_PI_2 + beta_diff);//p2n 1
				pn[1]= sinf(beta - M_PI_2 + beta_diff);
				pz = radius * cosf(beta - M_PI_2 + beta_diff) * sinf(alpha);//p2n 2
				pn[2]= cosf(beta - M_PI_2 + beta_diff) * sinf(alpha);
				normalize(pn);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, pn[1], pn[2], pn[3], texture[0], texture[1], output);

				px = radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha + alpha_diff);//p3n 0
				pn[0]= cosf(beta - M_PI_2 + beta_diff) * cosf(alpha + alpha_diff);
				py = radius * sinf(beta - M_PI_2 + beta_diff);//p3n 1
				pn[1]= sinf(beta - M_PI_2 + beta_diff);
				pz = radius * cosf(beta - M_PI_2 + beta_diff) * sinf(alpha + alpha_diff);//p3n 2
				pn[2]= cosf(beta - M_PI_2 + beta_diff) * sinf(alpha + alpha_diff);
				normalize(pn);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, pn[1], pn[2], pn[3], texture[0], texture[1], output);
			}
			beta += beta_diff;
		}
		beta = 0;
		alpha += alpha_diff;
	}
	fclose(output);
	return 0;
}
/*
int32_t gen_cone(float radius,
		 float height,
		 int32_t slices,
		 int32_t stacks,
		 char*file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	std::string coord;
	int i, j, r = 0;
	float y = 0;
	float px, py, pz;
	float angle = 0;
	float cur_rad = radius;
	float angle_diff = 2*M_PI/slices;
	float xz_diff = radius/stacks;
	float y_diff = height/stacks;

	for(i=0; i<stacks; i++) {
		for(j=0; j<slices; j++) {
			// Bottom face
			if (i==0) {
				px = cur_rad*sinf(angle);
				py = 0.0f;
				pz = cur_rad*cosf(angle);

				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = 0.0f;
				py = 0.0f;
				pz = 0.0f;
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = cur_rad*sinf(angle+angle_diff);
				py = 0.0f;
				pz = cur_rad*cosf(angle+angle_diff);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);
			}

			if (i==stacks-1) {
				px = cur_rad*sinf(angle+angle_diff);
				py = y;
				pz = cur_rad*cosf(angle+angle_diff); 
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = 0.0f;
				py = height;
				pz = 0.0f;
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = cur_rad*sinf(angle);
				py = y;
				pz = cur_rad*cosf(angle);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);
			}
			else {
				px = (cur_rad-xz_diff)*sinf(angle+angle_diff);
				py = y+y_diff;
				pz = (cur_rad-xz_diff)*cosf(angle+angle_diff);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = (cur_rad-xz_diff)*sinf(angle);
				py = y+y_diff;
				pz = (cur_rad-xz_diff)*cosf(angle);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = cur_rad*sinf(angle+angle_diff);
				py = y;
				pz = cur_rad*cosf(angle+angle_diff);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = (cur_rad-xz_diff)*sinf(angle);
				py = y+y_diff;
				pz = (cur_rad-xz_diff)*cosf(angle);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = cur_rad*sinf(angle);
				py = y;
				pz = cur_rad*cosf(angle);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);

				px = cur_rad*sinf(angle+angle_diff);
				py = y;
				pz = cur_rad*cosf(angle+angle_diff);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
				write_file(coord, px, py, pz, output);
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
*/

int32_t gen_box(float l, int32_t d, char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	std::string coord;
	int32_t i, j, r = 0;
	float x = -l / 2;
	float y = l / 2;
	float z = -l / 2;
	float px, py, pz;
	float diff = l / d;
	float texture_diff = 1.0f / d;

	// Bottom and Top Faces
	for (i = 0; i < d; i++) {
		for (j = 0; j < d; j++) {
			// Top face (normal: 0, 1, 0)
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 1, 0, j * texture_diff, i * texture_diff, output);
			px = x + diff;
			py = y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 1, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = x + diff;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 1, 0, (j + 1) * texture_diff, i * texture_diff, output);

			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 1, 0, j * texture_diff, i * texture_diff, output);
			px = x;
			py = y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 1, 0, j * texture_diff, (i + 1) * texture_diff, output);
			px = x + diff;
			py = y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 1, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);

			// Bottom face (normal: 0, -1, 0)
			px = x + diff;
			py = -y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, -1, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = x;
			py = -y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, -1, 0, j * texture_diff, i * texture_diff, output);
			px = x + diff;
			py = -y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, -1, 0, (j + 1) * texture_diff, i * texture_diff, output);

			px = x;
			py = -y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, -1, 0, j * texture_diff, (i + 1) * texture_diff, output);
			px = x;
			py = -y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, -1, 0, j * texture_diff, i * texture_diff, output);
			px = x + diff;
			py = -y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, -1, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);

			z += diff;
		}
		z = -l / 2;
		x += diff;
	}

	x = -l / 2;
	y = -l / 2;
	z = l / 2;

    // Front and Back faces
	for (i = 0; i < d; i++) {
		for (j = 0; j < d; j++) {
			// Front face (normal: 0, 0, 1)
			px = x + diff;
			py = y + diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, 1, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, 1, j * texture_diff, i * texture_diff, output);
			px = x + diff;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, 1, (j + 1) * texture_diff, i * texture_diff, output);

			px = x;
			py = y + diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, 1, j * texture_diff, (i + 1) * texture_diff, output);
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, 1, j * texture_diff, i * texture_diff, output);
			px = x + diff;
			py = y + diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, 1, (j + 1) * texture_diff, (i + 1) * texture_diff, output);

			// Back face (normal: 0, 0, -1)
			px = x;
			py = y;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, -1, j * texture_diff, i * texture_diff, output);
			px = x + diff;
			py = y + diff;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, -1, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = x + diff;
			py = y;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, 0, 0, -1, (j + 1) * texture_diff, i * texture_diff, output);

			px = x;
			py = y;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, -1, j * texture_diff, i * texture_diff, output);
			px = x;
			py = y + diff;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, -1, j * texture_diff, (i + 1) * texture_diff, output);
			px = x + diff;
			py = y + diff;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 0, 0, -1, (j + 1) * texture_diff, (i + 1) * texture_diff, output);

			y += diff;
		}
		y = -l / 2;
		x += diff;
	}

	z = -l / 2;
	y = -l / 2;
	x = l / 2;

	// Right and Left faces
	for (i = 0; i < d; i++) {
		for (j = 0; j < d; j++) {
			// Right face (normal: 1, 0, 0)
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 1, 0, 0, j * texture_diff, i * texture_diff, output);
			px = x;
			py = y + diff;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 1, 0, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = x;
			py = y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 1, 0, 0, (j + 1) * texture_diff, i * texture_diff, output);

			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 1, 0, 0, j * texture_diff, i * texture_diff, output);
			px = x;
			py = y + diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 1, 0, 0, j * texture_diff, (i + 1) * texture_diff, output);
			px = x;
			py = y + diff;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, 1, 0, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);

			// Left face (normal: -1, 0, 0)
			px = -x;
			py = y + diff;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, -1, 0, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = -x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, -1, 0, 0, j * texture_diff, i * texture_diff, output);
			px = -x;
			py = y;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, -1, 0, 0, (j + 1) * texture_diff, i * texture_diff, output);
            
			px = -x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, -1, 0, 0, j * texture_diff, i * texture_diff, output);
			px = -x;
			py = y + diff;
			pz = z + diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, -1, 0, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);
			px = -x;
			py = y + diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, -1, 0, 0, (j + 1) * texture_diff, (i + 1) * texture_diff, output);

			y += diff;
		}
		y = -l / 2;
		z += diff;
	}

	fclose(output);
	return 0;
}



int32_t gen_plane(float full_size,
	int32_t divs,
	char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	std::string coord;
	float x = full_size / 2, z = -x, off = full_size / divs;
	float px, py = 0.f, pz;
	int i, j,l, err = 0;
	float texture = 1.0 / divs;
	triple n;
	n.x = 0;
	n.y = 1;
	n.z = 0;

	for (i = 0; i < divs; i++) {
		for (j = 0; j < divs; j++) {
			//curr.x = x = i * div_len - div_len + off;
			if (i == 0) {
				l = i;
			}
			else {
				l = i - 1;
			}
			px = x;
			pz = z + off;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, n.x, n.y, n.z,i*texture,(j+1)*texture, output);
			px = x;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, n.x, n.y, n.z, i * texture, j * texture, output);
			px = x - off;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, n.x, n.y, n.z, l * texture, j * texture, output);
			px = x - off;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, n.x, n.y, n.z, l * texture, j * texture, output);
			px = x - off;
			pz = z + off;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, n.x, n.y, n.z, l * texture, (j+1) * texture, output);
			px = x;
			pz = z + off;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, n.x, n.y, n.z, i * texture, (j + 1) * texture, output);

			x -= off;
		}
		x = full_size / 2;
		z += off;
	}

	fclose(output);
	return err;
}


/*int32_t gen_torus(float inner_radius, float outer_radius,
		  int32_t slices, int32_t stacks, char* file)
{
	FILE* output = fopen(file, "w+");
	float alfa = 0, beta = 0;
	float alfa_diff = 2 * M_PI / slices;
	float beta_diff = 2*M_PI / stacks;
	float px, py, pz;
	char buff[512];
	std::string coord;
	int32_t rr = 0;

	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {

			px = (inner_radius + outer_radius * cos(beta -M_PI_2)) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2+ beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 )) * cos(alfa+alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2);
			pz= (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa+alfa_diff);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			py =  outer_radius * sin(beta - M_PI_2 + beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, output);

			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa + alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2 + alfa_diff);
			pz = (inner_radius +outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa + alfa_diff);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, output);


			px = (inner_radius + outer_radius * cos(beta - M_PI_2)) * cos(alfa + alfa_diff);
			py = outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa + alfa_diff);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
			write_file(coord, px, py, pz, output);

			beta += beta_diff;
		}
		beta = 0;
		alfa += alfa_diff;

	}
	fclose(output);
	return rr;
}

int32_t gen_cylinder(float radius, float height, int32_t slices, char* file)
{
	FILE* output = fopen(file, "w+");
	if (output == NULL) {
		perror("Error opening file");
		return -1;
	}
	std::string coord;
	float angle_diff = 2 * M_PI / slices;
	float px, py, pz;

	// Generate vertices for the side faces
	for (int i = 0; i < slices; i++) {
		float angle = i * angle_diff;
		float next_angle = (i + 1) * angle_diff;

		float x0 = radius * sinf(angle);
		float z0 = radius * cosf(angle);
		float x1 = radius * sinf(next_angle);
		float z1 = radius * cosf(next_angle);

		// Generate vertices for the bottom face
		px = 0.0f;
		py = 0.0f;
		pz = 0.0f;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x0;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x1;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);

		// Generate vertices for the top face
		px = 0.0f;
		py = height;
		pz = 0.0f;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = height;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x0;
		py = height;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);

		// Generate vertices for the side faces
		px = x0;
		py = 0.0f;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x0;
		py = height;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = 0.0f;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = 0.0f;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x0;
		py = height;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = height;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
		write_file(coord, px, py, pz, output);
	}

	fclose(output);
	return 0;
}

void multMM(float a[4][4], float b[4][4], float res[4][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			res[i][j] = 0;
			for (int k = 0; k < 4; k++)
				res[i][j] += a[i][k] * b[k][j];
		}
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
	v[0] = powf(V, 3);
	v[1] = powf(V, 2);
	v[2] = V;
	v[3] = 1;

	//m * v
	multMV(m, v, res);

	// U * m * V
	r = powf(U, 3) * res[0] + powf(U, 2) * res[1] + U * res[2] + res[3];

	return r;
}

int32_t bezieraux(float px[4][4], float py[4][4], float pz[4][4], int tesselation, FILE* output) {

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
			write_file(coord, x1, y1, z1, output);
			coord = std::to_string(x2) + std::to_string(y2) + std::to_string(z2);
			write_file(coord, x2, y2, z2, output);
			coord = std::to_string(x4) + std::to_string(y4) + std::to_string(z4);
			write_file(coord, x4, y4, z4, output);
			coord = std::to_string(x2) + std::to_string(y2) + std::to_string(z2);
			write_file(coord, x2, y2, z2, output);
			coord = std::to_string(x3) + std::to_string(y3) + std::to_string(z3);
			write_file(coord, x3, y3, z3, output);
			coord = std::to_string(x4) + std::to_string(y4) + std::to_string(z4);
			write_file(coord, x4, y4, z4, output);
		}
	}
	return 0;
}

int32_t gen_bezier(char* patch, float tesselation, char* out) {

	FILE* fd = fopen(patch, "r");
	FILE* output = fopen(out, "w+");
	//printf("%.3f",tesselation);
	if (output == NULL) {
		perror("Error opening file");
		return -1;
	}
	char line[1024];
	char* coord;
	char* num;
	float x, y, z;
	std::string aux;
	float px[4][4];
	float py[4][4];
	float pz[4][4];
	std::map<std::string, unsigned int> vi;
	float bezierM[4][4] = { {-1.0f,  3.0f, -3.0f, 1.0f},
				{ 3.0f, -6.0f,  3.0f, 0.0f},
				{-3.0f,  3.0f,  0.0f, 0.0f},
				{ 1.0f,  0.0f,  0.0f, 0.0f} };

	std::vector<triple> points;
	std::vector<std::vector<int>> indexes;


	//printf("%d",strtok(output,"\n");
	fgets(line, sizeof(line), fd);

	//while (fgets(line, sizeof(line), fd) != NULL) {
	int npatches = atoi(strtok(line, "\n"));

	// 32 linhas
	for (int i = 0; i != npatches; i++) {
		fgets(line, sizeof(line), fd);
		std::vector <int> patch1;
		int index = atoi(strtok(line, ","));
		patch1.push_back(index);
		// 16 pontos, 4 quadrados
		for (int l = 1; l < 16; l++) {
			index = atoi(strtok(NULL, ","));
			patch1.push_back(index);
		}
		indexes.push_back(patch1);
	}
	fgets(line, sizeof(line), fd);
	int npoints = atoi(strtok(line, "\n"));

	for (int i = 0; i != npoints; i++) {
		triple ponto;
		fgets(line, sizeof(line), fd);
		float x = atof(strtok(line, ","));
		ponto.x = x;
		float y = atof(strtok(NULL, ","));
		ponto.y = y;
		float z = atof(strtok(NULL, ","));
		ponto.z = z;
		points.push_back(ponto);
	}

	float mataux[4][4];

	// Get the length of the vector
	//size_t length = indexes.size();

	// Print the length of the vector
	//std::cout << "Length of the vector: " << length << std::endl;
	//int testarnpontos = 0;
	for (std::vector <int> quadrado:indexes) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {

				triple point = points[quadrado[i * 4 + j]];
				//printf("%.3f ", point.x);
				//printf("%.3f ", point.y);
				//printf("%.3f\n", point.z);
				px[j][i] = point.x;
				py[j][i] = point.y;
				pz[j][i] = point.z;
				//testarnpontos++;

			}

		}

		multMM(bezierM, px, mataux);
		multMM(mataux, bezierM, px);

		multMM(bezierM, py, mataux);
		multMM(mataux, bezierM, py);

		multMM(bezierM, pz, mataux);
		multMM(mataux, bezierM, pz);

		bezieraux(px, py, pz, tesselation, output);
	}
	//printf("%d",testarnpontos);
	fclose(fd);
	fclose(output);
	return 0;
}*/
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
    /*
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
	}*/
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
	}/*
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
	if (!strcmp(argv[1], "patch")) {
		if (argc != 5) {
			err = -1;
			goto clean;
		}
		strcat(tmp, argv[4]);
		strcpy(argv[4], tmp);
		err = gen_bezier(argv[2], atof(argv[3]), argv[4]);
	}
    */
clean:
	return err;
}
