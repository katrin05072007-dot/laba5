// laba 5 4.13.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <iomanip>
#include <locale.h>
#include <algorithm>
#include <list>
#include <numeric>
#include <cmath>
#include <set>

using namespace std;

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
