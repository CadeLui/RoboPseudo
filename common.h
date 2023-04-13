#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
#include <cmath>

double linearFromTwoPoints(double pointOne[2], double pointTwo[2], double input)
{
    /*
    I keep hand-calcuating the equations to calculate the motor's mod values.
    I have finally decided to write code that does this for me.
    It finds the formula for a line between two points.
    It then runs the input parameter through the formula and returns the result.
    */
    double slope = (pointOne[1] - pointTwo[1]) / (pointOne[0] - pointTwo[0]);
    double intercept = pointOne[1] - slope*pointOne[0];
    return slope*input + intercept;
}

/*
    As the joysticks rest in a circle, trig has to be used
        in order to generate realistic joystick input.
    Fun. Unrelated to operating the robot itself, merely for sim purposes.
    <really proud of realizing and solving this :3>

    Iterates inclusively from 0 to 360 degrees by an iterator parameter that defaults to 15 degrees.
    It then gets the (for X cos, for Y sin) trig function for that angle, and then multiplies there result by the "amp"
    the "amp" is the distance of the joystick from the center.
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