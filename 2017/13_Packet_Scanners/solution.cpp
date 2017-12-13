#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct Layer
{
    unsigned int depth;
    unsigned int range;
};

struct Result
{
    int severity;
    bool collision;
};

std::vector<Layer> read_layers(const char* file)
{
    std::vector<Layer> layers;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line))
        {
            Layer layer;

            std::stringstream line_stream(line);
            std::string number_string;

            std::getline(line_stream, number_string, ':');
            layer.depth = std::stoi(number_string);

            std::getline(line_stream, number_string, ':');
            layer.range = std::stoi(number_string);

            layers.push_back(layer);
        }
    }

    file_stream.close();

    return layers;
}

bool is_caught(const Layer& layer, int delay)
{
    return ((layer.depth + delay) % ((layer.range * 2) - 2) == 0);
}

Result calculate_severity(const std::vector<Layer>& layers, int delay)
{
    Result result { 0, false };

    for (const Layer& layer : layers)
    {
        if (is_caught(layer, delay))
        {
            result.severity += layer.depth * layer.range;
            result.collision = true;
        }
    }

    return result;
}

/*
 * This is very much an unoptimized way of doing this
 */
int calculate_smallest_delay(const std::vector<Layer>& layers)
{
    int delay = 0;

    while (calculate_severity(layers, delay).collision == true)
    {
        delay++;
    }

    return delay;
}

int main(int argc, char** argv)
{
    std::vector<Layer> layers = read_layers("input.txt");

    int severity = calculate_severity(layers, 0).severity;
    int delay = calculate_smallest_delay(layers);

    std::cout << "Severity: " << severity << '\n';
    std::cout << "Smallest delay: " << delay << '\n';

    return 0;
}
