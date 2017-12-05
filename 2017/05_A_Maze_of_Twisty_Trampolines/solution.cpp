#include <iostream>
#include <vector>
#include <fstream>

std::vector<int> read_ints_from_file(const char* file)
{
    std::vector<int> int_list;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        int number;

        while (file_stream >> number)
        {
            int_list.push_back(number);
        }
    }

    file_stream.close();

    return int_list;
}

int calculate_steps_from_exit(std::vector<int> int_list, int offset_threshold, bool use_threshold)
{
    int jumps_till_exit = 0;
    int current_index = 0;

    while (current_index < int_list.size() && current_index >= 0)
    {
        int temp_index = current_index;

        current_index = current_index + int_list[current_index];

        // Ideally move this condition check outside of the loop, but optimization isn't necessary for this problem.
        if (use_threshold)
        {
            if (int_list[temp_index] >= offset_threshold)
                int_list[temp_index]--;
            else
                int_list[temp_index]++;
        }

        jumps_till_exit++;
    }

    return jumps_till_exit;
}

int main(int argc, char** argv)
{
    std::vector<int> int_list = read_ints_from_file("input.txt");

    int steps_taken = calculate_steps_from_exit(int_list, 3, true);

    std::cout << "Steps taken: " << steps_taken << '\n';

    return 0;
}
