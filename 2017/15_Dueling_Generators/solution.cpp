#include <iostream>
#include <fstream>
#include <utility>
#include <cstdint>
#include <limits>
#include <queue>

std::pair<int, int> read_ints_from_file(const char* file)
{
    std::pair<int, int> numbers;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string buffer;
        int number_a, number_b;

        file_stream >> buffer;
        file_stream >> buffer;
        file_stream >> buffer;
        file_stream >> buffer;
        file_stream >> number_a;

        file_stream >> buffer;
        file_stream >> buffer;
        file_stream >> buffer;
        file_stream >> buffer;
        file_stream >> number_b;

        numbers = std::make_pair(number_a, number_b);
    }

    file_stream.close();

    return numbers;
}

int judge(int64_t generator_a, int64_t generator_b)
{
    if ((generator_a & 0xffff) == (generator_b & 0xffff))
        return 1;

    return 0;
}

int count_pairs(const std::pair<int, int>& generator_starts, int64_t iterations, int64_t factor_a, int64_t factor_b, int64_t divisor)
{
    int pairs = 0;

    int64_t generator_a = generator_starts.first;
    int64_t generator_b = generator_starts.second;

    for (uint32_t i = 0; i < iterations; i++)
    {
        generator_a *= factor_a;
        generator_b *= factor_b;

        generator_a %= divisor;
        generator_b %= divisor;

        pairs += judge(generator_a, generator_b);
    }

    return pairs;
}

int count_pairs_multiples(const std::pair<int, int>& generator_starts, int64_t iterations, int64_t factor_a, int64_t factor_b, int64_t divisor, int multiple_a, int multiple_b)
{
    int pairs = 0;

    int64_t generator_a = generator_starts.first;
    int64_t generator_b = generator_starts.second;

    std::queue<int64_t> generator_a_values;
    std::queue<int64_t> generator_b_values;

    while (generator_a_values.size() < iterations || generator_b_values.size() < iterations)
    {
        generator_a *= factor_a;
        generator_b *= factor_b;

        generator_a %= divisor;
        generator_b %= divisor;

        if (generator_a % multiple_a == 0)
            generator_a_values.push(generator_a);

        if (generator_b % multiple_b == 0)
            generator_b_values.push(generator_b);
    }

    while (generator_a_values.empty() == false && generator_b_values.empty() == false)
    {
        pairs += judge(generator_a_values.front(), generator_b_values.front());

        generator_a_values.pop();
        generator_b_values.pop();
    }

    return pairs;
}

int main(int argc, char** argv)
{
    std::pair<int, int> generator_starts = read_ints_from_file("input.txt");

    int pairs = count_pairs(generator_starts, 40000000, 16807, 48271, 2147483647);
    int pairs_part_two = count_pairs_multiples(generator_starts, 5000000, 16807, 48271, 2147483647, 4, 8);

    std::cout << "(Part One) Number of pairs: " << pairs << '\n';
    std::cout << "(Part Two) Number of pairs: " << pairs_part_two << '\n';

    return 0;
}
