//
// Created by Conner on 3/9/2023.
//

#include <iostream>
#include "RandomService.h"

std::unique_ptr<RandomService> RandomService::_instance = {};

RandomService* RandomService::Instance() {
    if(_instance){
        return _instance.get();
    }
    else{
        _instance = std::make_unique<RandomService>(RandomService());
        return _instance.get();
    }
}

RandomService::RandomService() {
    std::random_device r;
    _random = {std::default_random_engine (r())};
}

int RandomService::Random(int min, int max) {
    std::uniform_int_distribution<int> dist{min, max};
    return dist(_random);
}

double RandomService::Random() {
    std::uniform_real_distribution<double> dist{0.0,1.0};
    return dist(_random);
}
