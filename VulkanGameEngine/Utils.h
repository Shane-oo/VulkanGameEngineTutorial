//
// Created by ShaneMonck on 9/04/2024.
//

#ifndef VULKANGAMEENGINETUTORIAL_UTILS_H
#define VULKANGAMEENGINETUTORIAL_UTILS_H


#include <cstdio>
#include <typeindex>

template<typename T, typename... Rest>
void hashCombine(std::size_t &seed, const T &v, const Rest &...rest) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

class Utils {

};


#endif //VULKANGAMEENGINETUTORIAL_UTILS_H
