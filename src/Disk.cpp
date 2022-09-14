#include <Disk.h>

void Disk::set_current_state(allocation state)
{
    current_allocation = state;
}

allocation Disk::get_current_state()
{
    return current_allocation;
}

void Disk::set_number_of_blocks(int number_of_blocks)
{
    blocks.resize(number_of_blocks);
}

int Disk::get_number_of_blocks()
{
    return blocks.size();
}