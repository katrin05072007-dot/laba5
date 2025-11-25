// laba5 raschet.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <utility> // для пары (ребро)

using namespace std;

int main() {
    // Устанавливаем русскую локаль для вывода
    setlocale(LC_ALL, "rus");

    // Количество вершин и рёбер
    int n, m;
    cout << "Введите количество вершин графа: ";
    cin >> n;
    cout << "Введите количество рёбер графа: ";
    cin >> m;

    // Список рёбер (каждое ребро хранится как пара вершин)
    vector<pair<int, int>> edges;

    cout << "Введите рёбра (две вершины через пробел):" << endl;
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

    return 0;
}
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(int start, vector<vector<int>>& g, vector<bool>& used) {
    queue<int> q;
    q.push(start);
    used[start] = true;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : g[v]) {
            if (!used[u]) {
                used[u] = true;
                q.push(u);
            }
        }
    }
}

// Проверка связности (для неориентированного графа)
bool isConnected(vector<vector<int>>& g, int n) {
    vector<bool> used(n, false);
    bfs(0, g, used);
    for (int i = 0; i < n; i++) {
        if (!used[i]) return false;
    }
    return true;
}

// Проверка сильной связности (для ориентированного графа)
bool isStronglyConnected(vector<vector<int>>& g, int n) {
    for (int start = 0; start < n; start++) {
        vector<bool> used(n, false);
        bfs(start, g, used);
        for (int i = 0; i < n; i++) {
            if (!used[i]) return false;
        }
    }
    return true;
}

int edgeConnectivityDirected(int n, vector<pair<int, int>>& edges) {
    int m = edges.size();
    int minCut = m; // начальное значение
    for (int i = 0; i < m; i++) {
        // создаём копию графа без ребра i
        vector<vector<int>> g(n);
        for (int j = 0; j < m; j++) {
            if (i == j) continue;
            g[edges[j].first].push_back(edges[j].second);
        }
        if (!isStronglyConnected(g, n)) {
            minCut = 1; // нашли ребро, удаление которого рушит связность
            break;
        }
    }
    return minCut;
}

int edgeConnectivityUndirected(int n, vector<pair<int, int>>& edges) {
    int m = edges.size();
    int minCut = m;
    for (int i = 0; i < m; i++) {
        vector<vector<int>> g(n);
        for (int j = 0; j < m; j++) {
            if (i == j) continue;
            g[edges[j].first].push_back(edges[j].second);
            g[edges[j].second].push_back(edges[j].first);
        }
        if (!isConnected(g, n)) {
            minCut = 1;
            break;
        }
    }
    return minCut;
}

int main() {
    setlocale(LC_ALL, "rus");

    int n, m;
    cout << "Введите количество вершин ориентированного графа: ";
    cin >> n;
    cout << "Введите количество рёбер: ";
    cin >> m;

    vector<pair<int, int>> edgesDir;
    cout << "Введите рёбра (откуда -> куда):" << endl;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edgesDir.push_back({ u, v });
    }

    int connDir = edgeConnectivityDirected(n, edgesDir);
    cout << "Число рёберной связности ориентированного графа = " << connDir << endl;

    cout << "\nТеперь зададим неориентированный граф." << endl;
    cout << "Введите количество вершин: ";
    cin >> n;
    cout << "Введите количество рёбер: ";
    cin >> m;

    vector<pair<int, int>> edgesUndir;
    cout << "Введите рёбра (две вершины):" << endl;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edgesUndir.push_back({ u, v });
    }

    int connUndir = edgeConnectivityUndirected(n, edgesUndir);
    cout << "Число рёберной связности неориентированного графа = " << connUndir << endl;

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
