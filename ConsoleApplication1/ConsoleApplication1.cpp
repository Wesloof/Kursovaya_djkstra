#include <iostream>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// Функция для создания динамической матрицы
int** createMatrix(int size) {
    int** matrix = new int* [size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0; // Инициализация нулями
        }
    }
    return matrix;
}

// Функция для удаления динамической матрицы
void deleteMatrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Функция для отображения графа (матрицы смежности)
void displayGraph(int** graph, int nodes) {
    cout << "Матрица смежности графа:" << endl;
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

// Функция для ручного ввода рёбер
void manualInputEdges(int** graph, int nodes, bool isDirected) {
    int u, v, weight;
    int edges;
    cout << "Введите количество рёбер: ";
    cin >> edges;

    for (int i = 0; i < edges; i++) {
        cout << "Введите начало, конец и вес ребра (u v weight): ";
        cin >> u >> v >> weight;
        if (u < 0 && v < 0 && u >= nodes || v >= nodes) {
            cout << "Некорректные узлы, попробуйте снова." << endl;
            i--;
            continue;
        }
        graph[u][v] = weight;
        if (!isDirected) {
            graph[v][u] = weight; // Добавляем обратное ребро для неориентированного графа
        }
    }
}


// Функция для случайной генерации рёбер
void randomInputEdges(int** graph, int nodes, bool isDirected) {
    int maxWeight;
    cout << "Введите максимальный вес для рёбер: ";
    cin >> maxWeight;

    srand(time(0)); // Инициализация генератора случайных чисел

    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            if (i != j && rand() % 2) { // Случайное добавление ребра
                int weight = rand() % maxWeight + 1;
                graph[i][j] = weight;
                if (!isDirected) {
                    graph[j][i] = weight; // Добавляем обратное ребро
                }
            }
        }
    }
}


void printPath(int* predecessors, int target) {
    if (predecessors[target] == -1) {
        cout << target;
        return;
    }
    printPath(predecessors, predecessors[target]);
    cout << " -> " << target;
}

// Алгоритм Дейкстры с восстановлением пути
void dijkstra(int** graph, int nodes, int start) {
    int* distance = new int[nodes]; // Минимальные расстояния
    bool* visited = new bool[nodes]; // Посещённые узлы
    int* predecessors = new int[nodes]; // Предшественники для восстановления пути

    // Инициализация массивов
    for (int i = 0; i < nodes; i++) {
        distance[i] = INT_MAX; // "Бесконечность"
        visited[i] = false; // Все узлы не посещены
        predecessors[i] = -1; // Нет предшественников
    }
    distance[start] = 0; // Расстояние до начального узла

    for (int count = 0; count < nodes; count++) {
        // Находим узел с минимальным расстоянием
        int minDistance = INT_MAX, currentNode = -1;
        for (int i = 0; i < nodes; i++) {
            if (!visited[i] && distance[i] < minDistance) {
                minDistance = distance[i];
                currentNode = i;
            }
        }

        if (currentNode == -1) break; // Все узлы обработаны
        visited[currentNode] = true;

        // Обновляем расстояния до соседей
        for (int i = 0; i < nodes; i++) {
            if (graph[currentNode][i] != 0 && !visited[i]) {
                int newDistance = distance[currentNode] + graph[currentNode][i];
                if (newDistance < distance[i]) {
                    distance[i] = newDistance;
                    predecessors[i] = currentNode; // Запоминаем предшественника
                }
            }
        }
    }

    // Вывод результатов
    cout << "Кратчайшие расстояния от узла " << start << ":" << endl;
    for (int i = 0; i < nodes; i++) {
        if (distance[i] == INT_MAX) {
            cout << "До узла " << i << ": недостижимо" << endl;
        }
        else {
            cout << "До узла " << i << ": " << distance[i] << " (путь: ";
            printPath(predecessors, i);
            cout << ")" << endl;
        }
    }

    // Удаляем динамические массивы
    delete[] distance;
    delete[] visited;
    delete[] predecessors;
}

// Главное меню
void mainMenu() {
    int** graph = nullptr; // Динамическая матрица графа
    int nodes = 0;         // Количество узлов
    bool isDirected = false;

    while (true) {
        cout << "\nГлавное меню:" << endl;
        cout << "1. Установить количество узлов" << endl;
        cout << "2. Выбрать тип графа (ориентированный/неориентированный)" << endl;
        cout << "3. Ввести рёбра вручную" << endl;
        cout << "4. Сгенерировать рёбра случайным образом" << endl;
        cout << "5. Отобразить граф" << endl;
        cout << "6. Найти кратчайший путь (алгоритм Дейкстры)" << endl;
        cout << "7. Выход" << endl;

        int choice;
        cout << "Введите ваш выбор: ";
        cin >> choice;

        if (choice == 1) {
            // Установить количество узлов
            if (graph != nullptr) {
                deleteMatrix(graph, nodes);
            }
            cout << "Введите количество узлов: ";
            cin >> nodes;
            while (!(cin >> nodes) || nodes <= 0) {
                cout << "Введено неверное значение, попробуйте снова: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            graph = createMatrix(nodes);
            cout << "Количество узлов установлено на " << nodes << "." << endl;
        }
        else if (choice == 2) {
            // Установить тип графа
            cout << "Граф ориентированный? (1 - Да, 0 - Нет): ";
            cin >> isDirected;
            while (!(cin >> isDirected) || isDirected < 0 || isDirected > 1) {
                cout << "Введено неверное значение, попробуйте снова: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << (isDirected ? "Граф установлен как ориентированный." : "Граф установлен как неориентированный.") << endl;
        }
        else if (choice == 3) {
            // Ручной ввод рёбер
            if (nodes == 0 || graph == nullptr) {
                cout << "Сначала установите количество узлов." << endl;
            }
            else {
                manualInputEdges(graph, nodes, isDirected);
            }
        }
        else if (choice == 4) {
            // Случайная генерация рёбер
            if (nodes == 0 || graph == nullptr) {
                cout << "Сначала установите количество узлов." << endl;
            }
            else {
                randomInputEdges(graph, nodes, isDirected);
                cout << "Рёбра успешно сгенерированы." << endl;
            }
        }
        else if (choice == 5) {
            // Отобразить граф
            if (nodes == 0 || graph == nullptr) {
                cout << "Граф не задан." << endl;
            }
            else {
                displayGraph(graph, nodes);
            }
        }
        else if (choice == 6) {
            // Запустить алгоритм Дейкстры
            if (nodes == 0 || graph == nullptr) {
                cout << "Граф не задан." << endl;
            }
            else {
                int startNode;
                cout << "Введите стартовый узел: ";
                cin >> startNode;
                if (startNode < 0 || startNode >= nodes) {
                    cout << "Некорректный узел." << endl;
                }
                else {
                    dijkstra(graph, nodes, startNode);
                }
            }
        }
        else if (choice == 7) {
            // Выход
            cout << "Выход из программы." << endl;
            break;
        }
        else {
            cout << "Некорректный выбор. Попробуйте снова." << endl;
        }
    }

    // Освобождаем память
    if (graph != nullptr) {
        deleteMatrix(graph, nodes);
    }
}

// Точка входа
int main() {
    setlocale(LC_ALL, "");
    mainMenu();
    return 0;
}