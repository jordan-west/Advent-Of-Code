#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstdint>

struct Point
{
    int x;
    int y;

    bool operator==(const Point& other) const
    {
        return (x == other.x && y == other.y);
    }
};

struct PointHash
{
    std::size_t operator()(const Point& point) const noexcept
    {
        return std::hash<int>{}(point.x) ^ (std::hash<int>{}(point.y) << 1);
    }
};

int read_int_from_file(const char* file)
{
    int number;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        file_stream >> number;
    }

    file_stream.close();

    return number;
}

int find_nearest_odd_square(int number)
{
    for (int i = 1; i <= number + 2; i += 2)
    {
        if (number <= i * i)
            return i;
    }

    return 0;
}

int get_spiral_distance(int point)
{
    int side_length = find_nearest_odd_square(point);
    int nearest_square = side_length * side_length;
    int current_corner = nearest_square;

    while (current_corner > point)
    {
        if (current_corner - (side_length - 1) > point)
            current_corner -= (side_length - 1);
        else
        {
            int next_corner = current_corner - (side_length - 1);

            if (current_corner - point < point - next_corner)
            {
                return side_length - 1 - (current_corner - point);
            }
            else
            {
                return side_length - 1 - (point - next_corner);
            }
        }
    }

    return side_length - 1;
}

uint64_t get_adjacent_sum(const std::unordered_map<Point, uint64_t, PointHash>& points, const Point& point)
{
    uint64_t sum = 0;

    for (int x = point.x - 1; x <= point.x + 1; x++)
    {
        for (int y = point.y - 1; y <= point.y + 1; y++)
        {
            Point adjacent_point = { x, y };

            if (points.find(adjacent_point) != points.end())
            {
                sum += points.at(adjacent_point);
            }
        }
    }

    return sum;
}

uint64_t get_spiral_sum(uint64_t point)
{
    if (point == 1) return 1;

    std::unordered_map<Point, uint64_t, PointHash> points;
    Point current_point = { 0, 0 };
    int direction = 0;
    int step_amount = 1;
    bool change_step_amount = true;

    points[{ 0, 0 }] = 1;

    while (true)
    {
        for (uint64_t i = 0; i < step_amount; i++)
        {
            switch (direction)
            {
                case 0:
                    current_point.x += 1;
                    break;
                case 1:
                    current_point.y += 1;
                    break;
                case 2:
                    current_point.x -= 1;
                    break;
                case 3:
                    current_point.y -= 1;
                    break;
            }

            int sum = get_adjacent_sum(points, current_point);

            if (sum > point) return sum;

            points[current_point] = sum;
        }

        direction = (direction + 1) % 4;
        change_step_amount = !change_step_amount;
        if (change_step_amount) step_amount += 1;
    }
}

int main(int argc, char** argv)
{
    uint64_t point = read_int_from_file("input.txt");

    int spiral_distance = get_spiral_distance(point);
    uint64_t spiral_sum = get_spiral_sum(point);

    std::cout << "Point: " << point << '\n';

    std::cout << "Spiral distance: " << spiral_distance << '\n';
    std::cout << "Spiral sum: " << spiral_sum << '\n';

    return 0;
}
