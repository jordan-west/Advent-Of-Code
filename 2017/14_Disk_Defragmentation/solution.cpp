#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <unordered_set>

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

std::vector<int> read_ascii_from_file(const char* file)
{
    std::vector<int> ascii;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        char character;

        while (file_stream >> character)
        {
            int ascii_code = character;

            ascii.push_back(ascii_code);
        }
    }

    file_stream.close();

    return ascii;
}

/*
 * Reverses a segment in a cyclic list inplace
 */
void reverse_segment_inplace(std::vector<int>& data, int start_index, int end_index, int segment_length)
{
    float swaps = 0;

    while (swaps < segment_length / 2)
    {
        int temp = data[end_index];

        data[end_index] = data[start_index];
        data[start_index] = temp;

        start_index = (start_index + 1) % data.size();
        end_index -= 1;
        if (end_index < 0)
            end_index = data.size() - 1;

        swaps += 1;
    }
}

void knot_hash(std::vector<int>& data, const std::vector<int>& lengths, int rounds)
{
    int position = 0;
    int skip_size = 0;

    while (rounds > 0)
    {
        for (int length : lengths)
        {
            if (length > data.size())
                break;

            reverse_segment_inplace(data, position, (position + (length - 1)) % data.size(), length);
            position = (position + length + skip_size) % data.size();
            skip_size += 1;
        }

        rounds--;
    }
}

void append_length_suffixes(std::vector<int>& lengths)
{
    lengths.push_back(17);
    lengths.push_back(31);
    lengths.push_back(73);
    lengths.push_back(47);
    lengths.push_back(23);
}

std::vector<int> create_dense_hash(const std::vector<int>& sparse_hash)
{
    std::vector<int> dense_hash;

    for (int i = 0; i < sparse_hash.size() / 16; i++)
    {
        int xor_value = 0;

        for (int j = i * 16; j < (i * 16) + 16; j++)
        {
            xor_value ^= sparse_hash[j]; 
        }

        dense_hash.push_back(xor_value);
    }

    return dense_hash;
}

/*
 * Assumes int value in the range 0-15
 */
char to_hexadecimal_char(int value)
{
    if (value > 15)
        return 'f';

    char hex_char = '0' + value;

    switch (value)
    {
        case 10:
            hex_char = 'a';
            break;
        case 11:
            hex_char = 'b';
            break;
        case 12:
            hex_char = 'c';
            break;
        case 13:
            hex_char = 'd';
            break;
        case 14:
            hex_char = 'e';
            break;
        case 15:
            hex_char = 'f';
            break;
    }

    return hex_char;
}

/*
 * Assumes integers are in the range 0-256
 */
std::string to_hexadecimal(const std::vector<int>& data)
{
    std::string hexadecimal;

    for (int number : data)
    {
        char first_digit = to_hexadecimal_char(number / 16);
        char second_digit = to_hexadecimal_char(number % 16);

        hexadecimal = hexadecimal + first_digit + second_digit;
    }

    return hexadecimal;
}

std::string get_hash_string(std::vector<int> key_string)
{
    std::vector<int> data_for_ascii;

    int data_size = 256;

    data_for_ascii.reserve(data_size);

    for (int i = 0; i < data_size; i++)
    {
        data_for_ascii.push_back(i);
    }

    append_length_suffixes(key_string);
    knot_hash(data_for_ascii, key_string, 64);
    std::vector<int> dense_hash = create_dense_hash(data_for_ascii);
    std::string hexadecimal = to_hexadecimal(dense_hash);

    return hexadecimal;
}

int convert_hex_to_decimal(char hex_digit)
{
    int value = 0;

    switch (hex_digit)
    {
        case 'a':
            return 10;
        case 'b':
            return 11;
        case 'c':
            return 12;
        case 'd':
            return 13;
        case 'e':
            return 14;
        case 'f':
            return 15;
        default:
            return hex_digit - '0';
    }

    return value;
}

std::string convert_hex_to_binary(char hex_digit)
{
    int decimal = convert_hex_to_decimal(hex_digit);
    std::string binary_string = "";
    binary_string.reserve(4);

    for (int i = 3; i >= 0; i--)
    {
        int power = std::pow(2, i);

        if (decimal >= power)
        {
            binary_string += '1';
            decimal -= power;
        }
        else
        {
            binary_string += '0';
        }
    }

    return binary_string;
}

