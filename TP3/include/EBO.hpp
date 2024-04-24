#pragma once

#include <iostream>
#include <vector>

class EBO {
    private:
        unsigned int ElementsBufferObject;
    public:
        EBO();

        // Fonction pour générer l'EBO
        void genEBO(std::vector<unsigned int> indics);

        // Fonction pour supprimer l'EBO
        void delEBO();
};