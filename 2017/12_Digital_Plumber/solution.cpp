#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Program
{
    int id;

    std::vector<int> connecting_ids;
};

std::unordered_map<int, Program> read_programs_from_file(const char* file)
{
    std::unordered_map<int, Program> programs;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line))
        {
            Program program;

            std::stringstream string_stream(line);

            std::string buffer;

            string_stream >> program.id;
            string_stream >> buffer;

            while (std::getline(string_stream, buffer, ','))
            {
                program.connecting_ids.push_back(std::stoi(buffer));
            }

            programs[program.id] = program;
        }
    }

    file_stream.close();

    return programs;
}

int count_programs_in_group(int program_id, const std::unordered_map<int, Program>& programs, std::unordered_set<int>& seen_ids)
{
    int groups = 1;

    if (programs.find(program_id) == programs.end())
        return 0;

    Program program = programs.at(program_id);

    for (int id : program.connecting_ids)
    {
        if (seen_ids.find(id) == seen_ids.end())
        {
            seen_ids.insert(id);
            groups += count_programs_in_group(id, programs, seen_ids);
        }
    }

    return groups;
}

void mark_group(int program_id, const std::unordered_map<int, Program>& programs, std::unordered_set<int>& seen_ids, std::unordered_set<int>& ids_to_check)
{
    if (programs.find(program_id) == programs.end())
        return;

    Program program = programs.at(program_id);

    for (int id : program.connecting_ids)
    {
        if (seen_ids.find(id) == seen_ids.end())
        {
            if (ids_to_check.find(id) != ids_to_check.end())
            {
                ids_to_check.erase(ids_to_check.find(id));
            }

            seen_ids.insert(id);
            mark_group(id, programs, seen_ids, ids_to_check);
        }
    }
}

int count_groups(const std::unordered_map<int, Program>& programs, std::unordered_set<int> ids_to_check)
{
    int groups = 0;

    while (ids_to_check.size() > 0)
    {
        std::unordered_set<int> seen_ids;
        
        int program_id = *(ids_to_check.begin());

        seen_ids.insert(program_id);
        ids_to_check.erase(ids_to_check.begin());

        mark_group(program_id, programs, seen_ids, ids_to_check);

        groups += 1;
    }

    return groups;
}

int main(int argc, char** argv)
{
    std::unordered_map<int, Program> programs = read_programs_from_file("input.txt");
    std::unordered_set<int> seen_ids;
    std::unordered_set<int> ids_to_check;

    int program_id = 0;

    seen_ids.insert(program_id);

    for (auto it = programs.begin(); it != programs.end(); it++)
    {
        ids_to_check.insert(it->first);
    }

    int program_count = count_programs_in_group(program_id, programs, seen_ids);
    int groups = count_groups(programs, ids_to_check);

    std::cout << "Groups for Program " << program_id << ": " << program_count << '\n';
    std::cout << "Number of groups: " << groups << '\n';

    return 0;
}
