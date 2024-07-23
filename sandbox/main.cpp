#include "wen.hpp"

int main() {
    auto manager = new wen::Manager;
    manager->initializeEngine();



    manager->destroyEngine();

    manager = nullptr;
    delete manager;

    return 0;
}