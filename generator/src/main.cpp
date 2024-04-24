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
        b_read = snprintf(buff, 512, "%.3f, %.3f, %.3f %u\n", x, y, z, i);
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
    std::string coord;
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) { /* ifs */
			if(j!=0){
                px = radius * cosf(beta - M_PI_2) * cosf(alpha);
                py = radius * sinf(beta - M_PI_2);
                pz = radius*cosf(beta-M_PI_2)*sinf(alpha); 
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
                write_file(coord, px, py, pz, output);

                px = radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha + alpha_diff);
                py = radius * sinf(beta - M_PI_2 + beta_diff);
                pz = radius*cosf(beta-M_PI_2+beta_diff) * sinf(alpha + alpha_diff); 
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
                write_file(coord, px, py, pz, output);

                px = radius * cosf(beta - M_PI_2) * cosf(alpha + alpha_diff);
                py = radius * sinf(beta - M_PI_2);
                pz = radius*cosf(beta-M_PI_2)*sinf(alpha + alpha_diff);
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
                write_file(coord, px, py, pz, output);
			}
			if(j!=stacks-1){
                px = radius * cosf(beta - M_PI_2) * cosf(alpha);
                py = radius * sinf(beta - M_PI_2);
                pz = radius*cosf(beta-M_PI_2)*sinf(alpha);
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
                write_file(coord, px, py, pz, output);
                
                px = radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha);
                py = radius * sinf(beta - M_PI_2 + beta_diff);
                pz = radius*cosf(beta-M_PI_2 + beta_diff)*sinf(alpha);
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
                write_file(coord, px, py, pz, output);

                px = radius * cosf(beta - M_PI_2 + beta_diff) * cosf(alpha + alpha_diff);
                py = radius * sinf(beta - M_PI_2 + beta_diff);
                pz = radius*cosf(beta-M_PI_2+beta_diff)*sinf(alpha + alpha_diff);
                coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
                write_file(coord, px, py, pz, output);
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
       
        if (i==stacks-1){              
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
	float diff = l/d;

    // Bottom and Top Faces
	for (i=0; i<d; i++){
		for (j=0; j<d; j++) {
			
            px = x;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            pz = pz+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = x;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            
            px = x+diff;
            py = -y;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            py = -y;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
         
            px = x;
            py = -y;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

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

            px = x+diff;
            py = y+diff;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = x;
            py = y+diff;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y+diff;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            py = y+diff;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = x;
            py = y;
            pz = -z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            py = y+diff;
            pz = -z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            py = y;
            pz = -z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = x;
            py = y;
            pz = -z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y+diff;
            pz = -z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x+diff;
            py = y+diff;
            pz = -z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

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

            px = x;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y+diff;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = x;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y+diff;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            py = y+diff;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = -x;
            py = y+diff;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = -x;
            py = y;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = -x;
            py = y;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

            px = -x;
            py = y+diff;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = -x;
            py = y;
            z = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = -x;
            py = y+diff;
            pz = z+diff;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

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
    std::string coord;
	float x = full_size/2, z = -x, off=full_size/divs;
    float px, py=0.f, pz;
	int i,j, err = 0;

	for (i=0; i < divs; i++) {
		for (j=0; j < divs; j++) {
			//curr.x = x = i * div_len - div_len + off;
            px = x;
            pz = z+off;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x-off;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x-off;
            pz = z;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x-off;
            pz = z+off;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);
            px = x;
            pz = z+off;
            coord = std::to_string(px) + std::to_string(py) + std::to_string(pz);
            write_file(coord, px, py, pz, output);

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

int32_t gen_cylinder(float radius, float height, int32_t slices, char* file) {
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

clean:
	return err;
}
