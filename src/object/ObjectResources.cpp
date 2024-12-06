#include "ObjectResources.h"

#define PRIME_CONST 31


bool ObjectResources::addTexture(const std::string key) {
    if (textureFiles.count(key) == 0){
        try {
            auto texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(key));
            textureFiles[key] = std::move(texture);
            std::cout << "Loaded texture: " << key << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to load texture: " << key << ". Error: " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}

bool ObjectResources::addMesh(const std::string key) {
    if (meshFiles.count(key) == 0){
        try {
            auto mesh = std::make_unique<ppgso::Mesh>(key);
            meshFiles[key] = std::move(mesh);
            std::cout << "Loaded model: " << key << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to load model: " << key << ". Error: " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}

ppgso::Texture *ObjectResources::getTexture(const std::string code) {
    return textureFiles[code].get();
}

ppgso::Mesh *ObjectResources::getMesh(const std::string code) {
    ppgso::Mesh* a = meshFiles[code].get();
    return a;
}
