#include <iostream>
#include <vector>
#include <list>

std::list<int>::const_iterator step(std::list<int>::const_iterator start_iterator, const std::list<int>& buffer, int steps)
{
    while (steps > 0)
    {
        start_iterator++;

        if (start_iterator == buffer.end())
            start_iterator = buffer.begin();

        steps--;
    }

    return start_iterator;
}

void print_list(const std::list<int>& buffer, const std::list<int>::const_iterator& current_iterator)
{
    for (auto iterator = buffer.begin(); iterator != buffer.end(); iterator++)
    {
        if (*iterator == *current_iterator)
            std::cout << '(' << *iterator << ") ";
        else
            std::cout << *iterator << ' ';
    }

    std::cout << '\n';
}

int spinlock(int jump, int times)
{
    std::list<int> buffer { 0 };

    std::list<int>::const_iterator current_iterator = buffer.begin();

    for (int i = 1; i <= times; i++)
    {
        current_iterator = step(current_iterator, buffer, jump);
        current_iterator++;
        buffer.insert(current_iterator, i);
        current_iterator--;
    }

    current_iterator++;

    if (current_iterator == buffer.end())
        current_iterator = buffer.begin();

    return *current_iterator;
}

int zero_spinlock(int jump, int times)
{
    int position = 0;
    int value = 0;

    for (int i = 1; i <= times; i++)
    {
        position = (position + jump + 1) % i;

        if (position == 0)
            value = i;
    }

    return value;
}

int main(int argc, char** argv)
{
    int final_value = spinlock(370, 2017);
    int zero_value = zero_spinlock(370, 50000000);

    std::cout << "Final value: " << final_value << '\n';
    std::cout << "Zero value: " << zero_value << '\n';

    return 0;
}
