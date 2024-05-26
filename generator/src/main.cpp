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

void normalize(float* a1, float* a2, float* a3) {
	float l = sqrt((*a1) * (*a1) + (*a2) * (*a2) + (*a3) * (*a3));
	if (l != 0) {
		*a1 = *a1 / l;
		*a2 = *a2 / l;
		*a3 = *a3 / l;
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
    float pnx, pny, pnz;
    float texture[2];
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            if (j != 0) {
                px = radius * cos(M_PI_2 - beta) * sin(alpha);
                py = radius * sin(M_PI_2 - beta);
                pz = radius * cos(M_PI_2 - beta) * cos(alpha);
                pnx = cos(M_PI_2 - beta) * sin(alpha);
                pny = sin(M_PI_2 - beta);
                pnz = cos(M_PI_2 - beta) * cos(alpha);

                normalize(&pnx, &pny, &pnz);
                texture[0] = (float) i / slices;
                texture[1] = (float) (stacks - j) / stacks;
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) + std::to_string(texture[0]) + std::to_string(texture[1]);
                write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

                px = radius * cos(M_PI_2 - beta + beta_diff) * sin(alpha_diff * (i + 1));
                py = radius * sin(M_PI_2 - beta + beta_diff);
                pz = radius * cos(M_PI_2 - beta_diff * (j + 1)) * cos(alpha_diff * (i + 1));
                pnx = cos(M_PI_2 - beta_diff * (j + 1)) * sin(alpha_diff * (i + 1));
                pny = sin(M_PI_2 - beta_diff * (j + 1));
                pnz = cos(M_PI_2 - beta_diff * (j + 1)) * cos(alpha_diff * (i + 1));

                normalize(&pnx, &pny, &pnz);
                texture[0] = (float) (i + 1) / slices;
                texture[1] = (float) (stacks - (j + 1)) / stacks;
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) + std::to_string(texture[0]) + std::to_string(texture[1]);
                write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

                px = radius * cos(M_PI_2 - beta_diff * j) * sin(alpha_diff * (i + 1));
                py = radius * sin(M_PI_2 - beta_diff * j);
                pz = radius * cos(M_PI_2 - beta_diff * j) * cos(alpha_diff * (i + 1));
                pnx = cos(M_PI_2 - beta_diff * j) * sin(alpha_diff * (i + 1));
                pny = sin(M_PI_2 - beta_diff * j);
                pnz = cos(M_PI_2 - beta_diff * j) * cos(alpha_diff * (i + 1));

                normalize(&pnx, &pny, &pnz);
                texture[0] = (float) (i + 1) / slices;
                texture[1] = (float) (stacks - j) / stacks;
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) + std::to_string(texture[0]) + std::to_string(texture[1]);
                write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);
            }
            if (j != stacks - 1) {
                px = radius * cos(M_PI_2 - beta_diff * j) * sin(alpha_diff * i);
                py = radius * sin(M_PI_2 - beta_diff * j);
                pz = radius * cos(M_PI_2 - beta_diff * j) * cos(alpha_diff * i);
                pnx = cos(M_PI_2 - beta_diff * j) * sin(alpha_diff * i);
                pny = sin(M_PI_2 - beta_diff * j);
                pnz = cos(M_PI_2 - beta_diff * j) * cos(alpha_diff * i);

                normalize(&pnx, &pny, &pnz);
                texture[0] = (float) i / slices;
                texture[1] = (float) (stacks - j) / stacks;
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) + std::to_string(texture[0]) + std::to_string(texture[1]);
                write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

                px = radius * cos(M_PI_2 - beta_diff * (j + 1)) * sin(alpha_diff * i);
                py = radius * sin(M_PI_2 - beta_diff * (j + 1));
                pz = radius * cos(M_PI_2 - beta_diff * (j + 1)) * cos(alpha_diff * i);
                pnx = cos(M_PI_2 - beta_diff * (j + 1)) * sin(alpha_diff * i);
                pny = sin(M_PI_2 - beta_diff * (j + 1));
                pnz = cos(M_PI_2 - beta_diff * (j + 1)) * cos(alpha_diff * i);

                normalize(&pnx, &pny, &pnz);
                texture[0] = (float) i / slices;
                texture[1] = (float) (stacks - (j + 1)) / stacks;
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) + std::to_string(texture[0]) + std::to_string(texture[1]);
                write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

                px = radius * cos(M_PI_2 - beta_diff * (j + 1)) * sin(alpha_diff * (i + 1));
                py = radius * sin(M_PI_2 - beta_diff * (j + 1));
                pz = radius * cos(M_PI_2 - beta_diff * (j + 1)) * cos(alpha_diff * (i + 1));
                pnx = cos(M_PI_2 - beta_diff * (j + 1)) * sin(alpha_diff * (i + 1));
                pny = sin(M_PI_2 - beta_diff * (j + 1));
                pnz = cos(M_PI_2 - beta_diff * (j + 1)) * cos(alpha_diff * (i + 1));

                normalize(&pnx, &pny, &pnz);
                texture[0] = (float) (i + 1) / slices;
                texture[1] = (float) (stacks - (j + 1)) / stacks;
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) + std::to_string(texture[0]) + std::to_string(texture[1]);
                write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);
            }
            beta += beta_diff;
        }
        beta = 0;
        alpha += alpha_diff;
    }
    fclose(output);
    return 0;
}

