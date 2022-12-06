#pragma once

#include "../glm/glm/glm.hpp"
#include <vector>
class OBJLoader{
public:
    OBJLoader();
    bool loadOBJ(const char* path);

private:
    

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<unsigned int> vertices_indices;
    std::vector<unsigned int> uvs_indices;
    std::vector<unsigned int> normals_indices;
};

