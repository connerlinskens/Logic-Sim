//
// Created by Conner on 3/9/2023.
//

#ifndef LOGIC_SIM_RANDOMSERVICE_H
#define LOGIC_SIM_RANDOMSERVICE_H

#include <random>
#include <memory>

class RandomService {
public:
    static RandomService* Instance();

    int Random(int min, int max);
    double Random();
private:
    RandomService();
private:
    static std::unique_ptr<RandomService> _instance;
    std::default_random_engine _random;
};


#endif //LOGIC_SIM_RANDOMSERVICE_H
