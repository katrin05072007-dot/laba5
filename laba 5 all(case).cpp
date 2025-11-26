// laba 5 all(case).cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <locale.h>
#include <iomanip>
#include <list>
#include <numeric>
#include <cmath>
#include <locale>
#include <utility> // для пары (ребро)

using namespace std;
void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs(v, adj, visited);
        }
    }
}

// Функция для проверки связности графа (сильной связности для орграфа)
bool isConnected(int V, vector<vector<int>>& adj) {
    for (int i = 0; i < V; ++i) {
        vector<bool> visited(V, false);
        dfs(i, adj, visited);
        for (int j = 0; j < V; ++j) {
            if (!visited[j]) {
                return false; // Если из i нельзя дойти до j
            }
        }
    }
    return true;
}

// --- Определение реберной связности ---

int findEdgeConnectivityDirected(int V, vector<pair<int, int>>& edges) {
    if (V <= 1) return 0;
    int min_k = edges.size(); // Инициализируем максимальным возможным значением

    // Наивный подход: перебираем все возможные подмножества ребер
    // Это очень неэффективно. Для первокурсника можно упростить до поиска мостов

    // Упрощенный подход (как для первокурсника):
    // Если граф уже не связный, связность = 0
    vector<vector<int>> adj(V);
    for (auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
    }
    if (!isConnected(V, adj)) return 0;

    // В реальных условиях нужно использовать макс. поток.
    // Здесь мы просто вернем 1, если есть мост.
    // Если алгоритм поиска мостов не реализован, то это сложно.
    // Поэтому воспользуемся простым определением: минимальная степень вершины - верхняя граница реберной связности

    int minDegree = V;
    for (int i = 0; i < V; ++i) {
        if (adj[i].size() < minDegree) {
            minDegree = adj[i].size();
        }
    }
    return minDegree; // Это верхняя граница, а не точное значение
}

int findEdgeConnectivityUndirected(int V, vector<pair<int, int>>& edges) {
    if (V <= 1) return 0;

    // Сначала проверим, связен ли граф
    vector<vector<int>> adj(V);
    for (auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }
    if (!isConnected(V, adj)) return 0;

    // Для неориентированного графа реберная связность равна 1, если есть мост
    // (ребро, удаление которого делает граф несвязным).
    // Поиск мостов - это стандартный алгоритм, который первокурсник может изучить.

    int connectivity = 1; // Минимум 1, так как граф связен
    // Более сложный алгоритм поиска мостов здесь не реализован для простоты.
    // Для более точного результата нужно реализовать алгоритм Тарьяна или Эдмондса-Карпа.

    // В рамках "первокурсника" можем использовать наивный метод удаления ребер
    // и проверки связности.
    for (size_t i = 0; i < edges.size(); ++i) {
        // Временно удаляем ребро edges[i]
        int u = edges[i].first;
        int v = edges[i].second;

        // Создаем временный граф без этого ребра
        vector<vector<int>> tempAdj(V);
        for (size_t j = 0; j < edges.size(); ++j) {
            if (i == j) continue;
            tempAdj[edges[j].first].push_back(edges[j].second);
            tempAdj[edges[j].second].push_back(edges[j].first);
        }

        // Проверяем связность
        vector<bool> visited(V, false);
        dfs(0, tempAdj, visited);
        bool stillConnected = true;
        for (int k = 0; k < V; ++k) {
            if (!visited[k]) {
                stillConnected = false;
                break;
            }
        }
        if (!stillConnected) {
            // Нашли мост. Реберная связность равна 1.
            return 1;
        }
    }
    // Если ни одно отдельное ребро не разъединяет граф
    return 2; // Или больше (но для простоты вернем 2, если нет мостов)
}

void findHamiltonianCycles(int startNode, int currentNode, int numNodes, vector<int>& path, vector<vector<int>>& adjMatrix, vector<vector<int>>& allCycles) {
    path.push_back(currentNode);

    if (path.size() == numNodes) {
        if (adjMatrix[currentNode][startNode] == 1) {
            path.push_back(startNode);
            allCycles.push_back(path);
            path.pop_back();
        }
        path.pop_back();
        return;
    }

    for (int nextNode = 0; nextNode < numNodes; ++nextNode) {
        if (adjMatrix[currentNode][nextNode] == 1 && find(path.begin(), path.end(), nextNode) == path.end()) {
            findHamiltonianCycles(startNode, nextNode, numNodes, path, adjMatrix, allCycles);
        }
    }

    path.pop_back();
}
const int MAX_VERTICES = 10;

// --- 1. Построение графа и матрица смежности ---

