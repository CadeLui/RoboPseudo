#include <iostream>
#include "common.h"

const double _speedMod = 1;

double motorMod(int joyX, int joyY, double points[], int numOfPoints)
{
    /*
    A joystick lies within a circle, that we know.
    We want to modulate the speed of the motors according to where the joystick is on the circle.
    This means we need the joystick's angle to act as an input.
    From this input, we will do some math to get an output number that can be used to change the motor's speed.
    We don't want to write all the math by hand, so we'll have the computer auto generate those formulas.
    For the most part, we care about the joystick having a certain value at specific points on a circle.
        Joystick forward = everything forward, joystick backward = everything backward, joystick left = spin counterclockwise, joystick right = spin clockwise
    These points are evenly spaced on a circle, at 90, 180, 270, and 0/360 degrees respectively
    This program can take a number of desired outputs and place them evenly on a circle, starting at 0 degrees.
    It will then automatically generate lines that include these two points and test if the joystick's value is within that function.
    It will then apply the formula and get the modifier out.
    */

    // Define an array that will carry each point, as well as what number will be divisor when getting each section of a circle.
    double setOfPoints[numOfPoints][2][2];
    int angleDiv = numOfPoints/2;

    // Generates a list of pairs of points. Each pair is on a connected line.
    // These pairs will then be used to generate the formulas.
    for (int i = 0; i < numOfPoints; i++)
    {
        setOfPoints[i][0][0] = i*M_PI/angleDiv;
        setOfPoints[i][0][1] = points[i];
        setOfPoints[i][1][0] = (i+1)*M_PI/angleDiv;
        if (i+1 != numOfPoints)
            setOfPoints[i][1][1] = points[i+1];
        else
            setOfPoints[i][1][1] = points[0];
    }

    // Derive the angle of the joystick from it's X and Y position
    double theta = atan2(joyY, joyX);
    if (joyY < 0) theta += (2*M_PI);

    // Iterate through each point pair. If theta is inclusively between these points, apply their formula.
    for (int i = 0; i < numOfPoints; i++)
    {
        if (theta >= setOfPoints[i][0][0] && theta <= setOfPoints[i][1][0])
            return linearFromTwoPoints(setOfPoints[i][0], setOfPoints[i][1], theta);
    }

    // If, for whatever reason, theta is not inclusively between 0 and 360, just return 0.
    return 0;
}

/*
    Methods that use math to calculate motor speeds according to joystick position.
*/

std::vector<int> motorSpeeds(int joyY, int joyX)
{
    /*
        Takes the joystick input values and finds their distance relative to the origin
        Returns a Vector containing the motor speed's raw vector multiplied by a value defined by the horizontal position of the joystick.
        Value 0 is the speed of the left motor, Value 1 is the speed of the right motor.
    */

    // Sets of points to be used in the motorMod method.
    double leftPoints[] = {1, 1, 1, 0.5, -1, -1, -1, 0.5};
    int leftSize = sizeof(leftPoints)/sizeof(leftPoints[0]);
    double rightPonts[] = {-1, 0.5, 1, 1, 1, 0.5, -1, -1};
    int rightSize = sizeof(rightPonts)/sizeof(rightPonts[0]);

    // Finds the distance of the joystick from center and multiplies it by the constant speed modifier.
    int moveVector = sqrt(joyY*joyY + joyX*joyX) * _speedMod;

    // Does the modifier thing for each motor and returns their values
    std::vector<int> speeds {int(moveVector*motorMod(joyX, joyY, leftPoints, leftSize)), int(moveVector*motorMod(joyX, joyY, rightPonts, rightSize))};
    return speeds;
}

// What those methods would be for a *.ino file
// The point-array and its size would be calculated in the loop() method and be input as parameters.
int motorSpeed(int joyY, int joyX, double points[], int size)
{
    // Finds the distance of the joystick from center and multiplies it by the constant speed modifier.
    int moveVector = sqrt(joyY*joyY + joyX*joyX) * _speedMod;

    // Does the modifier thing for each motor and returns their values
    return (int) (moveVector*motorMod(joyX, joyY, points, size));
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
        <<"\n";
    }
    std::cout << "\n";
    return 0;
}