int32_t gen_box(float l, int32_t d, char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	std::string coord;
	int32_t i, j, r = 0;
	float x = -l/2;
	float y = l/2;
	float z = -l/2;
	float px, py, pz;
    float pnx, pny, pnz;
	float diff = l/d;
	float texture = 1.0f / d;
	float finaltexture[2];

	// Bottom and Top Faces
	for (i=0; i<d; i++){
		for (j=0; j<d; j++) {
			//Top
            pnx = 0;
            pny = 1;
            pnz = 0;

			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = y;
			pz = pz+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = x+diff;
			py= y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			

			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//Bottom
            pnx = 0;
            pny = -1;
            pnz = 0;
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = -y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = -y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = x+diff;
			py = -y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = -y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			pz = x;
			py = -y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = -y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

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
			//Front
            pnx = 0;
            pny = 0;
            pnz = 1;

			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = y+diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = x+diff;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			
			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = y+diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = y+diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//Back
            pnx = 0;
            pny = 0;
            pnz = -1;

			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = y+diff;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = x+diff;
			py = y;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = y+diff;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x+diff;
			py = y+diff;
			pz = -z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

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
			//Right
            pnx = 1;
            pny = 0;
            pnz = 0;

			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = y+diff;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = y+diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x;
			py = y+diff;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//Left
            pnx = -1;
            pny = 0;
            pnz = 0;

			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = -x;
			py = y+diff;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = -x;
			py = y;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = -x;
			py = y;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			
			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = -x;
			py = y+diff;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = -x;
			py = y;
			z = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = -x;
			py = y+diff;
			pz = z+diff;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			y += diff;
		}
		y = -l/2;
		z += diff;
	}

	fclose(output);
	return 0;
}

