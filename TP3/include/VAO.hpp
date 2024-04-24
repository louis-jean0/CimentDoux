#pragma once

class VAO {
    private:
        unsigned int VertexAttribObject;
    public:
        VAO();
        void genVAO();
        void delVAO();

        unsigned int getVAO();
};