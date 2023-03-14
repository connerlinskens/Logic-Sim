#include <iostream>
#include <SDL.h>
#include "SimManagers/SimManager.h"

int main(int argc, char* argv[]) {
//    SimManager simManager{1280, 720, false};
    SimManager simManager{};
    simManager.loop();

    return 0;
}
