#include <iostream>
#include <SDL.h>
#include "Sim/SimManager.h"

int main(int argc, char* argv[]) {
    SimManager simManager{};
    simManager.loop();

    return 0;
}
