#include <iostream>
#include <cmath>
#include <vector>

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
        return 1;
    if (theta >= M_PI/2 && theta < M_PI)
        return -4*theta/M_PI+3;
    if (theta >= M_PI && theta < 3*M_PI/2)
        return -1;
    if (theta >= 3*M_PI/2)
        return 4*theta/M_PI-7;
    else
        return 1;
}

double rightMotorMod(int joyX, int joyY)
{
    double theta = atan2(joyY, joyX);
    if (joyY < 0) theta += (2*M_PI);
    if (theta >= 0 && theta < M_PI/2)
        return 4*theta/M_PI-1;
    if (theta >= M_PI/2 && theta < M_PI)
        return 1;
    if (theta >= M_PI && theta < 3*M_PI/2)
        return -4*theta/M_PI+5;
    if (theta >= 3*M_PI/2)
        return -1;
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
    int moveVector = sqrt(joyY*joyY + joyX*joyX);
    std::vector<int> speeds {int(moveVector*leftMotorMod(joyX, joyY)), int(moveVector*rightMotorMod(joyX, joyY))};
    return speeds;
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

int main(void)
{
    int iterator = 15;
    int amp=100;
    std::vector<int> yVals = genJoyY(iterator, amp);
    std::vector<int> xVals = genJoyX(iterator, amp);
    std::cout << "y:x | lMotor:rMotor | joyAngle" << "\n";
    for (int i = 0; i < yVals.size(); i++)
    {
        std::vector<int> motors = motorSpeeds(yVals.at(i), xVals.at(i));
        std::cout << yVals.at(i) << ":" << xVals.at(i) << " | " 
        << motors.at(0) << ":" << motors.at(1) << " | " 
        << i*iterator <<  " | "
        //<< leftMotorMod(xVals.at(i), yVals.at(i)) << " | " 
        //<< rightMotorMod(xVals.at(i), yVals.at(i)) << " | "
        <<"\n";
    }
    std::cout << "\n";
    return 0;
}