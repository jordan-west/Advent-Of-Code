#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> read_lines_from_file(const char* file)
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

int get_code_for_line(const std::string& line, int start_code)
{
    int code = start_code;

    for (char letter : line)
    {
        switch (letter)
        {
            case 'U':
                if (code - 3 > 0)
                {
                    code -= 3;
                }

                break;
            case 'D':
                if (code + 3 < 10)
                {
                    code += 3;
                }

                break;
            case 'R':
                if (code != 3 && code != 6 && code != 9)
                {
                    code += 1;
                }

                break;
            case 'L':
                if (code != 1 && code != 4 && code != 7)
                {
                    code -= 1;
                }

                break;
        }
    }

    return code;
}

char get_code_for_line_part_two(const std::string& line, int start_code)
{
    int code = start_code;

    for (char letter : line)
    {
        switch (letter)
        {
            case 'U':
                if (code != 1 && code != 2 && code != 4 && code != 5 && code != 9)
                {
                    if (code == 13 || code == 3)
                    {
                        code -= 2;
                    } else {
                        code -= 4;
                    }
                }

                break;
            case 'D':
                if (code != 13 && code != 12 && code != 10 && code != 9 && code != 5)
                {
                    if (code == 1 || code == 11)
                    {
                        code += 2;
                    } else {
                        code += 4;
                    }
                }

                break;
            case 'R':
                if (code != 1 && code != 4 && code != 9 && code != 12 && code != 13)
                {
                    code += 1;
                }

                break;
            case 'L':
                if (code != 1 && code != 2 && code != 5 && code != 10 && code != 13)
                {
                    code -= 1;
                }

                break;
        }
    }

    char char_code;

    switch (code)
    {
        case 10:
            char_code = 'A';
            break;
        case 11:
            char_code = 'B';
            break;
        case 12:
            char_code = 'C';
            break;
        case 13:
            char_code = 'D';
            break;
        default:
            char_code = '0' + code;
            break;
    }

    return char_code;
}

std::vector<int> get_code_for_lines(const std::vector<std::string>& lines)
{
    std::vector<int> code;

    int previous_code = 5;

    for (const std::string& line : lines)
    {
        previous_code = get_code_for_line(line, previous_code);

        code.push_back(previous_code);
    }

    return code;
}

std::vector<char> get_code_for_lines_part_two(const std::vector<std::string>& lines)
{
    std::vector<char> code;

    int previous_code = 5;

    for (const std::string& line : lines)
    {
        char previous_char_code = get_code_for_line_part_two(line, previous_code);

        switch (previous_char_code)
        {
            case 'A':
                previous_code = 10;
                break;
            case 'B':
                previous_code = 11;
                break;
            case 'C':
                previous_code = 12;
                break;
            case 'D':
                previous_code = 13;
                break;
            default:
                previous_code = previous_char_code - '0';
                break;
        }

        code.push_back(previous_char_code);
    }

    return code;
}

int main(int argc, char** argv)
{
    std::vector<std::string> lines = read_lines_from_file("input.txt");

    std::vector<int> code = get_code_for_lines(lines);
    std::vector<char> code_part_two = get_code_for_lines_part_two(lines);

    for (char code_digit : code_part_two)
    {
        std::cout << code_digit;
    }

    std::cout << '\n';

    return 0;
}
