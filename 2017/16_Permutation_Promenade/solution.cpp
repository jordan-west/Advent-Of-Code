#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>

enum class InstructionType
{
    Spin,
    Exchange,
    Partner
};

struct Instruction
{
    InstructionType type;

    int operand_a;
    int operand_b;
};

std::vector<Instruction> read_instructions_from_file(const char* file)
{
    std::vector<Instruction> instructions;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string instruction_string;

        while (std::getline(file_stream, instruction_string, ','))
        {
            Instruction instruction;

            std::stringstream instruction_stream(instruction_string);
            std::string buffer;

            char instruction_type = instruction_stream.get();
            
            switch (instruction_type)
            {
                case ('s'):
                    instruction.type = InstructionType::Spin;
                    instruction_stream >> instruction.operand_a;

                    break;
                case ('x'):
                    instruction.type = InstructionType::Exchange;
                    
                    std::getline(instruction_stream, buffer, '/');
                    instruction.operand_a = std::stoi(buffer);
                    std::getline(instruction_stream, buffer, '/');
                    instruction.operand_b = std::stoi(buffer);

                    break;
                case ('p'):
                    instruction.type = InstructionType::Partner;

                    std::getline(instruction_stream, buffer, '/');
                    instruction.operand_a = buffer[0];
                    std::getline(instruction_stream, buffer, '/');
                    instruction.operand_b = buffer[0];

                    break;
            }

            instructions.push_back(instruction);
        }
    }

    file_stream.close();

    return instructions;
}

void swap(std::string& programs, int index_a, int index_b)
{
    char temp = programs[index_a];
    programs[index_a] = programs[index_b];
    programs[index_b] = temp;
}

void reverse(std::string& programs, int start_index, int end_index)
{
    int middle = start_index + ((end_index - start_index) / 2);

    while (start_index < middle + 1)
    {
        swap(programs, start_index, end_index);
        start_index++;
        end_index--;
    }
}

void spin(std::string& programs, int amount)
{
    int new_start_index = (programs.size() - (amount % (programs.size() + 1)));

    reverse(programs, new_start_index, programs.size() - 1);
    reverse(programs, 0, new_start_index - 1);
    reverse(programs, 0, programs.size() - 1);
}

int find(const std::string& programs, char element)
{
    for (int i = 0; i < programs.size(); i++)
    {
        if (programs[i] == element)
            return i;
    }

    return -1;
}

std::string follow_instructions(const std::vector<Instruction>& instructions, std::string programs, int times)
{
    std::unordered_map<std::string, int> program_map;

    bool cycle_found = false;

    for (int i = 0; i < times; i++)
    {
        for (const Instruction& instruction : instructions)
        {
            switch (instruction.type)
            {
                case InstructionType::Spin:
                    spin(programs, instruction.operand_a);
                    break;
                case InstructionType::Exchange:
                    swap(programs, instruction.operand_a, instruction.operand_b);
                    break;
                case InstructionType::Partner:
                    int index_a = find(programs, instruction.operand_a);
                    int index_b = find(programs, instruction.operand_b);
                    swap(programs, index_a, index_b);
                    break;
            }
        }

        if (program_map.find(programs) == program_map.end())
        {
            program_map[programs] = i;
        }
        else
        {
            // We've hit a cycle, so increment by however many times we can cycle before hitting the limit
            i += ((times - i) / i) * i;
        }
    }

    return programs;
}

int main(int argc, char** argv)
{
    std::vector<Instruction> instructions = read_instructions_from_file("input.txt");

    const int number_of_programs = 16;
    const int iterations = 1000000000;

    std::string programs;

    programs.reserve(number_of_programs);

    for (int i = 0; i < number_of_programs; i++)
    {
        programs.push_back('a' + i);
    }

    programs = follow_instructions(instructions, programs, iterations);

    for (char program : programs)
    {
        std::cout << program;
    }

    std::cout << '\n';

    return 0;
}
