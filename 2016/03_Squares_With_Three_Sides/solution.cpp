#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> load_lines_from_file(const char* file)
{
    std::vector<std::string> lines;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line))
        {
            lines.push_back(line);
        }
    }

    return lines;
}

bool is_triangle_valid(int side1, int side2, int side3)
{
    if (side1 + side2 <= side3)
        return false;
    else if (side1 + side3 <= side2)
        return false;
    else if (side2 + side3 <= side1)
        return false;
    else
        return true;
}

int sum_valid_triangles(const std::vector<std::string>& lines)
{
    int sum = 0;

    for (const std::string& line : lines)
    {
        std::vector<std::string> triangle_sides;
        std::stringstream string_stream(line);
        std::string buffer;

        while (string_stream >> buffer)
            triangle_sides.push_back(buffer);

        if (is_triangle_valid(std::stoi(triangle_sides[0]), std::stoi(triangle_sides[1]), std::stoi(triangle_sides[2])))
            sum += 1;
    }

    return sum;
}

int main(int argc, char** argv)
{
    std::vector<std::string> lines = load_lines_from_file("input.txt");

    int sum = sum_valid_triangles(lines);

    std :: cout << "Sum: " << sum << '\n';

    return 0;
}
