#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stack>

std::string read_string_from_file(const char* file)
{
    std::string file_string;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::getline(file_stream, file_string);
    }

    file_stream.close();

    return file_string;
}

int calculate_groups(const std::string& garbage)
{
    int groups = 0;
    int depth = 1;

    std::stack<int> memory;

    bool started_group = false;
    bool started_garbage = false;
    bool ignore_character = false;

    for (const char& character : garbage)
    {
        if (ignore_character)
        {
            ignore_character = false;

            continue;
        }

        if (started_garbage)
        {
            if (character == '>')
                started_garbage = false;
            else if (character == '!')
                ignore_character = true;

            continue;
        }

        switch (character)
        {
            case '!':
                ignore_character = true;
                break;
            case '<':
                started_garbage = true;
                break;
            case '{':
                memory.push(depth);
                depth += 1;
                break;
            case '}':
                if (memory.empty() == false)
                {
                    int temp_depth = memory.top();

                    memory.pop();

                    groups += temp_depth;
                    depth -= 1;
                }

                break;
        }
    }

    return groups;
}

int calculate_garbage_count(const std::string& garbage)
{
    int garbage_count = 0;

    bool started_garbage = false;
    bool ignore_character = false;

    for (const char& character : garbage)
    {
        if (ignore_character)
        {
            ignore_character = false;

            continue;
        }

        if (started_garbage)
        {
            if (character == '>')
            {
                started_garbage = false;

                continue;
            }
            else if (character == '!')
            {
                ignore_character = true;

                continue;
            }

            garbage_count += 1;
        }

        if (character == '<')
        {
            started_garbage = true;
        }
        else if (character == '!')
        {
            ignore_character = true;
        }
    }

    return garbage_count;
}

int main(int argc, char** argv)
{
    std::string garbage = read_string_from_file("input.txt");

    int groups = calculate_groups(garbage);
    int garbage_count = calculate_garbage_count(garbage);

    std::cout << "Groups: " << groups << '\n';
    std::cout << "Garbage: " << garbage_count << '\n';

    return 0;
}