int calculate_fragments_for_row(const std::string& hash_string)
{
    int fragment_squares = 0;

    for (char hex_digit : hash_string)
    {
        std::string binary_string = convert_hex_to_binary(hex_digit);

        for (char binary_digit : binary_string)
        {
            fragment_squares += (binary_digit == '1' ? 1 : 0);
        }
    }

    return fragment_squares;
}

int calculate_fragment_squares(std::vector<int>& key_string, int grid_size)
{
    int fragment_squares = 0;

    for (int i = 0; i < grid_size; i++)
    {
        std::string number = std::to_string(i);

        for (char digit : number)
        {
            key_string.push_back(digit);
        }

        std::string hash_string = get_hash_string(key_string);

        fragment_squares += calculate_fragments_for_row(hash_string);

        for (int i = 0; i < number.size() ; i++)
        {
            key_string.pop_back();
        }
    }

    return fragment_squares;
}

std::vector<std::string> calculate_hashes(std::vector<int>& key_string, int grid_size)
{
    std::vector<std::string> hashes;
    hashes.reserve(grid_size);

    for (int row = 0; row < grid_size; row++)
    {
        std::string number = std::to_string(row);

        for (char digit : number)
        {
            key_string.push_back(digit);
        }

        hashes.push_back(get_hash_string(key_string));

        for (int i = 0; i < number.size() ; i++)
        {
            key_string.pop_back();
        }
    }

    return hashes;
}

void mark_regions(std::unordered_set<Point, PointHash>& marked_regions, const std::vector<std::string>& hashes, int grid_size)
{
    for (int row = 0; row < grid_size; row++)
    {
        int column = 0;

        for (char hex_digit : hashes[row])
        {
            std::string binary_string = convert_hex_to_binary(hex_digit);

            for (char binary_digit : binary_string)
            {
                if (binary_digit == '1')
                {
                    marked_regions.insert(Point{column, row});
                }

                column += 1;
            }
        }
    }
}

void traverse_adjacent_regions(std::unordered_set<Point, PointHash>& marked_regions, const Point& point);

void traverse_region(std::unordered_set<Point, PointHash>& marked_regions, const Point& point)
{
    auto region_iterator = marked_regions.find(point);

    if (region_iterator != marked_regions.end())
    {
        marked_regions.erase(region_iterator);

        traverse_adjacent_regions(marked_regions, point);
    }
}

void traverse_adjacent_regions(std::unordered_set<Point, PointHash>& marked_regions, const Point& point)
{
    traverse_region(marked_regions, Point{point.x, point.y + 1});
    traverse_region(marked_regions, Point{point.x + 1, point.y});
    traverse_region(marked_regions, Point{point.x, point.y - 1});
    traverse_region(marked_regions, Point{point.x - 1, point.y});
}

int traverse_regions(std::unordered_set<Point, PointHash>& marked_regions, int grid_size)
{
    int regions = 0;

    for (int row = 0; row < grid_size; row++)
    {
        for (int column = 0; column < grid_size; column++)
        {
            Point point = {column, row};

            auto region_iterator = marked_regions.find(point);

            if (region_iterator != marked_regions.end())
            {
                marked_regions.erase(region_iterator);

                traverse_adjacent_regions(marked_regions, point);

                regions += 1;
            }
        }
    }

    return regions;
}

int calculate_regions(std::vector<int>& key_string, int grid_size)
{
    std::unordered_set<Point, PointHash> marked_regions;
    std::vector<std::string> hashes = calculate_hashes(key_string, grid_size);

    mark_regions(marked_regions, hashes, grid_size);

    return traverse_regions(marked_regions, grid_size);
}

int main(int argc, char** argv)
{
    std::vector<int> key_string = read_ascii_from_file("input.txt");
    key_string.push_back('-');

    int grid_size = 128;

    int fragment_squares = calculate_fragment_squares(key_string, grid_size);
    int region_count = calculate_regions(key_string, grid_size);

    std::cout << "Fragment squares: " << fragment_squares << '\n';
    std::cout << "Region count: " << region_count << '\n';

    return 0;
}