int32_t gen_cone(float radius,
	float height,
	int32_t slices,
	int32_t stacks,
	char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	std::string coord;
	int i, j, r = 0;
	float y = 0;
	float px, py, pz;
	float angle = 0;
	float cur_rad = radius;
	float angle_diff = 2 * M_PI / slices;
	float rat = height/radius;
	float xz_diff = radius / stacks;
	float y_diff = height / stacks;
	float pnx, pny, pnz;
	float texture[2],texture2[2];
	float x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4,r1,r2,h1,h2;
	int flag=0;
	for (j = 0; j<slices; j++){
		// Bottom face
		pnx = 0;
		pny = -1;
		pnz = 0;
		texture[0] = 0.5f * sinf(angle);
		texture[1] = 0.5f * cosf(angle);

		px = cur_rad * sinf(angle);
		py = 0.0f;
		pz = cur_rad * cosf(angle);

		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture[1]);
		write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

		px = 0.0f;
		py = 0.0f;
		pz = 0.0f;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture[1]);
		write_file(coord, px, py, pz, pnx, pny, pnz, 0.5f, 0.5f, output);

		px = cur_rad * sinf(angle + angle_diff);
		py = 0.0f;
		pz = cur_rad * cosf(angle + angle_diff);
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture[1]);
		write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);
	}
	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
				/*texture[0] = (float)j / slices;
				texture[1] = (float)i / stacks;
				texture2[0] = (float)(j+1) / slices;
				texture2[1] = (float)(i+1) / stacks;
				px = (cur_rad - xz_diff) * sinf(angle + angle_diff);
				pnx = cosf(atan(radius / height)) * sinf(angle + angle_diff);
				py = y + y_diff;
				pny = sinf(atan(radius / height));
				pz = (cur_rad - xz_diff) * cosf(angle + angle_diff);
				pnz = cosf(atan(radius / height)) * cosf(angle + angle_diff);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

				px = (cur_rad - xz_diff) * sinf(angle);
				pnx = cosf(atan(radius / height)) * sinf(angle);
				py = y + y_diff;
				pny = sinf(atan(radius / height));
				pz = (cur_rad - xz_diff) * cosf(angle);
				pnz = cosf(atan(radius / height)) * cosf(angle);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture2[0], texture[1], output);

				px = cur_rad * sinf(angle + angle_diff);
				pnx = cosf(atan(radius / height)) * sinf(angle + angle_diff);
				py = y;
				pny = sinf(atan(radius / height));
				pz = cur_rad * cosf(angle + angle_diff);
				pnz = cosf(atan(radius / height)) * cosf(angle + angle_diff);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture2[1], output);

				px = (cur_rad - xz_diff) * sinf(angle);
				pnx= cosf(atan(radius / height)) * sinf(angle);
				py = y + y_diff;
				pny = sinf(atan(radius / height));
				pz = (cur_rad - xz_diff) * cosf(angle);
				pnz= cosf(atan(radius / height)) * cosf(angle);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture2[1], output);

				px = cur_rad * sinf(angle);
				pnx = cosf(atan(radius / height)) * sinf(angle);
				py = y;
				pny = sinf(atan(radius / height));
				pz = cur_rad * cosf(angle);
				pnz = cosf(atan(radius / height)) * cosf(angle);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture2[0], texture[1], output);

				px = cur_rad * sinf(angle + angle_diff);
				pnx= cosf(atan(radius / height)) * sinf(angle + angle_diff);
				py = y;
				pny = sinf(atan(radius / height));
				pz = cur_rad * cosf(angle + angle_diff);
				pnz = cosf(atan(radius / height)) * cosf(angle + angle_diff);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture2[0], texture2[1], output);*/
				
			

			//coneAngle = atan(r / h)
			/*//(cos(coneAngle) * cos(circleAngle), sin(coneAngle), cos(coneAngle) * sin(circleAngle))
			if (i == stacks - 1) {
				
				texture[0] = (float)j / slices;
				texture[1] = (float)i / stacks;
				px = cur_rad * sinf(angle + angle_diff);
				pnx= cosf(atan(radius / height)) * sinf(angle + angle_diff);
				py = y;
				pny = sinf(atan(radius / height));
				pz = cur_rad * cosf(angle + angle_diff);
				pnz= cosf(atan(radius / height)) * cosf(angle + angle_diff);
                		normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

				//texture[0] = (float)j / slices;
				//texture[1] = (float)i / stacks;
				
				texture[0] = 0.5f;
				texture[1] = 1.0f; // height/height
				
				px = 0.0f;
				pnx = cosf(atan(radius / height));
				py = height;
				pny = sinf(atan(radius / height));
				pz = 0.0f;
				pnz = cosf(atan(radius / height));
                		normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture2[1], output);

				texture[0] = (float)j / slices;
				texture[1] = (float)i / stacks;
				px = cur_rad * sinf(angle);
				pnx = cosf(atan(radius / height)) * sinf(angle);
				py = y;
				pny = sinf(atan(radius / height));
				pz = cur_rad * cosf(angle);
				pnz= cosf(atan(radius / height)) * cosf(angle);
               			normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture2[1], output);
			}*/
				texture[0] = (float)j / slices;
				texture[1] = (float)i / stacks;
				texture2[0] = (float)(j+1) / slices;
				texture2[1] = (float)(i+1) / stacks;
				
            			h1 = height - (i * y_diff);
            			h2 = height - ((i+1) * y_diff);
            			r1 = h1 / rat;
           	 		r2 = h2 / rat;
				
				x1 = r1 * sin(angle_diff * j);
				y1 = (i * y_diff);
				z1 = r1 * cos(angle_diff * j);
				
				//px = (cur_rad - xz_diff) * sinf(angle + angle_diff);
				px = r1 * sin(angle_diff * j);
				pnx = cosf(atan(radius / height)) * sinf(angle + angle_diff);
				
				//py = y + y_diff;
				py =  y;
				pny = sinf(atan(radius / height));

				//pz = (cur_rad - xz_diff) * cosf(angle + angle_diff);
				pz = r1 * cos(angle_diff * j);
				pnz = cosf(atan(radius / height)) * cosf(angle + angle_diff);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture[1], output);

				x2 = r1 * sin(angle_diff * (j+1));
				y1 = (i * y_diff);
				z2 = r1 * cos(angle_diff * (j+1));

				//px = (cur_rad - xz_diff) * sinf(angle);
				px= r1 * sin(angle_diff * (j+1));
				pnx = cosf(atan(radius / height)) * sinf(angle);
				
				//py = y + y_diff;
				py = y;
				pny = sinf(atan(radius / height));
				
				//pz = (cur_rad - xz_diff) * cosf(angle);
				pz = r1 * cos(angle_diff * (j+1));
				pnz = cosf(atan(radius / height)) * cosf(angle);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture2[0], texture[1], output);

				x4 = r2 * sin(angle_diff * j);
				y2 = (i+1) * y_diff;
				z4 = r2 * cos(angle_diff * j);

				//px = cur_rad * sinf(angle + angle_diff);
				px = r2 * sin(angle_diff * j);
				pnx = cosf(atan(radius / height)) * sinf(angle + angle_diff);
				
				//py = y;
				py = y+y_diff;
				pny = sinf(atan(radius / height));
				
				//pz = cur_rad * cosf(angle + angle_diff);
				pz = r2 * cos(angle_diff * j);
				pnz = cosf(atan(radius / height)) * cosf(angle + angle_diff);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture2[1], output);
			
			if(i!= stacks-1){
				
				texture[0] = (float)j / slices;
				texture[1] = (float)i / stacks;
				texture2[0] = (float)(j+1) / slices;
				texture2[1] = (float)(i+1) / stacks;

				x4 = r2 * sin(angle_diff * j);
				y2 = (i+1) * y_diff;
				z4 = r2 * cos(angle_diff * j);
				

				//px = (cur_rad - xz_diff) * sinf(angle);
				px = r2 * sin(angle_diff * j);
				pnx= cosf(atan(radius / height)) * sinf(angle);
				
				py = y + y_diff;
				pny = sinf(atan(radius / height));
				
				//pz = (cur_rad - xz_diff) * cosf(angle);
				pz = r2 * cos(angle_diff * j);
				pnz= cosf(atan(radius / height)) * cosf(angle);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture[0], texture2[1], output);

				x2 = r1 * sin(angle_diff * (j+1));
				y1 = (i * y_diff);
				z2 = r1 * cos(angle_diff * (j+1));
				

				//px = cur_rad * sinf(angle);
				px = r1 * sin(angle_diff * (j+1)); 
				pnx = cosf(atan(radius / height)) * sinf(angle);
				
				py = y;
				pny = sinf(atan(radius / height));
				
				//pz = cur_rad * cosf(angle);
				pz =  r1 * cos(angle_diff * (j+1));
				pnz = cosf(atan(radius / height)) * cosf(angle);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture2[0], texture[1], output);


				x3 = r2 * sin(angle_diff * (j+1));
				y2 = (i+1) * y_diff;
				z3 = r2 * cos(angle_diff * (j+1));

				//px = cur_rad * sinf(angle + angle_diff);
				px = r2 * sin(angle_diff * (j+1));
				pnx= cosf(atan(radius / height)) * sinf(angle + angle_diff);
				py = y+y_diff;
				pny = sinf(atan(radius / height));
				
				//pz = cur_rad * cosf(angle + angle_diff);
				pz = r2 * cos(angle_diff * (j+1));
				pnz = cosf(atan(radius / height)) * cosf(angle + angle_diff);
				normalize(&pnx, &pny, &pnz);
				coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz)+ std::to_string(texture2[0]) + std::to_string(texture2[1]);
				write_file(coord, px, py, pz, pnx, pny, pnz, texture2[0], texture2[1], output);
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

