#include <iostream>
#include <vector>
#include <fstream>

/*
 * Returns a sequence of single-digit integers from a file
 * with no delimiters.
 */
std::vector<int> load_sequence_from_file(const char* file)
{
    std::vector<int> sequence;

    std::fstream file_stream(file);

    if (file_stream.is_open())
    {
        char digit;

        while (file_stream >> digit)
        {
            sequence.push_back(digit - '0');
        }
    }

    return sequence;
}

/*
 * Returns the sum of each matching digit in a sequence of integers
 * where the matching digits are offset by some amount. Only acts
 * on a list of even size.
 */
int get_sequence_sum(const std::vector<int>& sequence, int offset)
{
    if (sequence.size() % 2 != 0)
        return 0;

    int sum = 0;

    for (int i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] == sequence[(i + offset) % sequence.size()])
            sum += sequence[i];
    }

    return sum;
}

int main(int argc, char** argv)
{
    std::vector<int> sequence = load_sequence_from_file("input.txt");

    int sum = get_sequence_sum(sequence, sequence.size() / 2);

    std::cout << "Sum of matching pairs in sequence: " << sum << '\n';

    return 0;
}
