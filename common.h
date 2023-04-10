#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
#include <cmath>
#include <iostream>

double linearFromTwoPoints(double pointOne[2], double pointTwo[2], double input)
{
    double slope = (pointOne[1] - pointTwo[1]) / (pointOne[0] - pointTwo[0]);
    double intercept = pointOne[1] - slope*pointOne[0];
    return slope*input + intercept;
}

/*
    As the joysticks rest in a circle, trig has to be used
        in order to generate realistic joystick input.
    Fun. Unrelated to operating the robot itself, merely for sim purposes.
    <really proud of realizing and solving this :3>
*/

std::vector<int> genJoyY(int iterator=15, int amp=100)
{
    std::vector<int> values;
    for (double i = 0; i < 361; i += iterator)
        values.push_back( (int) (amp*sin((i*M_PI)/180)) );
    return values;
}
std::vector<int> genJoyX(int iterator=15, int amp=100)
{
    std::vector<int> values;
    for (double i = 0; i < 361; i += iterator)
        values.push_back( (int) (amp*cos((i*M_PI)/180)) );
    return values;
}

#endif