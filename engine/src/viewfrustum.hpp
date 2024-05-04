#include "transforms.hpp"

class plane
{
private:
    float v[4]          = {0.F, 1.F, 0.F, 0.F};
    float ori_to_plane{ 0.F };

public:
    plane(float* v, float distance);
    void normal_sign(bool* x, bool* y, bool* z);
    void get_vector(point r);
    float get_distance();
    float compute_distance_to_point(point p);
};

typedef struct frustum
{
    plane top_face;
    plane bottom_face;

    plane right_face;
    plane left_face;

    plane far_face;
    plane near_face;
} frustum;
frustum create_from_camera(camera cam);

class AABB
{
private:
    float box[8][4]{{0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F},
                    {0.F,0.F,0.F,1.F}};
    void p_n_vertices(float* p_vertex, float* n_vertex,
                      plane pl_nor);
public:
    AABB(point mat);
    bool is_in_frustum(frustum fr);
};

