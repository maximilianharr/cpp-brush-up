#pragma once

#include <iostream>

template <typename T>
class Point
{
private:
    T m_x{};
    T m_y{};

    inline static int s_nextId{1};
    int m_id{};

public:
    Point(T x, T y)
        : m_x{x}, m_y{y}, m_id{s_nextId}
    {
        s_nextId++;
    }

    void print()
    {
        std::cout << "x: " << this->m_x << "| y: " << this->m_y << std::endl;
    }
};
