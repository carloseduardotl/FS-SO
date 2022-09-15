#pragma once
#include <vector>

enum allocation
{
    contiguous,
    linked,
    indexed
};

typedef struct block
{
    char name;
    int next_block;
    int process_id;
} block;

class Disk 
{
    private:
    allocation current_allocation;
    std::vector<block> blocks;

    public:

    void set_current_state(int state);
    allocation get_current_state();

    void set_number_of_blocks(int number_of_blocks);
    int get_number_of_blocks();
    std::vector<block> get_blocks();
    int ger_number_of_free_blocks();

    bool start_file(char name, int starting_block, int size);
    bool add_file(char name, int size, int process_id);
};