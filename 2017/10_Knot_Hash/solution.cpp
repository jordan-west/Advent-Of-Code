#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::vector<int> read_ints_from_file(const char* file)
{
    std::vector<int> numbers;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (getline(file_stream, line, ','))
        {
            std::stringstream string_stream(line);
            int number;

            string_stream >> number;

            numbers.push_back(number);
        }
    }

    file_stream.close();

    return numbers;
}

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

int main(int argc, char** argv)
{
    std::vector<int> lengths = read_ints_from_file("input.txt");
    std::vector<int> ascii_lengths = read_ascii_from_file("input.txt");
    std::vector<int> data;
    std::vector<int> data_for_ascii;

    int data_size = 256;

    data.reserve(data_size);
    data_for_ascii.reserve(data_size);

    for (int i = 0; i < data_size; i++)
    {
        data.push_back(i);
        data_for_ascii.push_back(i);
    }

    knot_hash(data, lengths, 1);
    int multiple = data[0] * data[1];
    std::cout << "First two multiplied: " << multiple << '\n';

    append_length_suffixes(ascii_lengths);
    knot_hash(data_for_ascii, ascii_lengths, 64);
    std::vector<int> dense_hash = create_dense_hash(data_for_ascii);
    std::string hexadecimal = to_hexadecimal(dense_hash);

    std::cout << "Hexadecimal: " << hexadecimal << '\n';

    return 0;
}
