#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

std::vector<std::vector<int>> read_rows_from_file(const char* file)
{
    std::vector<std::vector<int>> rows;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        int row_number = 0;

        while (std::getline(file_stream, line))
        {
            rows.emplace_back();

            std::istringstream string_stream(line);

            int number;

            while (string_stream >> number)
            {
                rows[row_number].push_back(number);
            }

            row_number++;
        }
    }

    return rows;
}

int get_row_range(const std::vector<int>& row)
{
    int min_number = std::numeric_limits<int>::max();
    int max_number = std::numeric_limits<int>::min();

    for (int number : row)
    {
        if (number < min_number)
            min_number = number;

        if (number > max_number)
            max_number = number;
    }

    return max_number - min_number;
}

int get_row_division(const std::vector<int>& row)
{
    for (int i = 0; i < row.size() - 1; i++)
    {
        for (int j = i + 1; j < row.size(); j++)
        {
            if (row[i] % row[j] == 0)
                return row[i] / row[j];
            else if (row[j] % row[i] == 0)
                return row[j] / row[i];
        }
    }

    return 0;
}

int get_sum_of_ranges(const std::vector<std::vector<int>>& rows)
{
    int sum = 0;

    for (const std::vector<int>& row : rows)
    {
        sum += get_row_range(row);
    }

    return sum;
}

int get_sum_of_divisions(const std::vector<std::vector<int>>& rows)
{
    int sum = 0;

    for (const std::vector<int>& row : rows)
    {
        sum += get_row_division(row);
    }

    return sum;
}

int main(int argc, char** argv)
{
    std::vector<std::vector<int>> rows = read_rows_from_file("input.txt");

    std::cout << "Sum of ranges: " << get_sum_of_ranges(rows) << '\n';
    std::cout << "Sum of divisions: " << get_sum_of_divisions(rows) << '\n';

    return 0;
}
