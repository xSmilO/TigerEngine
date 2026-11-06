#pragma once
#include <cstddef>
#include <random>
#include <vector>

namespace TigerDemo {
struct Cell {
    int X;
    int Y;
    bool operator==(const Cell& other) const { return X == other.X && Y == other.Y; }
    bool operator!=(const Cell& other) const { return !(*this == other); }
};

enum class StepResult { Moved, Ate, Lost, Won };

class SnakeModel {
public:
    explicit SnakeModel(int width = 20, int height = 16,
                        unsigned seed = std::random_device{}());
    void Reset();
    bool QueueDirection(Cell direction);
    StepResult Step();
    const std::vector<Cell>& Body() const { return m_Body; }
    Cell Direction() const { return m_Direction; }
    Cell Food() const { return m_Food; }
    bool HasFood() const { return m_HasFood; }
    int Width() const { return m_Width; }
    int Height() const { return m_Height; }
    int Score() const { return m_Score; }

private:
    void SpawnFood();
    int m_Width;
    int m_Height;
    std::mt19937 m_Random;
    std::vector<Cell> m_Body;
    Cell m_Direction{1, 0};
    Cell m_NextDirection{1, 0};
    Cell m_Food{0, 0};
    bool m_TurnQueued = false;
    bool m_HasFood = false;
    bool m_Finished = false;
    StepResult m_LastResult = StepResult::Moved;
    int m_Score = 0;
};
}
