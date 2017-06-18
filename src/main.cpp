#include "../include/Viewer.hpp"
#include "../include/log.hpp"

#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

// forward declaration of scene building functions, in separate files
void initialize_scene(Viewer& viewer);
void speeder(Viewer& viewer);
void init_ground(Viewer& viewer);
void initialize_sceneTree(Viewer& viewer);
void global(Viewer & viewer);
void scene_foret(Viewer& viewer);

int main(int argc, char* argv[])
{
    Viewer viewer(1280, 720);   // window size in pixels
    srand(time(NULL));

    unsigned int scene_to_load = 1;

    for (unsigned int i = 1; i < argc; ++i) {
        std::istringstream tokenizer(std::string(argv[i]));
        tokenizer >> scene_to_load;
        if (tokenizer.fail()) {
            LOG(fatal, "unknown argument " << argv[i] << ". Please launch without argument or the number of the practical you want to load");
            exit(EXIT_FAILURE);
        }
    }

    switch (scene_to_load) {
        case 1:
            global(viewer);
            break;
        case 2:
            speeder(viewer);
            break;
        case 3:
            init_ground(viewer);
            break;
        case 4:
            initialize_sceneTree(viewer);
            break;
        case 5:
            scene_foret(viewer);
            break;
        default:
            LOG(fatal, "unknown practical number " << scene_to_load);
            exit(EXIT_FAILURE);
    }

    while (viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}
