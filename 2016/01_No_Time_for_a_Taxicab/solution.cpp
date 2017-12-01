#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

struct Instruction
{
    char turn_direction;
    int blocks_to_move;
};

struct Position
{
    int x;
    int y;
};

/*
 * Loads instructions from a file with the following format:
 * D1N1, D2N2, ... - where D is a direction letter and N is an integer
 */
std::vector<Instruction> load_instructions_from_file(const char* file)
{
    std::vector<Instruction> instructions;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line, ' '))
        {
            Instruction instruction;

            instruction.turn_direction = line[0];
            instruction.blocks_to_move = std::stoi(line.substr(1, line.size() - 1));

            instructions.push_back(instruction);
        }
    }

    std::cout << '\n';

    return instructions;
}

/*
 * Returns the new direction faced after turning left or right.
 */
int get_direction_facing(int direction_facing, char turn_direction)
{
    switch (turn_direction)
    {
        case 'L':
            direction_facing -= 1;

            if (direction_facing < 0)
                direction_facing = 3;

            return direction_facing;
        case 'R':
            direction_facing = (direction_facing + 1) % 4;

            return direction_facing;
    }

    return direction_facing;
}

/*
 * Returns the new position after moving in the direction facing a number of blocks
 */
Position get_new_position(Position current_position, int direction_facing, int blocks_to_move)
{
    switch (direction_facing)
    {
        case 0:
            current_position.y += blocks_to_move;

            return current_position;
        case 1:
            current_position.x += blocks_to_move;

            return current_position;
        case 2:
            current_position.y -= blocks_to_move;

            return current_position;
        case 3:
            current_position.x -= blocks_to_move;

            return current_position;
    }

    return current_position;
}

/*
 * Returns the final position reached after following the set of instructions
 * given. The turn direction char, either 'L' or 'R', turns left or right
 * 90 degrees. The blocks to move integer moves in the direction facing a
 * number of blocks.
 */
Position follow_instructions(const std::vector<Instruction>& instructions)
{
    int direction_facing = 0;

    Position current_position { 0, 0 };

    for (Instruction instruction : instructions)
    {
        direction_facing = get_direction_facing(direction_facing, instruction.turn_direction);

        current_position = get_new_position(current_position, direction_facing, instruction.blocks_to_move);
    }

    return current_position;
}

/*
 * Returns the first location that is visited twice by following
 * the set of instructions given. Returns { 0, 0 } otherwise.
 */
Position follow_instructions_part_two(const std::vector<Instruction>& instructions)
{
    int direction_facing = 0;

    Position current_position { 0, 0 };

    std::vector<Position> move_history;

    move_history.push_back(current_position);

    for (Instruction instruction : instructions)
    {
        direction_facing = get_direction_facing(direction_facing, instruction.turn_direction);

        for (int blocks_to_move = instruction.blocks_to_move; blocks_to_move > 0; blocks_to_move--)
        {
            current_position = get_new_position(current_position, direction_facing, 1);

            for (Position position : move_history)
            {
                if (current_position.x == position.x && current_position.y == position.y)
                    return current_position;
            }

            move_history.push_back(current_position);
        }
    }

    return { 0, 0 };
}

int main(int argc, char** argv)
{
    std::vector<Instruction> instructions = load_instructions_from_file("input.txt");

    Position end_position = follow_instructions_part_two(instructions);

    std::cout << "End position: " << end_position.x << ", " << end_position.y << '\n';

    std::cout << std::abs(end_position.x) + std::abs(end_position.y) << " blocks away\n";

    return 0;
}
