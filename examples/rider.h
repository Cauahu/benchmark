#ifndef EXAMPLE_RIDEER_H_
#define EXAMPLE_RIDEER_H_
#include <cstddef>
#include <iostream>

class Rider {
public:
    Rider(std::size_t speed):_speed(speed)
    {

    }

    void deliverPackage(std::size_t distance)
    {
        std::size_t largeNumber = 10000u * distance / _speed; 
        volatile std::size_t targetNumber;
        while (largeNumber--)
            targetNumber = largeNumber;
        (void)targetNumber;
    }
private:
    std::size_t    _speed;
};
#endif
