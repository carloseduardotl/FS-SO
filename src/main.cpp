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
    std::getline(files, line);
    disk.set_current_state(std::stoi(line));
    if(files.eof())
    {
        std::cout << "Error reading files" << std::endl;
        return 1;
    }

    std::getline(files, line);
    disk.set_number_of_blocks(std::stoi(line));
    if(files.eof())
    {
        std::cout << "Error reading files" << std::endl;
        return 1;
    }

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
    if(files.eof())
    {
        std::cout << "Error reading files" << std::endl;
        return 1;
    }

    std::cout << disk.get_current_state() << std::endl;
    std::cout << disk.get_number_of_blocks() << std::endl;

    std::vector<block> blocks = disk.get_blocks();
    for(i=0; i<blocks.size(); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl << disk.get_number_of_free_blocks() << std::endl;
    return 0;
}