#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>

struct Job
{
    std::vector<int> dependencies;
};

std::unordered_set<int> getStartJobs(std::unordered_map<int, Job> &jobs)
{
    std::unordered_set<int> allJobs;
    std::unordered_set<int> dependentJobs;
    std::unordered_set<int> startJobs;

    for (const auto &job : jobs)
    {
        allJobs.insert(job.first);
        for (int dependency : job.second.dependencies)
        {
            dependentJobs.insert(dependency);
        }
    }

    for (const auto &job : jobs)
    {
        if (dependentJobs.find(job.first) == dependentJobs.end())
        {
            startJobs.insert(job.first);
        }
    }

    return startJobs;
}

std::unordered_set<int> getEndJobs(std::unordered_map<int, Job> &jobs)
{
    std::unordered_set<int> allJobs;
    std::unordered_set<int> dependentJobs;
    std::unordered_set<int> endJobs;

    for (const auto &job : jobs)
    {
        allJobs.insert(job.first);
        for (int dependency : job.second.dependencies)
        {
            dependentJobs.insert(dependency);
        }
    }

    for (const auto &job : jobs)
    {
        if (dependentJobs.find(job.first) == dependentJobs.end())
        {
            endJobs.insert(job.first);
        }
    }

    return endJobs;
}

// Функция для проверки наличия циклов в DAG с использованием алгоритма поиска в глубину (DFS)
bool hasCycle(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph,
              std::string node,
              std::unordered_set<std::string> &visited,
              std::unordered_set<std::string> &recStack)
{
    if (graph.find(node) != graph.end())
    { // Проверка наличия ключа в словаре
        if (visited.find(node) == visited.end())
        {
            visited.insert(node);
            recStack.insert(node);

            for (const auto &neighbor : graph.at(node))
            {
                if (visited.find(neighbor) == visited.end() && hasCycle(graph, neighbor, visited, recStack))
                {
                    return true;
                }
                else if (recStack.find(neighbor) != recStack.end())
                {
                    return true;
                }
            }
        }
        recStack.erase(node);
    }
    return false;
}

// Функция для проверки наличия только одной компоненты связанности
void DFS(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph,
         const std::string &node,
         std::unordered_set<std::string> &visited)
{
    visited.insert(node);
    auto it = graph.find(node);
    if (it != graph.end())
    {
        for (const auto &neighbor : it->second)
        {
            if (visited.find(neighbor) == visited.end())
            {
                DFS(graph, neighbor, visited);
            }
        }
    }
}

bool hasSingleConnectedComponent(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph)
{
    if (graph.empty())
        return true;

    std::unordered_set<std::string> visited;
    int components = 0;

    for (const auto &node : graph)
    {
        if (visited.find(node.first) == visited.end())
        {
            DFS(graph, node.first, visited);
            components++;
        }
    }

    return components == 1;
}

int main()
{
    // Загрузка конфигурационного файла в формате YAML
    std::ifstream fin("config.yaml");
    YAML::Node config = YAML::Load(fin);
    fin.close();

    // Парсинг YAML и построение графа
    std::unordered_map<int, Job> jobs;
    int job_id;

    for (const auto &node : config)
    {
        std::string job = node.first.as<std::string>();
        for (const auto &dependency : node.second)
        {
            std::string dependentJob = dependency.as<std::string>();
            graph[job].insert(dependentJob);
        }
    }

    // Проверка наличия циклов в графе
    std::unordered_set<std::string> visited, recStack;
    bool hasCycles = false;

    for (const auto &node : graph)
    {
        if (hasCycle(graph, node.first, visited, recStack))
        {
            hasCycles = true;
            break;
        }
    }

    // Проверка наличия только одной компоненты связанности
    bool singleComponent = hasSingleConnectedComponent(graph);

    // Проверка наличия стартовых и завершающих джобов
    std::unordered_set<int> startJobs = getStartJobs(jobs);
    std::unordered_set<int> endJobs = getEndJobs(jobs);

    // Вывод результатов проверки
    if (hasCycles)
    {
        std::cout << "Граф содержит циклы.\n";
    }
    else if (!singleComponent)
    {
        std::cout << "Граф содержит несколько компонент связанности.\n";
    }
    else if (startJobs.empty() || endJobs.empty())
    {
        std::cout << "Отсутствуют стартовые или завершающие джобы!" << std::endl;
    }
    else
    {
        std::cout << "Проверка завершена. Граф корректен." << std::endl;
    }

    return 0;
}