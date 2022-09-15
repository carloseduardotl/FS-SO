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

    std::vector<block> blocks = disk.get_blocks();
    for(i=0; i<int(blocks.size()); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl << "Number of free blocks: " << disk.get_number_of_free_blocks() << std::endl;

    while(std::getline(files, line))
    {
        std::stringstream ss(line);
        std::vector<std::string> params;
        while (ss.good())
        {
            std::string substr;
            std::getline(ss, substr, ',');
            params.push_back(substr);
        }

        int process_id = std::stoi(params[0]);
        int operation_type = std::stoi(params[1]);
        char name = params[2][1];
        int size = std::stoi(params[3]);
        if(operation_type == 0)
        {
            if(disk.add_file(name, size, process_id))
            {
                std::cout << "Operação " << 1 << " do Processo " << process_id << " - Criar o arquivo " << name << " => Sucesso" << std::endl;
                // Adicionar informações extras da criação do processo
            }
            else
            {
                std::cout << "Operação " << 1 << " do Processo " << process_id << " - Criar o arquivo " << name << " => Falha" << std::endl;
                std::cout << disk.get_error_msg() << std::endl;
            }
        }
        else
        {
            disk.delete_file(name ,process_id);
        }
    }
    
    /*disk.add_file('A', 10, 1);
    blocks = disk.get_blocks();
    for(i=0; i<blocks.size(); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl;
    disk.delete_file('A', 1);*/
    blocks = disk.get_blocks();
    for(i=0; i<int(blocks.size()); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl << "Number of free blocks: " << disk.get_number_of_free_blocks() << std::endl;
    return 0;
}