#include <iostream>

#include "point.h"
#include "traffic_light.h"
using std::cout;
using std::endl;

class Calc
{
private:
    int m_value{};

public:
    Calc()
        : m_value{0}
    {
        cout << "class created" << endl;
    }

    void add(int value)
    {
        this->m_value += value;
    }

    void substract(int value)
    {
        this->m_value -= value;
    }

    void print()
    {
        cout << "value: " << m_value << endl;
    }
};

int main()
{

    // 15.1
    Calc calc{};
    Calc *calcptr{&calc};
    calcptr->add(10);
    calcptr->print();

    // 15.2, 15.5
    Point<int> point(10, 10);
    point.print();

    // 15.3, 15.4, 15.5, 15.6
    TrafficLight tl(TrafficLight::State::green);

    // 15.7
    tl.square(10.);

    // 15.8, 15.9
    TrafficLightInspector tli;
    tli.print(tl);

    // 15.10
}
