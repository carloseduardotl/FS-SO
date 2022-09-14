#pragma once

#include <vector>

enum allocation
{
    contiguous,
    linked,
    indexed
};

class Disk 
{
    private:
    allocation current_allocation;
    std::vector<char> blocks;

    public:

    void set_current_state(allocation state);
    allocation get_current_state();

    void set_number_of_blocks(int number_of_blocks);
    int get_number_of_blocks();
};