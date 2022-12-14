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
        std::cout << "Erro ao abrir os arquivos" << std::endl;
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
    std::getline(files, line);
    int current_state = std::stoi(line);

    std::getline(files, line);
    disk.set_number_of_blocks(std::stoi(line));

    std::getline(files, line);
    int number_of_files = std::stoi(line);
    int i = 0;
    char name;
    int starting_block, size;
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
        name = params[0][0];
        starting_block = std::stoi(params[1]);
        size = std::stoi(params[2]);
        disk.start_file(name, starting_block, size);
    }

    std::vector<block> blocks = disk.get_blocks();
    for(i=0; i<int(blocks.size()); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl << "Quantidade de Blocos Livres: " << disk.get_number_of_free_blocks() << std::endl << std::endl;
    std::cout << "Sistema de arquivos" << std::endl << std::endl;
    disk.set_current_state(current_state);

    int counter = 1;
    int process_id, operation_type, process_time_counter;
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
        params.push_back("0"); // Add a 0 to the end of the vector to avoid segmentation fault

        std::cout << counter << "?? Opera????o - ";
        counter++;

        process_id = std::stoi(params[0]); // process id
        operation_type = std::stoi(params[1]); // operation type
        name = params[2][1]; // file name
        size = std::stoi(params[3]); // file size
        process_time_counter = disk.get_process(process_id).time_counter;
        disk.set_process_time_count(process_id, process_time_counter + 1);
        if(process_time_counter == disk.get_process(process_id).process_time)
        {
            disk.set_error_msg("O processo " + std::to_string(process_id) + " j?? encerrou sua execu????o.\n");
        }
        if(operation_type == 0)
        {
            std::vector<int> blocks; // vector of blocks that the file occupies
            if((process_time_counter < disk.get_process(process_id).process_time) && disk.add_file(name, size, process_id, &blocks))
            {
                std::cout << "Opera????o " << process_time_counter+1 << " do Processo " << process_id << " - Criar o arquivo " << name << " => Sucesso" << std::endl;
                std::cout << "Bloco(s): ";
                for(int i = 0; i<int(blocks.size()); i++)
                {
                    if(i==int(blocks.size())-1) 
                    {
                        std::cout << blocks[i] << std::endl << std::endl; //print the last block without the comma
                    }
                    else
                    {
                        std::cout << blocks[i] << ", ";
                    }
                };
            }
            else
            {
                std::cout << "Opera????o " << process_time_counter+1 << " do Processo " << process_id << " - Criar o arquivo " << name << " => Falha" << std::endl;
                std::cout << disk.get_error_msg() << std::endl;
            }
        }
        else
        {
            if((process_time_counter < disk.get_process(process_id).process_time) && disk.delete_file(name ,process_id))
            {
                std::cout << "Opera????o " << process_time_counter+1 << " do Processo " << process_id << " - Deletar o arquivo " << name << " => Sucesso" << std::endl;
                std::cout << "O processo " << process_id << " deletou o arquivo " << name << std::endl << std::endl;
            }
            else
            {
                std::cout << "Opera????o " << process_time_counter+1 << " do Processo " << process_id << " - Deletar o arquivo " << name << " => Falha" << std::endl;
                std::cout << disk.get_error_msg() << std::endl;
            }
        }
        disk.set_error_msg("");
    }
    blocks = disk.get_blocks(); // update block vector
    for(i=0; i<int(blocks.size()); i++)
    {
        std::cout << blocks[i].name << " ";
    }
    std::cout << std::endl << "Quantidade de Blocos Livres: " << disk.get_number_of_free_blocks() << std::endl;
    return 0;
}