#!/bin/bash

SHADER_PATH="shaders/fs.frag"
BUILD_DIR="build"
content=$(cat "$SHADER_PATH")

# Vérifier si le fichier se termine par un espace
if [[ "$content" =~ \ $ ]]; then
    # Si oui, enlever l'espace
    echo "${content% }" > "$SHADER_PATH"
else
    # Sinon, ajouter un espace
    echo "$content " > "$SHADER_PATH"
fi

cd "$BUILD_DIR"
make -j 24
./main

# DE LA PURE FOLIE DE DEVOIR FAIRE CECI
