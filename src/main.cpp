#include <Disk.h>
#include <iostream>

int main()
{
    int blocks = 0;
    Disk disk;

    std::cin >> blocks;
    disk.set_number_of_blocks(blocks);
    std::cout << disk.get_number_of_blocks() << std::endl;
    return 0;
}