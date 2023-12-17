#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>

std::unordered_set<int> getStartJobs(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph, bool &hasStartJobs)
{
    std::unordered_set<int> startJobs;

    std::unordered_set<std::string> allNodes;
    std::unordered_set<std::string> dependentNodes;

    for (const auto &node : graph)
    {
        allNodes.insert(node.first);
        for (const auto &dependency : node.second)
        {
            dependentNodes.insert(dependency);
        }
    }

    for (const auto &node : graph)
    {
        // Проверяем, является ли node.first числом перед преобразованием в int
        if (std::all_of(node.first.begin(), node.first.end(), ::isdigit))
        {
            int converted = std::stoi(node.first);
            if (dependentNodes.find(node.first) == dependentNodes.end() && allNodes.find(node.first) != allNodes.end())
            {
                startJobs.insert(converted);
            }
        }
    }

    hasStartJobs = !startJobs.empty(); // Обновление значения hasStartJobs

    return startJobs;
}

// std::unordered_set<int> getEndJobs(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph, bool &hasEndJobs)
// {
//     std::unordered_set<int> endJobs;

//     std::unordered_set<std::string> allNodes;
//     std::unordered_set<std::string> dependentNodes;

//     for (const auto &node : graph)
//     {
//         allNodes.insert(node.first);
//         for (const auto &dependency : node.second)
//         {
//             dependentNodes.insert(dependency);
//         }
//     }

//     for (const auto &node : graph)
//     {
//         for (const auto &neighbor : node.second)
//         {
//             dependentNodes.insert(neighbor); // Заполняем dependentNodes зависимостями других узлов
//             allNodes.insert(node.first);     // Заполняем allNodes текущими узлами
//         }
//     }

//     // Отфильтруем endJobs для получения корректных завершающих узлов
//     for (const auto &node : graph)
//     {
//         dependentNodes.erase(node.first);                             // Удаляем текущий узел из dependentNodes
//         dependentNodes.erase(node.second.begin(), node.second.end()); // Удаляем зависимости текущего узла из dependentNodes
//     }

//     for (const auto &node : graph)
//     {
//         // Проверяем, является ли node.first числом перед преобразованием в int
//         if (std::all_of(node.first.begin(), node.first.end(), ::isdigit))
//         {
//             int converted = std::stoi(node.first);
//             if (dependentNodes.find(node.first) == dependentNodes.end() && allNodes.find(node.first) != allNodes.end())
//             {
//                 endJobs.insert(converted);
//             }
//         }
//     }

//     hasEndJobs = !endJobs.empty(); // Обновление значения hasEndJobs

//     return endJobs;
// }

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

void DFS(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph,
         const std::string &node,
         std::unordered_set<std::string> &visited)
{
    visited.insert(node);
    auto it = graph.find(node);
    if (it != graph.end())
    {
        for (const auto &neighbor : graph.at(node))
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
    {
        return true;
    }

    std::unordered_set<std::string> visited;

    // Выбираем первый узел и проверяем связанность
    const auto &firstNode = graph.begin()->first;
    DFS(graph, firstNode, visited);

    // Проверяем, что все узлы были посещены (одна компонента связности)
    return visited.size() == graph.size();
}

int main()
{
    // Загрузка конфигурационного файла в формате YAML
    std::ifstream fin("config.yaml");
    YAML::Node config = YAML::Load(fin);
    fin.close();

    // Парсинг YAML и построение графа
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;

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
    bool hasStartJobs = false;
    bool hasEndJobs = false;
    std::unordered_set<int> startJobs = getStartJobs(graph, hasStartJobs);
    // std::unordered_set<int> endJobs = getEndJobs(graph, hasEndJobs);

    // Вывод результатов проверки
    if (hasCycles)
    {
        std::cout << "Граф содержит циклы.\n";
    }
    else if (!singleComponent)
    {
        std::cout << "Граф содержит несколько компонент связанности.\n";
    }
    else if (hasStartJobs || hasEndJobs)
    {
        std::cout << "Отсутствуют ";
        if (hasStartJobs)
            std::cout << "стартовые ";
        if (hasEndJobs)
            std::cout << "или завершающие ";
        std::cout << "джобы." << std::endl;
    }
    else
    {
        std::cout << "Граф корректен.\n";
    }

    return 0;
}