#include <Disk.h>

Disk::Disk()
{
    blocks.clear();
    processes.clear();
}

Disk::~Disk()
{
    blocks.clear();
    processes.clear();
}

void Disk::add_process(int id, int priority, int process_time)
{
    process p;
    p.id = id;
    p.priority = priority;
    p.process_time = process_time;
    processes.push_back(p);
}

std::vector<process> Disk::get_processes()
{
    return processes;
}

bool Disk::process_exists(int id)
{
    for(size_t i=0; i<processes.size(); i++)
    {
        if(processes[i].id == id)
        {
            return true;
        }
    }
    return false;
}

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
    for(size_t i=0; i<blocks.size(); i++)
    {
        blocks[i].name = '0';
        blocks[i].next_block = -1;
        blocks[i].process_id = 0;
        blocks[i].indexed_file = -1;
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
    for(size_t i=0; i<blocks.size(); i++)
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
    for(size_t i=position; i<blocks.size(); i++)
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
    if(!process_exists(process_id))
    {
        return false;
    }

    int real_size = ceil(size * 1.1);
    int allocated_size = real_size;
    int previous_block = -1;
    bool first_allocated_block = true;
    switch (current_allocation)
    {
    case contiguous:
        for(size_t i=0; i<blocks.size(); i++)
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
    case linked:
        if(get_number_of_free_blocks() < real_size)
        {
            return false;
        }
        for(size_t i=0; i<blocks.size(); i++)
        {
            if(blocks[i].name == '0')
            {
                blocks[i].name = name;
                blocks[i].process_id = process_id;
                allocated_size--;
                if(previous_block != -1)
                {
                    blocks[previous_block].next_block = i;
                }
                previous_block = i;
            }
            if(allocated_size == 0)
            {
                return true;
            }
        }
        return false;
        break;

    case indexed:
        if(get_number_of_free_blocks() < size+1)
        {
            return false;
        }
        for(int i=0; i<blocks.size(); i++)
        {
            if(blocks[i].name == '0')
            {
                if(first_allocated_block == true)
                {
                    blocks[i].name = 'I';
                    blocks[i].process_id = process_id;
                    blocks[i].indexed_file = name;
                    first_allocated_block = false;
                }
                else
                {
                    blocks[i].name = name;
                    blocks[i].process_id = process_id;
                    size--;
                }
            }
            if(size <= 0) break;
        }
        return true;
        break;
    default:
        break;
    }
}

bool Disk::delete_file(char name, int origin_process_id)
{   
    process current_process;
    bool process_found = false;
    for(size_t i=0; i<processes.size(); i++)
    {
        if(processes[i].id == origin_process_id)
        {
            current_process = processes[i];
            process_found = true;
            break;
        }
    }
    if(!process_found)
    {
        return false;
    }
    bool file_deleted = false;
    for(int i=0; i<blocks.size(); i++)
    {
        if(((blocks[i].name == name) || (blocks[i].indexed_file == name)) && ((blocks[i].process_id == origin_process_id) || current_process.priority == 0))
        {
            blocks[i].name = '0';
            blocks[i].process_id = 0;
            blocks[i].next_block = -1;
            blocks[i].indexed_file = -1;
            file_deleted = true;
        }
    }
    if(file_deleted)
    {
        return true;
    }
    return false;
}