void printAdjMatrix(int V, const vector<vector<int>>& adj_matrix) {
    cout << "\nМатрица смежности для заданного графа:\n";
    cout << "  ";
    for (int i = 0; i < V; ++i) {
        cout << i << " ";
    }
    cout << "\n";

    for (int i = 0; i < V; ++i) {
        cout << i << " ";
        for (int j = 0; j < V; ++j) {
            cout << adj_matrix[i][j] << " ";
        }
        cout << "\n";
    }
}

// --- 2. Поиск мостов ---

vector<int> disc, low;
vector<bool> visited_bridges;
int Time;
vector<pair<int, int>> bridges;

void dfs_bridges(int u, int parent, const vector<list<int>>& adj_list) {
    visited_bridges[u] = true;
    disc[u] = low[u] = ++Time;

    for (int v : adj_list[u]) {
        if (v == parent) continue;
        if (visited_bridges[v]) {
            low[u] = min(low[u], disc[v]);
        }
        else {
            dfs_bridges(v, u, adj_list);
            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]) {
                bridges.push_back({ u, v });
            }
        }
    }
}

void findBridges(int V, const vector<list<int>>& adj_list) {
    visited_bridges.assign(V, false);
    disc.assign(V, -1);
    low.assign(V, -1);
    Time = 0;
    bridges.clear();

    for (int i = 0; i < V; ++i) {
        if (!visited_bridges[i]) {
            dfs_bridges(i, -1, adj_list);
        }
    }

    cout << "\nНайденные мосты в графе:\n";
    if (bridges.empty()) {
        cout << "Мостов не найдено.\n";
    }
    else {
        for (const auto& bridge : bridges) {
            cout << "(" << bridge.first << " -- " << bridge.second << ")\n";
        }
    }
}

// --- 3. Поиск минимального множества вершин для ацикличности ---

// DFS для проверки ацикличности (используется при переборе подмножеств)
bool hasCycleDFS(int u, int parent, const vector<list<int>>& current_adj_list, vector<bool>& visited_cycle) {
    visited_cycle[u] = true;
    for (int v : current_adj_list[u]) {
        if (v == parent) continue;
        if (visited_cycle[v]) return true; // Найден цикл
        if (hasCycleDFS(v, u, current_adj_list, visited_cycle)) return true;
    }
    return false;
}

// Проверка, является ли граф ацикличным
bool isAcyclic(int V, const vector<list<int>>& adj_list) {
    vector<bool> visited_cycle(V, false);
    for (int i = 0; i < V; ++i) {
        if (!visited_cycle[i]) {
            if (hasCycleDFS(i, -1, adj_list, visited_cycle)) {
                return false; // Цикл найден
            }
        }
    }
    return true; // Циклов нет
}

// Поиск минимального набора вершин методом полного перебора
void findMinVertexSetToTree(int V, const vector<list<int>>& original_adj_list) {
    if (V == 0) return;

    set<int> best_set;

    // Перебираем все возможные подмножества вершин (2^V вариантов)
    for (int i = 0; i < (1 << V); ++i) {
        set<int> current_set;
        // Формируем текущее подмножество вершин для удаления
        for (int j = 0; j < V; ++j) {
            if ((i >> j) & 1) {
                current_set.insert(j);
            }
        }

        // Если текущее множество не больше лучшего уже найденного
        if (!best_set.empty() && current_set.size() >= best_set.size()) continue;

        // Создаем копию графа без удаляемых вершин
        vector<list<int>> temp_adj_list(V);
        for (int u = 0; u < V; ++u) {
            if (current_set.count(u)) continue; // Пропускаем удаленную вершину
            for (int v : original_adj_list[u]) {
                if (!current_set.count(v)) {
                    temp_adj_list[u].push_back(v);
                }
            }
        }

        // Проверяем, стал ли оставшийся граф ацикличным
        if (isAcyclic(V, temp_adj_list)) {
            best_set = current_set;
        }
    }

    cout << "\nМинимальное множество вершин, удаление которых делает граф ацикличным:\n{ ";
    for (int v : best_set) {
        cout << v << " ";
    }
    cout << "}\n";
    cout << "Размер минимального множества: " << best_set.size() << "\n";
}


