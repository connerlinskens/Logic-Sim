#include <iostream>
#include <SDL.h>
#include "Sim/SimManager.h"

int main(int argc, char* argv[]) {
    SimManager simManager{1280, 720, false};
    simManager.loop();

    return 0;
}
