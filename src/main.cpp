#include <Disk.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
    Disk disk;

    std::ifstream processes("inputs/processes.txt");
    std::ifstream files("inputs/files.txt");

    if(!processes.is_open() || !files.is_open())
    {
        std::cout << "Error opening files" << std::endl;
        return 1;
    }

    std::string line;

    while(std::getline(processes, line))
    {   
        std::stringstream ss(line);
        int id, priority, process_time;
        while(ss.good())
        {
            std::string substr;
            std::getline(ss, substr, ',');
            id = std::stoi(substr);
            std::getline(ss, substr, ',');
            priority = std::stoi(substr);
            std::getline(ss, substr, ',');
            process_time = std::stoi(substr);
        }
        disk.add_process(id, priority, process_time);
    }

    /*std::vector<process> processes_vector = disk.get_processes();
    while(processes_vector.size() > 0)
    {
        std::cout << "Process ID: " << processes_vector[0].id << std::endl;
        std::cout << "Process Priority: " << processes_vector[0].priority << std::endl;
        std::cout << "Process Time: " << processes_vector[0].process_time << std::endl;
        processes_vector.erase(processes_vector.begin());
    }*/

    std::getline(files, line);
    disk.set_current_state(std::stoi(line));

    std::getline(files, line);
    disk.set_number_of_blocks(std::stoi(line));

    std::getline(files, line);
    int number_of_files = std::stoi(line);
    int i = 0;
    for(i=0; i<number_of_files; i++)
    {
        std::getline(files, line);   
        std::vector<std::string> params;
        std::stringstream ss(line);
        while (ss.good()) 
        {
            std::string substr;
            std::getline(ss, substr, ',');
            params.push_back(substr);
        }
        disk.start_file(params[0][0], std::stoi(params[1]), std::stoi(params[2]));
    }

    std::cout << disk.get_current_state() << std::endl;
    std::cout << disk.get_number_of_blocks() << std::endl;

    std::vector<block> blocks = disk.get_blocks();
    for(i=0; i<blocks.size(); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl << disk.get_number_of_free_blocks() << std::endl;
    disk.add_file('A', 3, 1);
    blocks = disk.get_blocks();
    for(i=0; i<blocks.size(); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl;
    disk.delete_file('A', 3);
    blocks = disk.get_blocks();
    for(i=0; i<blocks.size(); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    return 0;
}