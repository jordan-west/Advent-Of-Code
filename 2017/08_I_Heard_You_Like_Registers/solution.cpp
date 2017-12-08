#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <unordered_map>
#include <utility>

enum class Operator
{
    NONE,
    GREATER_THAN,
    LESS_THAN,
    GREATER_THAN_EQ,
    LESS_THAN_EQ,
    EQUAL,
    NOT_EQUAL
};

enum struct Operation
{
    NONE,
    INCREMENT,
    DECREMENT
};

struct Condition
{
    std::string register_name;
    Operator condition_operator;
    int value;
};

struct Instruction
{
    std::string modify_register;
    Operation operation;
    int value;
    Condition condition;
};

Operation parse_operation(const std::string& operation_string)
{
    if (operation_string == "inc")
        return Operation::INCREMENT;
    if (operation_string == "dec")
        return Operation::DECREMENT;

    return Operation::NONE;
}

Operator parse_operator(const std::string& operator_string)
{
    if (operator_string == ">")
        return Operator::GREATER_THAN;
    if (operator_string == "<")
        return Operator::LESS_THAN;
    if (operator_string == ">=")
        return Operator::GREATER_THAN_EQ;
    if (operator_string == "<=")
        return Operator::LESS_THAN_EQ;
    if (operator_string == "==")
        return Operator::EQUAL;
    if (operator_string == "!=")
        return Operator::NOT_EQUAL;

    return Operator::NONE;
}

std::vector<Instruction> read_instructions_from_file(const char* file)
{
    std::vector<Instruction> instructions;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line))
        {
            std::stringstream line_stream(line);

            Instruction instruction;
            std::string temp_buffer;

            line_stream >> instruction.modify_register;
            line_stream >> temp_buffer;
            instruction.operation = parse_operation(temp_buffer);
            line_stream >> temp_buffer;
            instruction.value = std::stoi(temp_buffer);
            line_stream >> temp_buffer;
            line_stream >> instruction.condition.register_name;
            line_stream >> temp_buffer;
            instruction.condition.condition_operator = parse_operator(temp_buffer);
            line_stream >> temp_buffer;
            instruction.condition.value = std::stoi(temp_buffer);

            instructions.push_back(instruction);
        }
    }

    file_stream.close();

    return instructions;
}

bool interpret_condition(const Condition& condition, int register_value)
{
    if (condition.condition_operator == Operator::GREATER_THAN)
        return register_value > condition.value;
    if (condition.condition_operator == Operator::LESS_THAN)
        return register_value < condition.value;
    if (condition.condition_operator == Operator::GREATER_THAN_EQ)
        return register_value >= condition.value;
    if (condition.condition_operator == Operator::LESS_THAN_EQ)
        return register_value <= condition.value;
    if (condition.condition_operator == Operator::EQUAL)
        return register_value == condition.value;
    if (condition.condition_operator == Operator::NOT_EQUAL)
        return register_value != condition.value;

    return false;
}

void interpret_operation(const Operation& operation, int& modify_register, int value)
{
    if (operation == Operation::INCREMENT)
        modify_register += value;
    else if (operation == Operation::DECREMENT)
        modify_register -= value;
}

int run_instructions(const std::vector<Instruction>& instructions, std::unordered_map<std::string, int>& registers)
{
    int max_value = std::numeric_limits<int>::min();

    for (const Instruction& instruction : instructions)
    {
        if (registers.find(instruction.modify_register) == registers.end())
            registers[instruction.modify_register] = 0;
        if (registers.find(instruction.condition.register_name) == registers.end())
            registers[instruction.condition.register_name] = 0;

        if (interpret_condition(instruction.condition, registers[instruction.condition.register_name]))
        {
                interpret_operation(instruction.operation, registers[instruction.modify_register], instruction.value);

                if (registers[instruction.modify_register] > max_value)
                    max_value = registers[instruction.modify_register];
        }
    }

    return max_value;
}

std::pair<int, int> calculate_largest_register(const std::vector<Instruction>& instructions)
{
    std::unordered_map<std::string, int> registers;

    int max_value_during = run_instructions(instructions, registers);

    int max_register_value = std::numeric_limits<int>::min();

    for (auto it = registers.begin(); it != registers.end(); it++)
    {
        if (it->second > max_register_value)
            max_register_value = it->second;
    }

    return std::make_pair(max_register_value, max_value_during);
}

int main(int argc, char** argv)
{
    std::vector<Instruction> instructions = read_instructions_from_file("input.txt");

    std::pair<int, int> largest_values = calculate_largest_register(instructions);

    std::cout << "Largest final register value: " << largest_values.first << '\n';
    std::cout << "Largest register value throughout: " << largest_values.second << '\n';

    return 0;
}
