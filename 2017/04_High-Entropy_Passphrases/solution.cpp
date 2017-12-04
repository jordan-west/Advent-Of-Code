#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

std::vector<std::vector<std::string>> read_passphrases_from_file(const char* file)
{
    std::vector<std::vector<std::string>> passphrases;

    std::ifstream file_stream(file);

    int current_phrase = 0;
    
    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line))
        {
            passphrases.emplace_back();

            std::istringstream string_stream(line);

            std::string word;

            while (string_stream >> word)
            {
                passphrases[current_phrase].push_back(word);
            }

            current_phrase++;
        }
    }

    file_stream.close();

    return passphrases;
}

bool is_passphrase_valid(const std::vector<std::string>& passphrase)
{
    std::unordered_map<std::string, bool> words;

    for (const std::string& word : passphrase)
    {
        if (words[word] == true)
            return false;

        words[word] = true;
    }

    return true;
}

bool is_passphrase_valid_non_anagram(const std::vector<std::string>& passphrase)
{
    std::unordered_map<std::string, bool> words;

    for (std::string word : passphrase)
    {
        std::sort(word.begin(), word.end());

        if (words[word] == true)
            return false;

        words[word] = true;
    }

    return true;
}

int sum_of_valid_passphrases(const std::vector<std::vector<std::string>>& passphrases)
{
    int sum = 0;

    for (const std::vector<std::string>& passphrase : passphrases)
    {
        if (is_passphrase_valid(passphrase))
            sum++;
    }

    return sum;
}

int sum_of_valid_anagram_passphrases(const std::vector<std::vector<std::string>>& passphrases)
{
    int sum = 0;

    for (const std::vector<std::string>& passphrase : passphrases)
    {
        if (is_passphrase_valid_non_anagram(passphrase))
            sum++;
    }

    return sum;
}

int main(int argc, char** argv)
{
    auto passphrases = read_passphrases_from_file("input.txt");

    std::cout << "Sum of valid passphrases: " << sum_of_valid_passphrases(passphrases) << '\n';
    std::cout << "Sum of valid non-anagram passphrases: " << sum_of_valid_anagram_passphrases(passphrases) << '\n';

    return 0;
}
