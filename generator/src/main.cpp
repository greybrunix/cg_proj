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

	struct pair curr, tmp;
	int32_t err = 0, i,j;
	    /*tot = (divs+1)*(divs+1);*/
	float div_len = full_size / divs,
	      off = !(divs % 2) ? 0 : div_len/divs,
	      x,z;

	for (i=0; i < divs; i++) {
		for (j=0; j < divs; j++) {
			//curr.x = x = i * div_len - div_len + off;
			curr.x = x = i * div_len + off;
			curr.z = z = j * div_len - div_len + off;
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", curr.x, curr.z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			curr.x = tmp.x = x;
			curr.z = tmp.z = z + div_len;
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", curr.x, curr.z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			
			curr.x = x - div_len;
			curr.z = z;
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", curr.x, curr.z);
			fwrite(buff, sizeof (int8_t),b_read, output);
			fwrite(buff, sizeof (int8_t),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", tmp.x, tmp.z);
			fwrite(buff, sizeof (int8_t),b_read, output);

			curr.x = tmp.x - div_len;
			curr.z = tmp.z;
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n\n", curr.x, curr.z);
			fwrite(buff, sizeof (int8_t),b_read, output);
		}
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


clean:
	return err;
}
