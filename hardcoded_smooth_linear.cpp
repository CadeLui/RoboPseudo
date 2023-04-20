#include <iostream>
#include "common.h"

/*
    Methods that take inputs and relate them to motor outputs
    Expects an X between -100 and 100, same for Y
    The linear equation contains two points:
    (0, 1) when in the center, don't change motor values

*/

double leftMotorMod(int joyX, int joyY)
{
    double theta = atan2(joyY, joyX);
    if (joyY < 0) theta += (2*M_PI);
    if (theta >= 0 && theta < M_PI/2)
        return -2*theta/M_PI+2;
    if (theta >= M_PI/2 && theta < M_PI)
        return -6*theta/M_PI+4;
    if (theta >= M_PI && theta < 3*M_PI/2)
        return 2*theta/M_PI-4;
    if (theta >= 3*M_PI/2)
        return 6*theta/M_PI-10;
    else
        return 1;
}

double rightMotorMod(int joyX, int joyY)
{
    double theta = atan2(joyY, joyX);
    if (joyY < 0) theta += (2*M_PI);
    if (theta >= 0 && theta < M_PI/2)
        return 6*theta/M_PI-2;
    if (theta >= M_PI/2 && theta < M_PI)
        return 2*theta/M_PI;
    if (theta >= M_PI && theta < 3*M_PI/2)
        return -6*theta/M_PI+8;
    if (theta >= 3*M_PI/2)
        return -2*theta/M_PI+2;
    else
        return 1;
}

/*
    Methods that use math to calculate motor speeds according to joystick position.
*/

std::vector<int> motorSpeeds(int joyY, int joyX)
{
    /*
        Takes the joystick input values and finds their distance relative to the origin
        Returns a Vector containing the motor speed's raw vector multiplied by a
            value defined by the horizontal position of the joystick.
        Value 0 is the speed of the left motor, Value 1 is the speed of the right motor.
    */
    int moveVector = sqrt(joyY*joyY + joyX*joyX)/2;
    std::vector<int> speeds {int(moveVector*leftMotorMod(joyX, joyY)), int(moveVector*rightMotorMod(joyX, joyY))};
    return speeds;
}

int main(void)
{
    int iterator = 15;
    int amp=100;
    std::vector<int> yVals = genJoyY(iterator, amp);
    std::vector<int> xVals = genJoyX(iterator, amp);
    std::cout << "y:x | lMotor:rMotor | joyAngle | lMotorMod:rMotorMod" << "\n";
    for (int i = 0; i < yVals.size(); i++)
    {
        std::vector<int> motors = motorSpeeds(yVals.at(i), xVals.at(i));
        std::cout << yVals.at(i) << ":" << xVals.at(i) << " | " 
        << motors.at(0) << ":" << motors.at(1) << " | " 
        << i*iterator <<  " | "
        << leftMotorMod(xVals.at(i), yVals.at(i)) << ":" 
        << rightMotorMod(xVals.at(i), yVals.at(i)) << " | "
        <<"\n";
    }
    std::cout << "\n";
    return 0;
}