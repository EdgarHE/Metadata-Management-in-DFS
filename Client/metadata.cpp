#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <functional>
using namespace std;

 
unsigned int SDBMHash(string a)
{
    unsigned int hash = 0;
    int i=0;

    while (a[i])
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (a[i++]) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

int main()
{
    std::string s = "Stand back! I've got jimmies!";
    //std::hash<std::string> hash_fn;
 
    //size_t hash = hash_fn(s);
 
    std::cout << SDBMHash(s) << '\n';
}