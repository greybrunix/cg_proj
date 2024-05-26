struct triple {
	float x, y, z;
};

float dot(const float* v, const float* u);
float len(const float* v);
void normalize(float *v);
void scalar(const float* v, const float scalar, float* r);
void cross(const float* v, const float* u, float* r);