int32_t gen_plane(float full_size,
	int32_t divs,
	char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	std::string coord;
	float x = -(full_size / 2);
	float z = -(full_size / 2);
	float off = full_size / divs;
	float px, py = 0.f, pz;
    float pnx, pny, pnz;
	int i, j, l,fix, err = 0;
	float texture = 1.0f / divs;
	float finaltexture[2];

    pnx = 0;
    pny = 1;
    pnz = 0;
	for (i = 0; i < divs; i++) {
		for (j = 0; j < divs; j++) {
			//curr.x = x = i * div_len - div_len + off;
			//finaltexture[0]=i*texture;
			//finaltexture[1]=j*texture;
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + 
                std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) +
                std::to_string(finaltexture[0]) + std::to_string(finaltexture[1]);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//finaltexture[0]=(i+1)*texture;
			//finaltexture[1]=(j+1)*texture;
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x + off;
			pz = z + off;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + 
                std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) +
                std::to_string(finaltexture[0]) + std::to_string(finaltexture[1]);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);
			
			//finaltexture[0]=(i+1)*texture;
			//finaltexture[1]=j*texture;
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=j*texture;
			px = x + off;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + 
                std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) +
                std::to_string(finaltexture[0]) + std::to_string(finaltexture[1]);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//finaltexture[0]=i*texture;
			//finaltexture[1]=j*texture;
			finaltexture[0]=i*texture;
			finaltexture[1]=j*texture;
			px = x ;
			pz = z;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + 
                std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) +
                std::to_string(finaltexture[0]) + std::to_string(finaltexture[1]);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//finaltexture[0]=i*texture;
			//finaltexture[1]=(j+1)*texture;
			finaltexture[0]=i*texture;
			finaltexture[1]=(j+1)*texture;
			px = x ;
			pz = z + off ;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + 
                std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) +
                std::to_string(finaltexture[0]) + std::to_string(finaltexture[1]);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			//finaltexture[0]=(i+1)*texture;
			//finaltexture[1]=(j+1)*texture;
			finaltexture[0]=(i+1)*texture;
			finaltexture[1]=(j+1)*texture;
			px = x + off;
			pz = z + off;
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + 
                std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz) +
                std::to_string(finaltexture[0]) + std::to_string(finaltexture[1]);
			write_file(coord, px, py, pz, pnx, pny, pnz,finaltexture[0],finaltexture[1], output);

			z += off;
		}
		z = -(full_size / 2);
		x += off;
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
	float px, py, pz, pnx, pny, pnz;
	char buff[512];
	std::string coord;
	int32_t rr = 0;
	float texture[2];

	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {
			
			texture[0]=(float) i/stacks;
			texture[1]=(float) j/slices;
			px = (inner_radius + outer_radius * cos(beta -M_PI_2)) * cos(alfa);
			pnx= outer_radius * cos(beta -M_PI_2) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2);
			pny= outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa);
			pnz= outer_radius * cos(beta - M_PI_2) * sin(alfa);
			normalize(&pnx,&pny,&pnz);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz, 0, 0, output);

			texture[0]=(float) (i+1)/stacks;
			texture[1]=(float) j/slices;
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			pnx= outer_radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa);
			py = outer_radius * sin(beta - M_PI_2+ beta_diff);
			pny= outer_radius * sin(beta - M_PI_2+ beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			pnz= outer_radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa);
			normalize(&pnx,&pny,&pnz);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz, 0, 0, output);

			texture[0]=(float) i/stacks;
			texture[1]=(float) (j+1)/slices;
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 )) * cos(alfa+alfa_diff);
			pnx= outer_radius * cos(beta - M_PI_2 ) * cos(alfa+alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2);
			pny= outer_radius * sin(beta - M_PI_2);
			pz= (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa+alfa_diff);
			pnz= outer_radius * cos(beta - M_PI_2) * sin(alfa+alfa_diff);
			normalize(&pnx,&pny,&pnz);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz, 0, 0, output);

			texture[0]=(float) (i+1)/stacks;
			texture[1]=(float) j/slices;
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa);
			pnx= outer_radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa);
			py =  outer_radius * sin(beta - M_PI_2 + beta_diff);
			pny= outer_radius * sin(beta - M_PI_2 + beta_diff);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa);
			pnz= outer_radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa);
			normalize(&pnx,&pny,&pnz);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz, 0, 0, output);

			texture[0]=(float) (i+1)/stacks;
			texture[1]=(float) (j+1)/slices;
			px = (inner_radius + outer_radius * cos(beta - M_PI_2 + beta_diff)) * cos(alfa + alfa_diff);
			pnx= outer_radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa + alfa_diff);
			py =  outer_radius * sin(beta - M_PI_2 + alfa_diff);
			pnz=  outer_radius * sin(beta - M_PI_2 + alfa_diff);
			pz = (inner_radius +outer_radius * cos(beta - M_PI_2 + beta_diff)) * sin(alfa + alfa_diff);
			pnz = outer_radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa + alfa_diff);
			normalize(&pnx,&pny,&pnz);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz, 0, 0, output);

			texture[0]=(float) i/stacks;
			texture[1]=(float) (j+1)/slices;
			px = (inner_radius + outer_radius * cos(beta - M_PI_2)) * cos(alfa + alfa_diff);
			pnx= outer_radius * cos(beta - M_PI_2) * cos(alfa + alfa_diff);
			py = outer_radius * sin(beta - M_PI_2);
			pny= outer_radius * sin(beta - M_PI_2);
			pz = (inner_radius + outer_radius * cos(beta - M_PI_2)) * sin(alfa + alfa_diff);
			pnz = outer_radius * cos(beta - M_PI_2) * sin(alfa + alfa_diff);
			normalize(&pnx,&pny,&pnz);
			coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
			write_file(coord, px, py, pz, pnx, pny, pnz, 0, 0, output);

			beta += beta_diff;
		}
		beta = 0;
		alfa += alfa_diff;

	}
	fclose(output);
	return rr;
}
/*
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
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x0;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x1;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);

		// Generate vertices for the top face
		px = 0.0f;
		py = height;
		pz = 0.0f;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = height;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x0;
		py = height;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);

		// Generate vertices for the side faces
		px = x0;
		py = 0.0f;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x0;
		py = height;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = 0.0f;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = 0.0f;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x0;
		py = height;
		pz = z0;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
		px = x1;
		py = height;
		pz = z1;
		coord = std::to_string(px) + std::to_string(py) + std::to_string(pz) + std::to_string(pnx) + std::to_string(pny) + std::to_string(pnz);
		write_file(coord, px, py, pz, output);
	}

	fclose(output);
	return 0;
}*/

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

