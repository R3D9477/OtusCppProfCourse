#include "lalloc10.h"

#include <map>
#include <vector>
#include <iostream>

#include "lalloc10.h"
#include "factorial.h"
#include "customcontainer.h"

int main()
{
    std::map<int, int> fmap;
    for (int key = 0; key < 10; ++key) fmap.insert({ key, calc_factorial(key) });
    for (auto [key,value]: fmap) std::cout << key << " " << value << std::endl;
    std::cout << std::endl;

    std::map<int, int, std::less<int>, LAlloc10<std::pair<const int,int>>> flmap;
    for (int key = 0; key < 10; ++key) flmap.insert({ key, calc_factorial(key) });
    for (auto [key,value]: flmap) std::cout << key << " " << value << std::endl;
    std::cout << std::endl;

    CustomContainer<int,10> ic1;
    for (size_t i = 0; i < ic1.size(); ++i) ic1.push_back(i);
    for (auto elem: ic1) std::cout << elem << " ";
    std::cout << std::endl;

    CustomContainer<int, 10, LAlloc10<int>> ic2;
    for (size_t i = 0; i < 10; ++i) ic2.push_back(i);
    for (auto elem: ic2) std::cout << elem << " ";
    std::cout << std::endl;

    return 0;
}
