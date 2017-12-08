#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <limits>
#include <utility>

std::vector<int> load_ints_from_file(const char* file)
{
    std::vector<int> numbers;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        int number;

        while (file_stream >> number)
        {
            numbers.push_back(number);
        }
    }

    file_stream.close();

    return numbers;
}

std::string vector_to_string(const std::vector<int>& data)
{
    std::string converted = "";
    converted.reserve(data.size());

    for (int number : data)
    {
        converted += std::to_string(number);
    }

    return converted;
}

int find_max_index(const std::vector<int>& data)
{
    int max_blocks = std::numeric_limits<int>::min();
    int max_index = -1;

    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] > max_blocks)
        {
            max_blocks = data[i];
            max_index = i;
        }
    }

    return max_index;
}

std::pair<int, int> reallocate_cycles(std::vector<int> blocks)
{
    std::unordered_set<std::string> previous_blocks;
    std::vector<std::pair<std::string, int>> indices;

    int cycle_count = 0;

    while (previous_blocks.find(vector_to_string(blocks)) == previous_blocks.end())
    {
        std::string block_string = vector_to_string(blocks);
        previous_blocks.insert(block_string);
        indices.push_back(std::make_pair(block_string, cycle_count));

        int current_index = find_max_index(blocks);
        int blocks_to_distribute = blocks[current_index];
        blocks[current_index] = 0;

        while (blocks_to_distribute > 0)
        {
            current_index = (current_index + 1) % blocks.size();

            blocks[current_index]++;

            blocks_to_distribute--;
        }

        cycle_count++;
    }

    int loop_start = 0;

    for (auto x : indices)
    {
        if (x.first == vector_to_string(blocks))
        {
            loop_start = x.second;
        }
    }
    
    int loop_count = cycle_count - loop_start;

    return std::make_pair(loop_count, cycle_count);
}

int main(int argc, char** argv)
{
    std::vector<int> numbers = load_ints_from_file("input.txt");

    auto cycles = reallocate_cycles(numbers);

    std::cout << "Loop: " << cycles.first << '\n';
    std::cout << "Cycles: " << cycles.second << '\n';

    return 0;
}
