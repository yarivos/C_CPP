#include <cstdlib>
#include <iostream>
#include "HashMap.hpp"

//
//#define INITIAL_SIZE 16
//
int main()
{
    HashMap<int,int> hm;

//    hm.insert(0,1);
//    hm.insert(16,1);
    HashMap<int, int> h1 ({1, 2, 1, 4, 5}, {10, 20, 30, 40, 50});

    for (const auto& it : h1)
    {
        std::cout << it.first << "\t" << it.second << std::endl;
    }

//    h1.insert()

    std::vector<int> keys;
    std::vector<int> values;
    keys.push_back(1);
    values.push_back(1);

    /* Create the map */
    HashMap<int, int> map(keys, values);
    map.insert(55,55);
    map.insert(5,5);
    return 0;
}