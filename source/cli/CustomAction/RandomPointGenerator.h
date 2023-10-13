#pragma once

#include <random>

class RandomPointGenerator
{
public:
    RandomPointGenerator();
    ~RandomPointGenerator() = default;

public:
    void generate_point(int32_t x, int32_t y, int32_t w, int32_t h, int32_t& rx, int32_t& ry);

private:
    std::mt19937 rng_;
};