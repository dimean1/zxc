// ConsoleApplication14.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

// Размер игрового поля
const int SIZE_3x3 = 3;
const int SIZE_4x4 = 4;

// Структура для хранения состояния игры
struct Game {
    int size;
    int** board;
    int emptyRow;
    int emptyCol;
    int movesCount;
    chrono::steady_clock::time_point startTime;
};

// Функция для инициализации игрового поля
void initBoard(Game& game) {
    game.board = new int* [game.size];
    for (int i = 0; i < game.size; i++) {
        game.board[i] = new int[game.size];
    }

    int counter = 1;
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            game.board[i][j] = counter++;
        }
    }
    game.board[game.size - 1][game.size - 1] = 0; // Пустая клетка
    game.emptyRow = game.size - 1;
    game.emptyCol = game.size - 1;
}

// Функция для вывода игрового поля
void printBoard(const Game& game) {
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            if (game.board[i][j] == 0) {
                cout << "  ";
            }
            else {
                cout << game.board[i][j] << " ";
            }
        }
        cout << endl;
    }
}

// Функция для перемещения пятнашки
bool moveTile(Game& game, int row, int col) {
    if (abs(row - game.emptyRow) + abs(col - game.emptyCol) != 1) {
        return false; // Перемещение недействительно
    }
    swap(game.board[row][col], game.board[game.emptyRow][game.emptyCol]);
    game.emptyRow = row;
    game.emptyCol = col;
    game.movesCount++;
    return true;
}

// Функция для проверки выигрыша
bool isGameWon(const Game& game) {
    int counter = 1;
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            if (game.board[i][j] != counter && counter != game.size * game.size) {
                return false;
            }
            counter++;
        }
    }
    return true;
}

// Функция для случайного перемешивания пятнашек
void shuffleBoard(Game& game) {
    for (int i = 0; i < game.size * game.size * 5; i++) { // Количество перемешиваний
        int row, col;
        do {
            row = rand() % game.size;
            col = rand() % game.size;
        } while (game.board[row][col] == 0);
        moveTile(game, row, col);
    }
}

// Функция для хода игрока
void playerMove(Game& game) {
    setlocale(LC_ALL, "rus");
    int direction;
    cout << "Введите направление (0-верх, 1-низ, 2-вправо, 3-влево): ";
    cin >> direction;

    if (direction < 0 || direction > 3) {
        cout << "Неверное направление. Введите число от 0 до 3." << endl;
        return;
    }

    int row = game.emptyRow;
    int col = game.emptyCol;

    switch (direction) {
    case 0: // Вверх
        row--;
        break;
    case 1: // Вниз
        row++;
        break;
    case 2: // Вправо
        col++;
        break;
    case 3: // Влево
        col--;
        break;
    }

    if (moveTile(game, row, col)) {
        printBoard(game);
    }
    else {
        cout << "Невозможно выполнить этот ход!" << endl;
    }
}

// Функция для хода компьютера (простая стратегия)
bool computerMove(Game& game) {
    setlocale(LC_ALL, "rus");
    int bestRow = game.emptyRow;
    int bestCol = game.emptyCol;
    int minDistance = game.size * game.size;

    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            if (game.board[i][j] != 0) {
                int distance = abs(i - (game.board[i][j] - 1) / game.size) + abs(j - (game.board[i][j] - 1) % game.size);
                if (distance < minDistance) {
                    minDistance = distance;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    if (moveTile(game, bestRow, bestCol)) {
        cout << "Компьютер сделал ход." << endl;
        printBoard(game);
        return true;
    }

    return false; // Если ход невозможен
}

int main() {
    setlocale(LC_ALL, "rus");
    srand(time(0));

    int choice;
    cout << "Выберите размер игры:" << endl;
    cout << "1. 8 (3х3)" << endl;
    cout << "2. 15 (4х4)" << endl;
    cin >> choice;

    Game game;
    if (choice == 1) {
        game.size = SIZE_3x3;
    }
    else if (choice == 2) {
        game.size = SIZE_4x4;
    }
    else {
        cout << "Неверный выбор. Завершение программы." << endl;
        return 0;
    }

    initBoard(game);

    cout << "\nВыберите режим перемешивания:" << endl;
    cout << "1. Ручное перемешивание" << endl;
    cout << "2. Компьютерное перемешивание" << endl;
    cin >> choice;

    if (choice == 1) {
        cout << "Перемешайте пятнашки вручную:\n";
        printBoard(game);
    }
    else if (choice == 2) {
        shuffleBoard(game);
        cout << "\nПеремешанные пятнашки:\n";
        printBoard(game);
    }
    else {
        cout << "Неверный выбор. Завершение программы." << endl;
        return 0;
    }

    cout << "\nВыберите режим игры:" << endl;
    cout << "1. Человек собирает" << endl;
    cout << "2. Компьютер собирает" << endl;
    cin >> choice;

    game.movesCount = 0;
    game.startTime = chrono::steady_clock::now();

    if (choice == 1) {
        cout << "\nСобирайте пятнашки:\n";
        while (true) {
            playerMove(game);
            if (isGameWon(game)) {
                break;
            }
        }
    }
    else if (choice == 2) {
        cout << "\nКомпьютер собирает пятнашки:\n";
        while (true) {
            if (!computerMove(game)) {
                cout << "Компьютер не может сделать ход. Вы выиграли!" << endl;
                break;
            }
            if (isGameWon(game)) {
                break;
            }
        }
    }
    else {
        cout << "Неверный выбор. Завершение программы." << endl;
        return 0;
    }

    chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
    chrono::duration<double> elapsedSeconds = endTime - game.startTime;
    cout << "\nИгра завершена!" << endl;
    cout << "Время: " << elapsedSeconds.count() << " секунд" << endl;
    cout << "Перемещений: " << game.movesCount << endl;

    // Освобождение памяти для игрового поля
    for (int i = 0; i < game.size; i++) {
        delete[] game.board[i];
    }
    delete[] game.board;

    return 0;
}