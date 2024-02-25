/* Output a 3d model file*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>

struct pair {
	float x,z;
};

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
			
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x-off, z+off);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", x-off, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n\n", x-off, z+off);
			fwrite(buff, sizeof (int8_t),b_read, output);

			x -= off;
		}
		x = full_size/ 2;
		z += off;
	}

	fclose(output);
	return err;
}
void drawSphere(float radius, int slices, int stacks) {
	float px, py, pz;
	float alfa_diff = 2 * M_PI / slices;
	float beta_diff = M_PI / stacks;
	float alfa = 0;
	float beta = 0;
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < stacks; j++) {
			px = radius * cos(beta - M_PI_2) * cos(alfa);
			py = radius * sin(beta - M_PI_2);
			pz = radius * cos(beta - M_PI_2) * sin(alfa);
			glVertex3f(px, py, pz);
			px = radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa + alfa_diff);
			py = radius * sin(beta - M_PI_2 + beta_diff);
			pz = radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa + alfa_diff);
			glVertex3f(px, py, pz);
			px = radius * cos(beta - M_PI_2) * cos(alfa + alfa_diff);
			py = radius * sin(beta - M_PI_2);
			pz = radius * cos(beta - M_PI_2) * sin(alfa + alfa_diff);
			glVertex3f(px, py, pz);
			px = radius * cos(beta - M_PI_2) * cos(alfa);
			py = radius * sin(beta - M_PI_2);
			pz = radius * cos(beta - M_PI_2) * sin(alfa);
			glVertex3f(px, py, pz);
			px = radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa);
			py = radius * sin(beta - M_PI_2 + beta_diff);
			pz = radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa);
			glVertex3f(px, py, pz);
			px = radius * cos(beta - M_PI_2 + beta_diff) * cos(alfa + alfa_diff);
			py = radius * sin(beta - M_PI_2 + beta_diff);
			pz = radius * cos(beta - M_PI_2 + beta_diff) * sin(alfa + alfa_diff);
			glVertex3f(px, py, pz);
			beta += beta_diff;
		}
		beta = 0;
		alfa += alfa_diff;
	}
	glEnd();
}

int32_t main(int32_t argc, char**argv)
{
	int32_t err = 0;
	if (argc < 2) {
		err = -1;
		goto clean;
	}
	if (!strcmp(argv[1], "plane")) {
		if (argc != 5) {
			err = -1;
			goto clean;	
		}
		err = gen_plane(atof(argv[2]),
				atoi(argv[3]),
				argv[4]);
	}


clean:
	return err;
}
