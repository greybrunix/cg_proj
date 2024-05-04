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
void plane::get_vector(point r)
{
    int i;
    for (i=0; i<4;i++)
        r[i] = this->v[i];
}
float plane::get_distance(){return this->ori_to_plane;}
float plane::compute_distance_to_point(point p)
{
    float a,b,c,d;
    float v[4]; this->get_vector(v);
    a = v[0], b = v[1], c = v[2], d = this->get_distance();
    float numerator = abs(a * p[0] + b * p[1] + c * p[2] + d);
    float denominator = sqrtf(a * a + b * b + c * c);
    return numerator / denominator;
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

bool AABB::is_in_frustum(frustum fr)
{
    bool res = true;

    plane frustum_planes[6] = {fr.top_face,
        fr.bottom_face, fr.near_face,
        fr.far_face, fr.left_face,
        fr.right_face};

    for (auto& plane : frustum_planes) {
        float p_vertex[4], n_vertex[4];

        p_n_vertices(p_vertex, n_vertex, plane);

        if (plane.compute_distance_to_point(p_vertex) < 0) {
            res = false;
            break;
        }
    }

    return res;
}
