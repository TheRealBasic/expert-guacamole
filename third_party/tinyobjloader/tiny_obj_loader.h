#ifndef TINY_OBJ_LOADER_H
#define TINY_OBJ_LOADER_H
#include <string>
#include <vector>

namespace tinyobj {
struct attrib_t {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;
};

struct index_t {
    int vertex_index;
    int normal_index;
    int texcoord_index;
};

struct mesh_t {
    std::vector<unsigned char> num_face_vertices;
    std::vector<index_t> indices;
};

struct shape_t {
    mesh_t mesh;
};

struct material_t {
    std::string name;
};

bool LoadObj(attrib_t* attrib,
             std::vector<shape_t>* shapes,
             std::vector<material_t>* materials,
             std::string* warn,
             std::string* err,
             const char* filename,
             const char* basepath = nullptr,
             bool triangulate = true,
             bool default_vcols_fallback = true);
}

#endif
