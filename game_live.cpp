#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <set>

using namespace std;

class Universe 
{
public:
    Universe(int rows, int cols) : rows(rows), cols(cols) 
    {
        grid.resize(rows, vector<bool>(cols, false));
    }

    void setCell(int row, int col, bool alive)
    {
        if (row >= 0 && row < rows && col >= 0 && col < cols) 
        {
            grid[row][col] = alive;
        }
    }

    void display() const 
    {
        for (const auto& row : grid) 
        {
            for (const auto& cell : row) 
            {
                cout << (cell ? '*' : '-') << ' ';
            }
            cout << endl;
        }
    }

    int countAliveCells() const 
    {
        int count = 0;
        for (const auto& row : grid) 
        {
            for (const auto& cell : row) 
            {
                if (cell) count++;
            }
        }
        return count;
    }

    void update() 
    {
        vector<vector<bool>> newGrid(rows, vector<bool>(cols, false));
        for (int r = 0; r < rows; ++r) 
        {
            for (int c = 0; c < cols; ++c) 
            {
                int aliveNeighbors = countAliveNeighbors(r, c);
                if (grid[r][c])
                {
                    newGrid[r][c] = (aliveNeighbors == 2 || aliveNeighbors == 3);
                }
                else
                {
                    newGrid[r][c] = (aliveNeighbors == 3);
                }
            }
        }
        grid = newGrid;
    }
    const vector<vector<bool>>& getGrid() const 
    {
        return grid;
    }

    private:
    int rows, cols;
    vector<vector<bool>> grid;

    int countAliveNeighbors(int row, int col) const 
    {
        int count = 0;
        for (int r = row - 1; r <= row + 1; ++r) 
        {
            for (int c = col - 1; c <= col + 1; ++c) 
            {
                if ((r == row && c == col) || r < 0 || r >= rows || c < 0 || c >= cols)
                    continue;
                if (grid[r][c]) count++;
            }
        }
        return count;
    }
};

bool areGridsEqual(const vector<vector<bool>>& grid1, const vector<vector<bool>>& grid2) 
{
    for (size_t i = 0; i < grid1.size(); ++i) 
    {
        if (grid1[i] != grid2[i]) return false;
    }
    return true;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    ifstream inputFile("gen1.txt");
    if (!inputFile.is_open())
    {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }

    int rows, cols;
    inputFile >> rows >> cols;

    Universe universe(rows, cols);
    int row, col;

    while (inputFile >> row >> col)
    {
        universe.setCell(row, col, true);
    }

    inputFile.close();

    int generation = 0;
    vector<vector<vector<bool>>> previousStates; // Вектор для хранения предыдущих состояний

    while (true)
    {
        std::system("cls");// Очищаем консоль

        cout << "Поколение: " << generation << endl;
        universe.display();
        int aliveCount = universe.countAliveCells();
        cout << "Количество живых клеток: " << aliveCount << endl;

        if (aliveCount == 0)
        {
            cout << "Игра завершена: все клетки мертвы." << endl;
            break;
        }

        const auto& currentGrid = universe.getGrid(); // Получаем текущее состояние

        // Проверяем на стагнацию
        for (const auto& state : previousStates)
        {
            if (areGridsEqual(currentGrid, state))
            {
                cout << "Игра завершена: достигнуто состояние стагнации." << endl;
                return 0; // Состояние уже встречалось
            }
        }
        previousStates.push_back(currentGrid); // Сохраняем текущее состояние
        universe.update();
        generation++;

        Sleep(1000); // Задержка на 1 секунду

        if (generation > 100)// Ограничение по поколениям
        {
            cout << "Игра завершена: достигнуто максимальное количество поколений." << endl;
            break;
        }
    }
    return 0;
}