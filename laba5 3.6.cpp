// laba5 3.6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <locale>

using namespace std;

// Функция для поиска всех гамильтоновых циклов
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

int main() {
    setlocale(LC_ALL, "rus");

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

    return 0;
}

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
