#include <Disk.h>

void Disk::set_current_state(int state)
{
    switch (state)
    {
    case 1:
        current_allocation = contiguous;
        break;
    case 2:
        current_allocation = linked;
        break;
    case 3:
        current_allocation = indexed;
        break;    
    default:
        break;
    }
}

allocation Disk::get_current_state()
{
    return current_allocation;
}

void Disk::set_number_of_blocks(int number_of_blocks)
{
    blocks.resize(number_of_blocks);
    for(int i=0; i<blocks.size(); i++)
    {
        blocks[i] = '0';
    }
}

int Disk::get_number_of_blocks()
{
    return blocks.size();
}

std::vector<char> Disk::get_blocks()
{
    return blocks;
}

bool Disk::add_file(char name, int starting_block, int size)
{
    if(starting_block + size > blocks.size())
    {
        return false;
    }
    for(int i=starting_block; i<starting_block+size; i++)
    {
        blocks[i] = name;
    }
    return true;
}