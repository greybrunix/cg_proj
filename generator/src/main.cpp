/* Output a 3d model file*/
#include <cstddef>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct pair {
	float x,z;
};

int gen_plane(float full_size, int divs, char* file)
{
	FILE* output = fopen(file, "w+"); 
	char buff[512];
	size_t b_read;

	struct pair curr, tmp;
	int err = 0, i,j,
	    tot = (divs+1)*(divs+1);
	float div_len = full_size / divs,
	      off = !(divs % 2) ? 0 : div_len/2,
	      x,z;

	for (i=0; i < divs; i++) {
		for (j=0; j < divs; j++) {
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", curr.x, curr.z);
			fwrite(buff, sizeof (char),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", curr.x, curr.z);
			fwrite(buff, sizeof (char),b_read, output);
			
			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", curr.x, curr.z);
			fwrite(buff, sizeof (char),b_read, output);
			fwrite(buff, sizeof (char),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n", tmp.x, tmp.z);
			fwrite(buff, sizeof (char),b_read, output);

			b_read = sprintf(buff, "%.3f 0.000 %.3f\n\n", curr.x, curr.z);
			fwrite(buff, sizeof (char),b_read, output);
		}
	}

	fclose(output);
	return err;
}

int main(int argc, char**argv)
{
	int err = 0;
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
