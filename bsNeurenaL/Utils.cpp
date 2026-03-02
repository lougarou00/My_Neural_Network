#include "perceptron.hpp"
#include <random>

double getRandomFloatOptimized()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> distrib(0.0, 1.0);
    
    return distrib(gen);
}

float computeXor(float a, float b)
{
    if (a != b)
        return 1.f;
    return 0.f;
}

int getRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(min, max);
    
    return distrib(gen);
}
