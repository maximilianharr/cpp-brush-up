#pragma once

class TrafficLight;

class TrafficLightInspector
{
public:
    void print(const TrafficLight &tl) const;
};

class TrafficLight
{
public:
    enum State
    {
        red,
        yellow,
        green
    };

private:
    State m_state{State::red};
    static constexpr double m_factor{1};
    friend void TrafficLightInspector::print(const TrafficLight &tl) const;

public:
    TrafficLight(State state);
    ~TrafficLight();
    void setState(State state);
    static double square(double x);
};
