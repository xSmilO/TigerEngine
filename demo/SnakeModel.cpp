#include "SnakeModel.h"
#include <algorithm>
#include <cstdlib>
#include <stdexcept>

namespace TigerDemo {
SnakeModel::SnakeModel(int width, int height, unsigned seed) : m_Width(width), m_Height(height), m_Random(seed) {
    if(width < 4 || height < 4 || width > 128 || height > 128)
        throw std::invalid_argument("Snake board dimensions must be between 4 and 128");
    Reset();
}

void SnakeModel::Reset() {
    const int x = m_Width / 2;
    const int y = m_Height / 2;
    m_Body = {{x, y}, {x - 1, y}, {x - 2, y}};
    m_Direction = m_NextDirection = {1, 0};
    m_TurnQueued = false;
    m_Finished = false;
    m_LastResult = StepResult::Moved;
    m_Score = 0;
    SpawnFood();
}

bool SnakeModel::QueueDirection(Cell direction) {
    if(m_Finished || m_TurnQueued || std::abs(direction.X) + std::abs(direction.Y) != 1)
        return false;
    if(direction.X == -m_Direction.X && direction.Y == -m_Direction.Y)
        return false;
    if(direction == m_Direction)
        return false;
    m_NextDirection = direction;
    m_TurnQueued = true;
    return true;
}

StepResult SnakeModel::Step() {
    if(m_Finished)
        return m_LastResult;
    m_Direction = m_NextDirection;
    m_TurnQueued = false;
    const auto head = m_Body.front();
    const Cell next{head.X + m_Direction.X, head.Y + m_Direction.Y};
    const bool eating = m_HasFood && next == m_Food;
    const auto collisionEnd = eating ? m_Body.end() : m_Body.end() - 1;
    if(next.X < 0 || next.Y < 0 || next.X >= m_Width || next.Y >= m_Height ||
       std::find(m_Body.begin(), collisionEnd, next) != collisionEnd) {
        m_Finished = true;
        return m_LastResult = StepResult::Lost;
    }
    m_Body.insert(m_Body.begin(), next);
    if(!eating) {
        m_Body.pop_back();
        return m_LastResult = StepResult::Moved;
    }
    m_Score += 10;
    SpawnFood();
    if(!m_HasFood) {
        m_Finished = true;
        return m_LastResult = StepResult::Won;
    }
    return m_LastResult = StepResult::Ate;
}

void SnakeModel::SpawnFood() {
    std::vector<Cell> free;
    for(int y = 0; y < m_Height; ++y)
        for(int x = 0; x < m_Width; ++x) {
            const Cell cell{x, y};
            if(std::find(m_Body.begin(), m_Body.end(), cell) == m_Body.end())
                free.push_back(cell);
        }
    m_HasFood = !free.empty();
    if(m_HasFood) {
        std::uniform_int_distribution<std::size_t> index(0, free.size() - 1);
        m_Food = free[index(m_Random)];
    }
}
} // namespace TigerDemo
