#pragma once

#include <iostream>
#include <vector>

class VBO {
    private:
        unsigned int VertexBufferObject;
    public:
        VBO();
        void genVBO(std::vector<float> verts);
        void delVBO();
};