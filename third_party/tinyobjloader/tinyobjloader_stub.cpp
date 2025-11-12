#include "tiny_obj_loader.h"

namespace tinyobj {
bool LoadObj(attrib_t* attrib,
             std::vector<shape_t>* shapes,
             std::vector<material_t>* materials,
             std::string* warn,
             std::string* err,
             const char*,
             const char*,
             bool,
             bool) {
    if (attrib) attrib->vertices = {0.f, 0.f, 0.f};
    if (shapes) shapes->push_back(shape_t{});
    if (materials) materials->push_back(material_t{});
    if (warn) warn->clear();
    if (err) err->clear();
    return true;
}
}