float bu(float U, float V, float m[4][4]) {

	float aux[4];
	float v[4];
	float r;

	v[0] = V*V*V;
	v[1] = V*V;
	v[2] = V;
	v[3] = 1;

	multMV(m, v, aux);

	r = 3 * U*U * aux[0] + 2 * U * aux[1] + aux[2];

	return r;

}

float bv(float U, float V, float m[4][4]) {

	float aux[4];
	float v[4];
	float r;

	v[0] = 3 * V*V*V;
	v[1] = V * 2;
	v[2] = 1;
	v[3] = 0;

	multMV(m, v, aux);

	r = U*U*U * aux[0] + U*U * aux[1] + U * aux[2] + aux[3];

	return r;

}

void multcr(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

int32_t bezieraux(float px[4][4], float py[4][4], float pz[4][4], int tesselation, FILE* output) {
	
	float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
	float pu1[3], pu2[3], pu3[3], pu4[3], pv1[3], pv2[3], pv3[3], pv4[3], re1[3], re2[3], re3[3], re4[3];
	float t = 1.0f / tesselation;
    	std::string coord;

		for (float i = 0; i < 1; i += t) {
		for (float j = 0; j < 1; j += t) {

		
			x1 = puv(i, j, px);
			pu1[0] = bu(i, j, px);
			pv1[0] = bv(i, j, px);

			x2 = puv(i + t, j, px);
			pu2[0] = bu(i + t, j, px);
			pv2[0] = bv(i + t, j, px);

			x3 = puv(i + t, j + t, px);
			pu3[0] = bu(i + t, j + t, px);
			pv3[0] = bv(i + t, j + t, px);

			x4 = puv(i, j + t, px);
			pu4[0] = bu(i , j + t, px);
			pv4[0] = bv(i , j + t, px);


			y1 = puv(i, j, py);
			pu1[1] = bu(i, j, py);
			pv1[1] = bv(i, j, py);

			y2 = puv(i + t, j, py);
			pu2[1] = bu(i+t, j, py);
			pv2[1] = bv(i+t, j, py);

			y3 = puv(i + t, j + t, py);
			pu3[1] = bu(i+t, j+t, py);
			pv3[1] = bv(i+t, j+t, py);

			y4 = puv(i, j + t, py);
			pu4[1] = bu(i, j+t, py);
			pv4[1] = bv(i, j+t, py);


			z1 = puv(i, j, pz);
			pu1[2] = bu(i, j, pz);
			pv1[2] = bv(i, j, pz);

			z2 = puv(i + t, j, pz);
			pu2[2] = bu(i+t, j, pz);
			pv2[2] = bv(i, j, pz);

			z3 = puv(i + t, j + t, pz);
			pu3[2] = bu(i+t, j+t, pz);
			pv3[2] = bv(i+t, j+t, pz);

			z4 = puv(i, j + t, pz);
			pu4[2] = bu(i, j+t, pz);
			pv4[2] = bv(i, j+t, pz);

			multcr(pu1, pv1, re1);
			normalize(&re1[0], &re1[1], &re1[2]);
			
			multcr(pu2, pv2, re2);
			normalize(&re2[0], &re2[1], &re2[2]);
			
			multcr(pu3, pv3, re3);
			normalize(&re3[0], &re3[1], &re3[2]);
			
			multcr(pu4, pv4, re4);
			normalize(&re4[0], &re4[1], &re4[2]);

			coord = std::to_string(x1) + std::to_string(y1) + std::to_string(z1) + std::to_string(re1[0]) + std::to_string(re1[1]) + std::to_string(re1[2]) + std::to_string(j) + std::to_string(i);
			write_file(coord, x1, y1, z1, re1[0], re1[1], re1[2], j, i, output);
			
			coord = std::to_string(x2) + std::to_string(y2) + std::to_string(z2) + std::to_string(re2[0]) + std::to_string(re2[1]) + std::to_string(re2[2]) + std::to_string(j) + std::to_string(i+t);
			write_file(coord, x2, y2, z2, re2[0], re2[1], re2[2],j,i+t, output);
			
			coord = std::to_string(x4) + std::to_string(y4) + std::to_string(z4) + std::to_string(re4[0]) + std::to_string(re4[1]) + std::to_string(re4[2]) + std::to_string(j+t) + std::to_string(i);
			write_file(coord, x4, y4, z4, re4[0], re4[1], re4[2],j+t,i, output);
			
			coord = std::to_string(x2) + std::to_string(y2) + std::to_string(z2) + std::to_string(re2[0]) + std::to_string(re2[1]) + std::to_string(re2[2]) + std::to_string(j) + std::to_string(i+t);
			write_file(coord, x2, y2, z2, re2[0], re2[1], re2[2],j,i+t, output);
			
			coord = std::to_string(x3) + std::to_string(y3) + std::to_string(z3) + std::to_string(re3[0]) + std::to_string(re3[1]) + std::to_string(re3[2]) + std::to_string(j+t) + std::to_string(i+t);
			write_file(coord, x3, y3, z3, re3[0], re3[1], re3[2],j+t,i+t, output);
			
			coord = std::to_string(x4) + std::to_string(y4) + std::to_string(z4) + std::to_string(re4[0]) + std::to_string(re4[1]) + std::to_string(re4[2]) + std::to_string(j+t) + std::to_string(i);
			write_file(coord, x4, y4, z4, re4[0], re4[1], re4[2],j+t,i, output);
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
    */
	if (!strcmp(argv[1], "patch")) {
		if (argc != 5) {
			err = -1;
			goto clean;
		}
		strcat(tmp, argv[4]);
		strcpy(argv[4], tmp);
		err = gen_bezier(argv[2], atof(argv[3]), argv[4]);
	}
    
clean:
	return err;
}
