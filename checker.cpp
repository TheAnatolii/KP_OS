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
#include <signal.h>
#include <unistd.h>

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
std::vector<std::unordered_set<int>> create_tree_from_up_to_down(const std::vector<Job> &jobs, std::unordered_set<int> &start_jobs)
{
    std::vector<std::unordered_set<int>> tree(jobs.size());

    for (auto &job : jobs)
    {
        if (job.depends_on.empty())
        {
            start_jobs.insert(job.name);
        }

        for (auto &dependence : job.depends_on)
        {
            tree[job.name - 1].insert(dependence);
        }
    }
    return tree;
}

// находим конечные джобы
void find_end_jobs(const std::vector<std::unordered_set<int>> &tree, std::unordered_set<int> &end_jobs)
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
            end_jobs.insert(i + 1);
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
bool check_cycle(std::vector<std::unordered_set<int>> &tree, std::unordered_set<int> &starts)
{
    std::vector<int> visited(tree.size());
    bool res = true;
    for (int start : starts)
    {
        res &= !find_cycle(tree, start - 1, visited);
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
bool check_connectivity(std::vector<std::unordered_set<int>> &tree, std::unordered_set<int> &starts)
{
    std::vector<int> visited(tree.size());
    bool res = true;
    size_t i = 0;
    dfs(tree, *(starts.begin()) - 1, visited);
    for (auto itr = std::next(starts.begin(), 1); itr != starts.end(); ++itr)
    {
        res &= !find_components(tree, *itr - 1, visited);
    }
    return res;
}

void killall_jobs(std::queue<std::pair<int, pid_t>> &q)
{
    while (!q.empty())
    {
        auto [id, pid] = q.front();
        q.pop();
        if (pid != -1)
            kill(pid, SIGINT);
    }
}

pid_t create_job(std::string command)
{
    std::stringstream ss{command};
    std::vector<std::string> strargs;
    std::vector<const char *> args;
    std::string str;
    while (ss >> str)
    {
        strargs.push_back(str);
        args.push_back(str.c_str());
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        execv(args[0], const_cast<char *const *>(args.data()));
        pid = -2;
        std::cerr << "exec error" << std::endl;
        exit(-2);
    }

    std::cout << "create " << pid << ": " << command << std::endl;
    return pid;
}

int main()
{

    std::vector<Job> jobs = parser("new_test.yaml"); // читаем файл

    std::unordered_set<int> start_jobs, end_jobs;
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

    for (auto &job : jobs)
    {
        std::string mutex_name = job.mutex_name;
        if (!mutex_name.empty())
        {
            mutex_names[job.name] = mutex_name;
            mutex_vals[mutex_name] = true;
        }
    }

    // начинаем запуск DAG'а
    std::queue<std::pair<int, pid_t>> waitq; // очередь задач
    std::mutex qmtx;                         // мьютекс для доступа к waitq

    std::thread wait_thread([&]()
                            {
        while (!end_jobs.empty() || !waitq.empty()) {
            if (!waitq.empty()) {
                pid_t pid;
                int id;
                {
                    std::lock_guard<std::mutex> lock(qmtx);
                    id = waitq.front().first;
                    pid = waitq.front().second;
                    waitq.pop();
                }
                // проверка нужно ли ждать этот процесс, -1 значит, что команда была запущена с помощью system()
                if (pid != -1) {
                    int sig;
                    // Ловим ошибку с помощью waitpid
                    std::cout << "wait " << pid << std::endl;
                    waitpid(pid, &sig, 0);
                    if (WIFSIGNALED(sig)) {
                        // если ошибка найдена, то завершаем все процессы и переходим к завершению программы
                        std::cout << "signal is heared from " << id << std::endl;
                        std::lock_guard<std::mutex> lock(qmtx);
                        end_jobs = {};
                        killall_jobs(waitq);
                    }
                }
                // Разблокируем мьютекс джобы
                {
                    std::lock_guard<std::mutex> lock(qmtx);
                    mutex_vals[mutex_names[id - 1]] = true;
                }
                // добавляем в очередь запуска готовые к выполнению джобы
                for (int i = 0; i < tree.size(); ++i) {
                    if (tree[i].count(id)) {
                        tree[i].erase(id);
                        if (tree[i].empty()) {
                            std::lock_guard<std::mutex> lock(qmtx);
                            start_jobs.insert(i + 1);
                        }
                    }
                }
                end_jobs.erase(id);
            }
        } });

    while (!end_jobs.empty())
    {
        // контейнер для запоминания запущенных джобов, чтобы удалить их из start_points
        std::vector<int> erase_id;
        std::lock_guard<std::mutex> lock(qmtx);
        for (int id : start_jobs)
        {
            if ((mutex_names.count(id) && mutex_vals[mutex_names[id]]) || !mutex_names.count(id))
            {
                std::string command = jobs[id - 1].command;
                if (command.front() != '.' && command.front() != '/')
                {
                    // если команда не является выполнимым файлом, то запускаем её с помощью system()
                    int success = std::system(command.c_str());
                    if (success == -1)
                    {
                        killall_jobs(waitq);
                    }
                    erase_id.push_back(id);
                    waitq.push({id, -1});
                }
                else
                {
                    // иначе делаем fork exec
                    pid_t pid = create_job(command);

                    // блокируем мьютекс джобы
                    if (mutex_names.count(id))
                        mutex_vals[mutex_names[id]] = false;

                    erase_id.push_back(id);
                    // добавляем процесс в очередь для ожидания
                    waitq.push({id, pid});
                }
            }
        }
        for (int id : erase_id)
        {
            start_jobs.erase(id);
        }
    }

    killall_jobs(waitq);
    wait_thread.join();
    return 0;
}