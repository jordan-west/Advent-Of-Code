#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>

enum struct Direction
{
    NORTH,
    NORTH_EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    NORTH_WEST
};

struct Point
{
    float x = 0;
    float y = 0;
};

std::vector<Direction> read_directions_from_file(const char* file)
{
    std::vector<Direction> directions;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line, ','))
        {
            std::stringstream string_stream(line);

            std::string direction;

            string_stream >> direction;

            if (direction == "n")
                directions.push_back(Direction::NORTH);
            else if (direction == "ne")
                directions.push_back(Direction::NORTH_EAST);
            else if (direction == "se")
                directions.push_back(Direction::SOUTH_EAST);
            else if (direction == "s")
                directions.push_back(Direction::SOUTH);
            else if (direction == "sw")
                directions.push_back(Direction::SOUTH_WEST);
            else if (direction == "nw")
                directions.push_back(Direction::NORTH_WEST);
        }
    }

    file_stream.close();

    return directions;
}

Point follow_directions(Point start_point, const std::vector<Direction>& directions)
{
    Point end_point = start_point;

    for (const Direction& direction : directions)
    {
        if (direction == Direction::NORTH)
        {
            end_point.y += 1;
        }
        else if (direction == Direction::NORTH_EAST)
        {
            end_point.x += 1;
            end_point.y += 0.5;
        }
        else if (direction == Direction::SOUTH_EAST)
        {
            end_point.x += 1;
            end_point.y -= 0.5;
        }
        else if (direction == Direction::SOUTH)
        {
            end_point.y -= 1;
        }
        else if (direction == Direction::SOUTH_WEST)
        {
            end_point.x -= 1;
            end_point.y -= 0.5;
        }
        else if (direction == Direction::NORTH_WEST)
        {
            end_point.x -= 1;
            end_point.y += 0.5;
        }
    }

    return end_point;
}

int calculate_distance(Point end_point)
{
    float distance = 0;

    distance += std::abs(end_point.x);

    if (std::abs(end_point.y) > std::abs(end_point.x / 2))
        distance += std::abs(end_point.y) - std::abs(end_point.x / 2);

    return distance;
}

int calculate_distance(Point start_point, Point end_point)
{
    // Offset our end_point so we're calculating distance to origin
    end_point.x = std::abs(start_point.x - end_point.x);
    end_point.y = std::abs(start_point.y - end_point.y);

    return calculate_distance(end_point);
}

int calculate_max_distance(const std::vector<Direction>& directions)
{
    Point point;
    point.x = 0;
    point.y = 0;

    Point max_point;
    point.x = 0;
    point.y = 0;

    for (const Direction& direction : directions)
    {
        if (direction == Direction::NORTH)
        {
            point.y += 1;
        }
        else if (direction == Direction::NORTH_EAST)
        {
            point.x += 1;
            point.y += 0.5;
        }
        else if (direction == Direction::SOUTH_EAST)
        {
            point.x += 1;
            point.y -= 0.5;
        }
        else if (direction == Direction::SOUTH)
        {
            point.y -= 1;
        }
        else if (direction == Direction::SOUTH_WEST)
        {
            point.x -= 1;
            point.y -= 0.5;
        }
        else if (direction == Direction::NORTH_WEST)
        {
            point.x -= 1;
            point.y += 0.5;
        }

        if (calculate_distance(point) > calculate_distance(max_point))
        {
            max_point = point;
        }
    }

    return calculate_distance(max_point);
}

int main(int argc, char** argv)
{
    std::vector<Direction> directions = read_directions_from_file("input.txt");

    Point start_point;
    start_point.x = 0;
    start_point.y = 0;

    Point end_point = follow_directions(start_point, directions);

    float distance = calculate_distance(start_point, end_point);
    float max_distance = calculate_max_distance(directions);

    std::cout << "End point: (" << end_point.x << ", " << end_point.y << ")\n";
    std::cout << "Distance from start: " << distance << '\n';
    std::cout << "Max distance from start: " << max_distance << '\n';

    return 0;
}