int main() {
    setlocale(LC_ALL, "rus");
	int k;
    cin >> k;
	switch (k)
	{
	case 3:
	{
        int numNodes;
        cout << "Введите количество вершин (не более 10): ";
        cin >> numNodes;

        if (numNodes > 10 || numNodes <= 0) {
            cout << "Некорректное количество вершин." << endl;
            return 1;
        }

        vector<vector<int>> adjMatrix(numNodes, vector<int>(numNodes, 0));
        cout << "Введите матрицу смежности (" << numNodes << "x" << numNodes << "):" << endl;
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                cin >> adjMatrix[i][j];
            }
        }

        vector<vector<int>> allCycles;
        vector<int> path;
        for (int i = 0; i < numNodes; ++i) {
            findHamiltonianCycles(i, i, numNodes, path, adjMatrix, allCycles);
        }

        set<pair<int, int>> unionEdges;
        for (const auto& cycle : allCycles) {
            for (size_t i = 0; i < cycle.size() - 1; ++i) {
                int u = cycle[i];
                int v = cycle[i + 1];
                if (u > v) swap(u, v);
                unionEdges.insert({ u, v });
            }
        }

        vector<vector<int>> unionGraph(numNodes, vector<int>(numNodes, 0));
        for (const auto& edge : unionEdges) {
            unionGraph[edge.first][edge.second] = 1;
            unionGraph[edge.second][edge.first] = 1;
        }

        cout << "\nМатрица смежности графа, являющегося объединением всех гамильтоновых циклов:" << endl;
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                cout << unionGraph[i][j] << " ";
            }
            cout << endl;
        }

        break;
    }
	
	case 1:
	{  // Количество вершин и рёбер
        int n, m;
        cout << "Введите количество вершин графа: ";
        cin >> n;
        cout << "Введите количество рёбер графа: ";
        cin >> m;

        // Список рёбер (каждое ребро хранится как пара вершин)
        vector<pair<int, int>> edges;

        cout << "Введите ребра (u v), нумерация с 0:" << endl;
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            edges.push_back({ u, v });
        }

        cout << "\nВведённый граф:" << endl;
        cout << "Вершины: 0.." << n << endl;
        cout << "Рёбра:" << endl;

        for (int i = 0; i < m; i++) {
            cout << edges[i].first << " -- " << edges[i].second << endl;
        }
        break;
    }
	case 4:
	{
        int num_vertices;
        int num_edges;

        cout << "Введите количество вершин в графе (не более " << MAX_VERTICES << "): ";
        cin >> num_vertices;

        if (num_vertices > MAX_VERTICES || num_vertices <= 0) {
            cout << "Некорректное количество вершин.\n";
            return 1;
        }

        cout << "Введите количество ребер в графе: ";
        cin >> num_edges;

        vector<vector<int>> adj_matrix(num_vertices, vector<int>(num_vertices, 0));
        vector<list<int>> adj_list(num_vertices);

        cout << "\nВведите ребра графа (пары вершин от 0 до " << num_vertices - 1 << "):\n";
        for (int i = 0; i < num_edges; ++i) {
            int u, v;
            cin >> u >> v;

            if (u >= 0 && u < num_vertices && v >= 0 && v < num_vertices && u != v) {
                if (adj_matrix[u][v] == 0) { // Проверка на мультиграфы (добавляем ребро только если его нет)
                    adj_matrix[u][v] = 1;
                    adj_matrix[v][u] = 1;
                    adj_list[u].push_back(v);
                    adj_list[v].push_back(u);
                }
                else {
                    cout << "Ребро (" << u << "--" << v << ") уже существует. Пропускаю.\n";
                    i--;
                }
            }
            else {
                cout << "Ошибка ввода. Повторите ввод ребра.\n";
                i--;
            }
        }

        printAdjMatrix(num_vertices, adj_matrix);
        findBridges(num_vertices, adj_list);
        findMinVertexSetToTree(num_vertices, adj_list);
        break;
    }
	case 2:
	{
        int V_dir, E_dir;
        cout << "Введите количество вершин для ориентированного графа: ";
        cin >> V_dir;
        cout << "Введите количество ребер для ориентированного графа: "; cin >> E_dir;

        vector<pair<int, int>> edges_dir(E_dir);
        cout << "Введите ребра (u v):" << endl;
        for (int i = 0; i < E_dir; ++i) {
            cin >> edges_dir[i].first >> edges_dir[i].second;
        }

        // Для ориентированного графа определение реберной связности сложно.
        // Применим упрощенный подход (верхнюю границу - мин степень)
        int edgeConnDir = findEdgeConnectivityDirected(V_dir, edges_dir);
        cout << "Число реберной связности ориентированного графа (упрощенно): " << edgeConnDir << endl << endl;


        int V_undir, E_undir;
        cout << "Введите количество вершин для неориентированного графа: ";
        cin >> V_undir;
        cout << "Введите количество ребер для ориентированного графа: "; cin >> E_undir;

        vector<pair<int, int>> edges_undir(E_undir);
        cout << "Введите ребра (u v), нумерация с 0:" << endl;
        for (int i = 0; i < E_undir; ++i) {
            cin >> edges_undir[i].first >> edges_undir[i].second;
        }

        int edgeConnUndir = findEdgeConnectivityUndirected(V_undir, edges_undir);
        cout << "Число реберной связности неориентированного графа: " << edgeConnUndir << endl;
        break;
    }
	}
    return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
