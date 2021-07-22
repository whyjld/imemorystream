#include "../include/imemorystream.h"

#include <iostream>
#include <string>

int main(int, char*[])
{
    char buffer[] = "1 2.2 buffer \12";
    imemorystream ims(buffer, sizeof(buffer));

    int int_value;
    double double_value;
    std::string str_value;

    ims >> int_value >> double_value >> str_value;
    std::cout << int_value << std::endl;
    std::cout << double_value << std::endl;
    std::cout << str_value << std::endl;
    ims.seekg(0, std::ios::beg);
    for (; !ims.eof();)
    {
        std::cout << ims.get() << std::endl;
    }

    return 0;
}