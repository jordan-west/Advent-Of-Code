#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

struct Node
{
    std::string name;
    int weight;
    std::vector<Node*> connecting_nodes;
};

std::vector<Node> read_nodes_from_file(const char* file)
{
    std::vector<Node> nodes;
    std::unordered_map<std::string, std::vector<std::string>> children;

    std::ifstream file_stream(file);

    if (file_stream.is_open())
    {
        std::string line;

        while (std::getline(file_stream, line))
        {
            Node temp_node;

            std::string temp_buffer;
            std::stringstream string_stream(line);

            string_stream >> temp_node.name;
            string_stream >> temp_buffer;

            temp_buffer.erase(temp_buffer.begin());
            temp_buffer.erase(temp_buffer.end());
            
            temp_node.weight = std::stoi(temp_buffer);

            std::vector<std::string> children_names;

            if (string_stream >> temp_buffer)
            {
                while (string_stream >> temp_buffer)
                {
                    if (temp_buffer[temp_buffer.size() - 1] == ',')
                    {
                        temp_buffer.erase(temp_buffer.end() - 1);
                    }

                    children_names.push_back(temp_buffer);
                }
            }

            if (children_names.size() > 0)
            {
                children[temp_node.name] = children_names;
            }

            nodes.push_back(temp_node);
        }
    }

    file_stream.close();

    for (Node& node : nodes)
    {
        if (children.find(node.name) != children.end())
        {
            for (const std::string& child_name : children[node.name])
            {
                for (Node& other : nodes)
                {
                    if (other.name == child_name)
                    {
                        node.connecting_nodes.push_back(&other);

                        break;
                    }
                }
            }
        }
    }

    return nodes;
}

Node find_root(const std::vector<Node>& nodes)
{
    std::unordered_set<std::string> has_parent;

    for (const Node& node : nodes)
    {
        for (const Node* child : node.connecting_nodes)
        {
            has_parent.insert(child->name);
        }
    }

    for (const Node& node : nodes)
    {
        if (has_parent.find(node.name) == has_parent.end())
            return node;
    }
    
    Node fail_node;
    fail_node.name = "FAIL";

    return fail_node;
}

int find_sum(const Node* node)
{
    int sum = node->weight;

    for (const Node* child : node->connecting_nodes)
    {
        sum += find_sum(child);
    }

    return sum;
}

const Node* find_inbalanced_node(const Node* node)
{
    int previous_sum = -1;

    for (const Node* child : node->connecting_nodes)
    {
        const Node* inbalanced_node = find_inbalanced_node(child);

        if (inbalanced_node != nullptr)
            return inbalanced_node;
        else
        {
            int sum = find_sum(child);

            if (previous_sum != sum && previous_sum != -1)
            {
                return node;
            }

            previous_sum = sum;
        }
    }

    return nullptr;
}

int find_balance(const Node& root)
{
    int balance = -1;

    const Node* inbalanced_node = find_inbalanced_node(&root);

    if (inbalanced_node == nullptr)
        return balance;

    std::unordered_map<int, int> sum_occurrences;
    std::unordered_map<std::string, int> node_to_sum;

    for (const Node* child : inbalanced_node->connecting_nodes)
    {
        int sum = find_sum(child);

        node_to_sum[child->name] = sum;

        if (sum_occurrences.find(sum) == sum_occurrences.end())
        {
            sum_occurrences[sum] = 1;
        }
        else
        {
            sum_occurrences[sum]++;
        }
    }

    const Node* node_to_change = nullptr;
    int equal_sum = -1;

    for (auto it = sum_occurrences.begin(); it != sum_occurrences.end(); it++)
    {
        if (it->second == 1)
        {
            for (const Node* child : inbalanced_node->connecting_nodes)
            {
                if (node_to_sum[child->name] == it->first)
                {
                    node_to_change = child;
                }
            }
        }
        else
        {
            equal_sum = it->first;
        }
    }

    if (node_to_change == nullptr)
        return balance;

    int difference = node_to_sum[node_to_change->name] - equal_sum;
    balance = node_to_change->weight - difference;

    return balance;
}

int main(int argc, char** argv)
{
    std::vector<Node> nodes = read_nodes_from_file("input.txt");

    Node root_node = find_root(nodes);
    std::string root_node_name = root_node.name;
    int balance = find_balance(root_node);

    std::cout << "Root node: " << root_node_name << '\n';

    std::cout << "Weight needed to balance: " << balance << '\n';

    return 0;
}
