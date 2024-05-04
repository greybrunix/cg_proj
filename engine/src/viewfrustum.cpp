#include "viewfrustum.hpp"

plane::plane(float* v, float distance)
{
    int i;
    for (i=0; i<4; i++)
        this->v[i] = v[i];
    this->ori_to_plane = distance;
}
void plane::normal_sign(bool* x, bool* y, bool* z)
{
    *x = this->v[0] >= 0? true : false;
    *y = this->v[1] >= 0? true : false;
    *z = this->v[2] >= 0? true : false;
}
AABB::AABB(point mat)
{
    int i;
    for (i=0; i < 32; i++) {
        this->box[i/4][i%4] = mat[i];
    }
}
void AABB::p_n_vertices(float* p_vertex, float* n_vertex,
                   plane pl_nor)
{
    bool x,y,z;
    int i;
    float min_x, min_y, min_z,
          max_x, max_y, max_z;
    pl_nor.normal_sign(&x,&y,&z);
    min_x = box[0][0], min_y = box[0][1], min_z = box[0][2];
    max_x = box[0][0], max_y = box[0][1], max_z = box[0][2];
    for (i = 1; i < 8; ++i) {
        min_x = std::min(min_x, this->box[i][0]);
        min_y = std::min(min_y, this->box[i][1]);
        min_z = std::min(min_z, this->box[i][2]);
        max_x = std::max(max_x, this->box[i][0]);
        max_y = std::max(max_y, this->box[i][1]);
        max_z = std::max(max_z, this->box[i][2]);
    }

    p_vertex[0] = x ? max_x : min_x;
    p_vertex[1] = y ? max_y : min_y;
    p_vertex[2] = z ? max_z : min_z;
    p_vertex[3] = 1.F;

    n_vertex[0] = x ? min_x : max_x;
    n_vertex[1] = y ? min_y : max_y;
    n_vertex[2] = z ? min_z : max_z;
    n_vertex[3] = 1.F;
}

bool is_in_frustum(frustum fr)
{
    bool res = false;
    return true;
}
