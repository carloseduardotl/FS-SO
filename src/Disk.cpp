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
        blocks[i].name = '0';
        blocks[i].next_block = -1;
        blocks[i].process_id = 0;
    }
}

int Disk::get_number_of_blocks()
{
    return blocks.size();
}

std::vector<block> Disk::get_blocks()
{
    return blocks;
}

int Disk::get_number_of_free_blocks()
{
    int number_of_free_blocks = 0;
    for(int i=0; i<blocks.size(); i++)
    {
        if(blocks[i].name == '0')
        {
            number_of_free_blocks++;
        }
    }
    return number_of_free_blocks;
}

int Disk::get_contiguous_free_blocks(int position)
{
    int number_of_free_blocks = 0;
    for(int i=position; i<blocks.size(); i++)
    {
        if(blocks[i].name == '0')
        {
            number_of_free_blocks++;
        }
        else
        {
            break;
        }
    }
    return number_of_free_blocks;
}

bool Disk::start_file(char name, int starting_block, int size)
{
    if(starting_block + size > blocks.size())
    {
        return false;
    }
    for(int i=starting_block; i<starting_block+size; i++)
    {
        blocks[i].name = name;
    }
    return true;
}

bool Disk::add_file(char name, int size, int process_id)
{
    if(get_number_of_free_blocks() < size)
    {
        return false;
    }
    switch (current_allocation)
    {
    case contiguous:
        for(int i=0; i<blocks.size(); i++)
        {
            if(blocks[i].name == '0')
            {
                if(get_contiguous_free_blocks(i) >= size)
                {
                    for(int j=i; j<i+size; j++)
                    {
                        blocks[j].name = name;
                        blocks[j].process_id = process_id;
                    }
                    return true;
                }
            }
        }
        return false;
        break;
    
    default:
        break;
    }
}