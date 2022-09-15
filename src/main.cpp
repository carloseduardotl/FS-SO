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
             
    }

    std::cout << disk.get_current_state() << std::endl;
    std::cout << disk.get_number_of_blocks() << std::endl;
    return 0;
}