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

process Disk::get_process(int id)
{
    for(int i=0; i<int(processes.size()); i++)
    {
        if(processes[i].id == id)
        {
            return processes[i];
        }
    }
    process p; // return empty process
    return p;
}

void Disk::set_process_time_count(int id, int time_count)
{
    for(int i=0; i<int(processes.size()); i++)
    {
        if(processes[i].id == id)
        {
            processes[i].time_counter = time_count;
        }
    }
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
    // Defining the type of allocation
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
        blocks[i].name = '0'; // get the name of the file
        blocks[i].next_block = -1; //get the next block
        blocks[i].process_id = 0; // get the process id
        blocks[i].indexed_file = -1; // get the indexed file
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

bool Disk::add_file(char name, int size, int process_id, std::vector<int> * blocks_vector)
{
    if(get_number_of_free_blocks() < size)
    {
        set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (falta de espaço).\n");
        return false;
    }
    if(!process_exists(process_id))
    {
        set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (processo não existe).\n");
        return false;
    }
    switch (current_allocation)
    {
    case contiguous:
        for(int i=0; i<int(blocks.size()); i++)
        {
            if(blocks[i].name == '0')
            {
                if(get_contiguous_free_blocks(i) >= size) // Check if there are enough free blocks
                {
                    for(int j=i; j<i+size; j++)
                    {
                        blocks[j].name = name; // get the name of the file
                        blocks[j].process_id = process_id; // get the process id
                        blocks_vector->push_back(j); // get the block number
                    }
                    return true;
                }
            }
        }        
        set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (falta de espaço).\n");
        return false;
        break;
    case linked:
    {
        int real_size = ceil(size * 1.1);
        if(get_number_of_free_blocks() < real_size)
        {
            set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (falta de espaço).\n");
            return false;
        }
        int allocated_size = real_size;
        int previous_block = -1;
        for(int i=0; i<int(blocks.size()); i++)
        {
            if(blocks[i].name == '0')
            {
                blocks[i].name = name; // get the name of the file
                blocks[i].process_id = process_id; //get the process id
                allocated_size--; //get the next block
                if(previous_block != -1) // get the indexed file
                {
                    blocks[previous_block].next_block = i;
                }
                previous_block = i;
                blocks_vector->push_back(i); // get the block number
            }
            if(allocated_size == 0)
            {
                return true;
            }
        }
        return false;
        break;
    }
    case indexed:
    {
        bool first_allocated_block = true;
        if(get_number_of_free_blocks() < size+1)
        {
            set_error_msg("O processo " + std::to_string(process_id) + " não pode criar o arquivo " + name + " (falta de espaço).\n");
            return false;
        }
        for(size_t i=0; i<blocks.size(); i++)
        {
            if(blocks[i].name == '0')
            {
                if(first_allocated_block == true)
                {
                    blocks[i].name = 'I'; // get the name of the file
                    blocks[i].process_id = process_id; // get the process id
                    blocks[i].indexed_file = name; // get the indexed file
                    first_allocated_block = false; // get the next block
                    blocks_vector->push_back(i);    //get the block number
                }
                else
                {
                    blocks[i].name = name; // get the name of the file
                    blocks[i].process_id = process_id; // get the process id
                    size--; // get the next block
                    blocks_vector->push_back(i); // get the block number
                }
            }
            if(size <= 0) break;
        }
        return true;
        break;
    }
    default:
        break;
    }
    return false;
}

bool Disk::delete_file(char name, int origin_process_id)
{   
    process current_process;
    bool process_found = false;
    bool block_found = false;
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
        set_error_msg("O processo " + std::to_string(origin_process_id) + " não pode deletar o arquivo " + name + " (processo não existe).\n");
        return false;
    }
    bool file_deleted = false;
    for(int i=0; i<int(blocks.size()); i++)
    {
        if((blocks[i].name == name) || (blocks[i].indexed_file == name))
        {
            block_found = true;
            if((blocks[i].process_id == origin_process_id) || (current_process.priority == 0))
            {
            blocks[i].name = '0'; // get the name of the file
            blocks[i].process_id = 0; // get the process id
            blocks[i].next_block = -1; // get the next block
            blocks[i].indexed_file = -1; // get the indexed file
            file_deleted = true;
            }
        }
    }
    if(file_deleted)
    {
        return true;
    }
    if(!block_found)
    {  
        set_error_msg(get_error_msg() + "O processo " + std::to_string(origin_process_id) + " não pode deletar o arquivo " + name + " (arquivo não existe).\n");
        if(current_process.priority != 0)
        {
            set_error_msg(get_error_msg() + "O processo " + std::to_string(origin_process_id) + " não pode deletar um arquivo que não foi criado por ele.\n");
        }
        return false;
    }
    if(current_process.priority != 0)
    {
        set_error_msg(get_error_msg() + "O processo " + std::to_string(origin_process_id) + " não pode deletar um arquivo que não foi criado por ele.\n");
        return false;
    }
    return false;
}