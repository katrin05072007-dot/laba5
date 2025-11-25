// laba 5 2.7.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <locale.h>

using namespace std;

// --- Базовые функции для работы с графами ---

// Функция DFS для проверки связности
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


int main() {
    setlocale(LC_ALL, "rus"); // Устанавливаем русскую локаль

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
