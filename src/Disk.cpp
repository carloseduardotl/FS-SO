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
    for(int i=0; i<int(processes.size()); i++)
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

void Disk::set_error_msg(std::string msg)
{
    error_msg = msg;
}

std::string Disk::get_error_msg()
{
    return error_msg;
}

allocation Disk::get_current_state()
{
    return current_allocation;
}

void Disk::set_number_of_blocks(int number_of_blocks)
{
    blocks.resize(number_of_blocks);
    for(int i=0; i<int(blocks.size()); i++)
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
    for(int i=0; i<int(blocks.size()); i++)
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
    for(int i=position; i<int(blocks.size()); i++)
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
    if(starting_block + size > int(blocks.size()))
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
        set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (falta de espaço)");
        return false;
    }
    if(!process_exists(process_id))
    {
        set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (processo não existe)");
        return false;
    }
    switch (current_allocation)
    {
    case contiguous:
        for(int i=0; i<int(blocks.size()); i++)
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
        set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (falta de espaço)");
        return false;
        break;
    case linked:
    {
        int real_size = ceil(size * 1.1);
        int allocated_size = real_size;
        int previous_block = -1;
        for(int i=0; i<int(blocks.size()); i++)
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
    return false;
}

bool Disk::delete_file(char name, int origin_process_id)
{   
    process current_process;
    bool process_found = false;
    for(int i=0; i<int(processes.size()); i++)
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
    for(int i=0; i<int(blocks.size()); i++)
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