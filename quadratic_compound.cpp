#include <iostream>
#include <cmath>
#include <vector>

double leftMotorMod(int joyX)
{
    double newJoyX = joyX;
    if (joyX < -50)
        return ((newJoyX*newJoyX)/2500 + (2*newJoyX)/25 + 3);
    else if (joyX < 0)
        return (newJoyX*newJoyX)/-2500+1;
    else
        return 1;
}

double rightMotorMod(int joyX)
{
    double newJoyX = joyX;
    if (joyX > 50)
        return ((newJoyX*newJoyX)/2500 - (2*newJoyX)/25 + 3);
    else if (joyX > 0)
        return (newJoyX*newJoyX)/-2500+1;
    else
        return 1;
}

std::vector<int> motorSpeeds(int joyY, int joyX)
{
    /*
        Takes the joystick input values and finds their distance relative to the origin.
        Returns a Vector containing the motor speed's raw vector multiplied by a
            value defined by the horizontal position of the joystick.
        Value 0 is the speed of the left motor, Value 1 is the speed of the right motor.
    */
    int moveVector = (int) (sqrt(joyY*joyY + joyX*joyX));
    std::vector<int> speeds {int(moveVector*leftMotorMod(joyX)), int(moveVector*rightMotorMod(joyX))};
    return speeds;
}

/*
    As the joysticks rest in a circle, trig has to be used
        in order to generate realistic joystick input.
    Fun. Unrelated to operating the robot itself, merely for sim purposes.
*/

std::vector<int> genJoyY(int iterator=15, int amp=100)
{
    std::vector<int> values;
    for (double i = 0; i < 360; i += iterator)
        values.push_back( (int) (amp*sin((i*M_PI)/180)) );
    return values;
}
std::vector<int> genJoyX(int iterator=15, int amp=100)
{
    std::vector<int> values;
    for (double i = 0; i < 360; i += iterator)
        values.push_back( (int) (amp*cos((i*M_PI)/180)) );
    return values;
}

int main(void)
{
    int iterator = 15;
    int amp=100;
    std::vector<int> yVals = genJoyY(iterator, amp);
    std::vector<int> xVals = genJoyX(iterator, 100);
    std::cout << "y:x | lMotor:rMotor | joyAngle" << "\n";
    for (int i = 0; i < yVals.size(); i++)
    {
        std::vector<int> motors = motorSpeeds(yVals.at(i), xVals.at(i));
        std::cout << yVals.at(i) << ":" << xVals.at(i) << " | " 
        << motors.at(0) << ":" << motors.at(1) << " | " 
        << i*iterator <<  "\n";
    }
    std::cout << "\n";
    for (int x = -100; x < 101; x += 10)
        std::cout << leftMotorMod(x) << " | ";
    std::cout << "\n";
    return 0;
}
