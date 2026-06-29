#include <iostream>
#include "traffic_light.h"

TrafficLight::TrafficLight(State state)
    : m_state{state}
{
    std::cout << "Created" << std::endl;
}

TrafficLight::~TrafficLight()
{
    std::cout << "deleting TL" << std::endl;
}

void TrafficLight::setState(State state)
{
    this->m_state = state;
}

double TrafficLight::square(double x)
{
    // m_state = State::red; //static function cannot access non-static members
    return m_factor * x * x;
}

void TrafficLightInspector::print(const TrafficLight &tl) const
{
    std::cout << tl.m_state << std::endl;
}
