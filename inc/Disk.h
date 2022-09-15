#pragma once
#include <vector>
#include <math.h>

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
    char indexed_file;
} block;

typedef struct process
{
    int id;
    int priority;
    int process_time;
} process;

class Disk 
{
    private:
    allocation current_allocation;
    std::vector<block> blocks;
    std::vector<process> processes;

    public:

    Disk();
    ~Disk();

    void add_process(int id, int priority, int process_time);
    std::vector<process> get_processes();
    bool process_exists(int id);

    void set_current_state(int state);
    allocation get_current_state();

    void set_number_of_blocks(int number_of_blocks);
    int get_number_of_blocks();
    std::vector<block> get_blocks();
    int get_number_of_free_blocks();
    int get_contiguous_free_blocks(int position);

    bool start_file(char name, int starting_block, int size);
    bool add_file(char name, int size, int process_id);
    bool delete_file(char name, int origin_process_id);
};