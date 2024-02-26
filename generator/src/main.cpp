/* Output a 3d model file*/
#include <bits/types/FILE.h>
#include <cstddef>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>

struct pair {
	float x,z;
};

int32_t gen_box(float l, int32_t d, char* file)
{
	FILE* output = fopen(file, "w+");
	char buff[512];
	size_t b_read;
	int32_t i, j, r = 0;
	float x = l/2;
	float y = l/2;
	float z = -l/2;
	float diff = l/d;

    // Bottom and Top Faces
	for (i=0; i<d; i++){
		for (j=0; j<d; j++) {
			

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, -y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, -y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, -y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, -y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, -y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, -y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			x -= diff;
		}
		x = l/2;
		z += diff;
	}

	x = l/2;
	z = -l/2;

    // Right and Left faces
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++) {

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y-diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y-diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y-diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y-diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y-diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", -x, y-diff, z+diff);
			fwrite(buff, sizeof (int8_t),b_read, output);

			z += diff;
		}
		z = -l/2;
		y -= diff;
	}
    
	y = l/2;
	z = l/2;

    // Front and Back faces
	for (int i=0; i<d; i++){
		for (int j=0; j<d; j++) {

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y-diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y-diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y-diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y-diff, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y-diff, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x, y, z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			b_read = snprintf(buff, 512, "%.3f %.3f %.3f\n", x-diff, y-diff, -z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			x -= diff;
		}
		x = l/2;
		y -= diff;
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
	if (!strcmp(argv[1], "box")) {
		if (argc != 5) {
			err = -1;
			goto clean;
		}
		err = gen_box(atof(argv[2]),
			      atoi(argv[3]),
			      argv[4]);
	}


clean:
	return err;
}
