#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <sys/wait.h>
#include <thread>
#include <mutex>
#include <string>

// структура для джоба
struct Job
{
    int name;
    std::string command;
    std::vector<int> depends_on;
    std::string mutex_name;
};

// достаём инфу, просто собираем джобы в один вектор
std::vector<Job> parser(std::string filename)
{
    YAML::Node config = YAML::LoadFile("new_test.yaml");

    std::vector<Job> jobs;

    // Парсинг job'ов
    for (const auto &jobNode : config["jobs"])
    {
        Job job;
        job.name = jobNode["name"].as<int>();
        job.command = jobNode["command"].as<std::string>();
        if (jobNode["mutex"])
            job.mutex_name = jobNode["mutex"].as<std::string>();

        // Парсинг зависимостей
        const auto dependsOnNode = jobNode["depends_on"];
        if (dependsOnNode.IsSequence())
        {
            for (const auto &dependency : dependsOnNode)
            {
                job.depends_on.push_back(dependency.as<int>());
            }
        }

        jobs.push_back(job);
    }
    return jobs;
}

// создаём дерево, которое показывает переходы от данной вершины к её родителю(ям)
std::vector<std::unordered_set<int>> create_tree_from_up_to_down(const std::vector<Job> &jobs, std::vector<int> &start_jobs)
{
    std::vector<std::unordered_set<int>> tree(jobs.size());

    for (auto &job : jobs)
    {
        if (job.depends_on.empty())
        {
            start_jobs.push_back(job.name);
        }

        for (auto &dependence : job.depends_on)
        {
            tree[job.name - 1].insert(dependence);
        }
    }
    return tree;
}

// находим конечные джобы
void find_end_jobs(const std::vector<std::unordered_set<int>> &tree, std::vector<int> &end_jobs)
{
    std::vector<bool> is_top(tree.size(), true);

    for (const auto &children : tree)
    {
        for (auto &child : children)
        {
            is_top[child - 1] = false;
        }
    }

    for (size_t i = 0; i < is_top.size(); ++i)
    {
        if (is_top[i])
        {
            end_jobs.push_back(i + 1);
        }
    }
}

// находим циклы
bool find_cycle(std::vector<std::unordered_set<int>> &tree, int u, std::vector<int> &visited)
{
    if (visited[u] == 1)
    {
        return true;
    }
    if (visited[u] == 2)
    {
        return false;
    }
    visited[u] = 1;
    bool res = false;
    for (int v : tree[u])
    {
        res |= find_cycle(tree, v - 1, visited);
    }
    visited[u] = 2;
    return res;
}

// прогоняем функцию нахождения циклов для каждой вершины дерева
bool check_cycle(std::vector<std::unordered_set<int>> &tree, std::vector<int> &starts)
{
    std::vector<int> visited(tree.size());
    bool res = true;
    for (int start : starts)
    {
        res &= !find_cycle(tree, start - 1, visited);
        // for (auto &children : visited)
        // {
        //     std::cout << children << " ";
        // }
        // std::cout << std::endl;
    }
    return res;
}

// обход дерева в глубину и отметка посещённых вершин
void dfs(std::vector<std::unordered_set<int>> &tree, int u, std::vector<int> &visited)
{
    if (visited[u] == 1)
    {
        return;
    }
    visited[u] = 1;
    bool res = true;
    for (int v : tree[u])
    {
        dfs(tree, v - 1, visited);
    }
}

/* ищет компоненты отделенные от графа
 * @return true, если текущий компонент не связан с графом, false, если связь есть.
 */
bool find_components(std::vector<std::unordered_set<int>> &tree, int u, std::vector<int> &visited)
{
    if (visited[u] == 1)
    {
        return false;
    }
    if (tree[u].empty())
    {
        return true;
    }
    visited[u] = 1;
    bool res = true;
    for (int v : tree[u])
    {
        res &= find_components(tree, v - 1, visited);
    }
    return res;
}

// проверка графа на одну компоненту связности
bool check_connectivity(std::vector<std::unordered_set<int>> &tree, std::vector<int> &starts)
{
    std::vector<int> visited(tree.size());
    bool res = true;
    size_t i = 0;
    dfs(tree, starts[i++] - 1, visited);
    for (; i < starts.size(); i++)
    {
        res &= !find_components(tree, starts[i] - 1, visited);
    }
    return res;
}

int main()
{

    std::vector<Job> jobs = parser("new_test.yaml"); // читаем файл

    std::vector<int> start_jobs, end_jobs;
    // создаём дерево, и сразу находим начальные job'ы
    std::vector<std::unordered_set<int>> tree = create_tree_from_up_to_down(jobs, start_jobs);

    // находим конечные job'ы
    find_end_jobs(tree, end_jobs);

    // проверяем граф по заданию
    if (start_jobs.empty())
    {
        std::cerr << "Error: Отсутствуют начальные джобы" << std::endl;
        return -1;
    }
    if (end_jobs.empty())
    {
        std::cerr << "Error: Отсутствуют завершающие джобы" << std::endl;
        return -1;
    }
    if (!check_cycle(tree, end_jobs))
    {
        std::cerr << "Error: В графе есть циклы" << std::endl;
        return -1;
    }
    if (!check_connectivity(tree, end_jobs))
    {
        std::cerr << "Error: В графе больше одной компоненты связности" << std::endl;
        return -1;
    }

    // читаем мьютексы, указанные у джобов
    std::unordered_map<std::string, bool> mutex_vals;
    std::unordered_map<int, std::string> mutex_names;

    for (auto job : jobs)
    {
        std::string mutex_name = job.mutex_name;
        if (!mutex_name.empty())
        {
            mutex_names[job.name] = mutex_name;
            mutex_vals[mutex_name] = true;
        }
    }

    // for (auto &mutex : mutex_names)
    // {
    //     std::cout << mutex.first << " " << mutex.second << std::endl;
    // }

    // начинаем запуск DAG'а
    std::queue<std::pair<int, pid_t>> waitq; // очередь задач
    std::mutex qmtx;                         // мьютекс для доступа к waitq

    return 0;
}