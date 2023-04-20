#include <iostream>
#include "common.h"

const double _speedMod = 1;

//const double leftPoints[] = {1, 0.5, 0.5, 1, 0.5, -0.5, -1, -0.5, -0.5, -1, -0.5, 0.5};
//const double leftPoints[] = {1, 0.7, 0.7, 1, 0.5, -0.5, -1, -0.7, -0.7, -1, -0.5, 0.5};
//const double leftPoints[] = {1, 1, -1, -1};
const double leftPoints[] = {1, 0.5, 1, 0, -1, -0.5, -1, 0};
const int leftSize = sizeof(leftPoints)/sizeof(leftPoints[0]);
    
//const double rightPoints[] = {-1, -0.5, 0.5, 1, 0.5, 0.5, 1, 0.5, -0.5, -1, -0.5, -0.5};
//const double rightPoints[] = {-1, -0.5, 0.5, 1, 0.7, 0.7, 1, 0.5, -0.5, -1, -0.7, -0.7};
//const double rightPoints[] = {-1, 1, 1, -1};
const double rightPoints[] = {-1, 0, 1, 0.5, 1, 0, -1, -0.5};
const int rightSize = sizeof(rightPoints)/sizeof(rightPoints[0]);

double motorMod(int joyX, int joyY, const double points[], int numOfPoints)
{
    /*
        A joystick lies within a circle, that we know.
        We want to modulate the speed of the motors according to where the joystick is on the circle.
        This means we need the joystick's angle to act as an input.
        From this input, we will do some math to get an output number that can be used to change the motor's speed.
        We don't want to write all the math by hand, so we'll have the computer auto generate those formulas.
        For the most part, we care about the joystick having a certain value at specific points on a circle.
            Joystick forward = everything forward (bothMotors=1)
            joystick backward = everything backwards (bothMotors=-1)
            joystick left = spin counterclockwise (leftMotor=-1, rightMotor=1)
            joystick right = spin clockwise (leftMotor=1, rightMotor=-1)
        These points are evenly spaced on a circle, at 90, 180, 270, and 0/360 degrees respectively
        This program can take a number of desired outputs and place them evenly on a circle, starting at 0 degrees.
            Only looks at the outputs of a single motor, so like the joystick position example above,
            the points would be (0, 1) (pi/2, 1) (pi, -1) (3pi/2, -1) for the left motor.
            Right motor would be (0, -1) (pi/2, 1) (pi, 1) (3pi/2, -1)
            Everything is done in radian instead of degrees, because thats how C does trig math
        It will then generate formulas that contain each neighboring point
            A formula that connects (0, -1) and (pi/2 (90), 1), then connect (pi/2 (90), 1) and (pi (180), 1),
            then (pi (180), 1) and (3pi/2 (270), -1), then (3pi/2 (270), -1) and (2pi (360), -1)
            Above example looks at the example for the right motor. For the final formula, 360 degrees, or 2pi, is used to generate
        If the joystick is between two specific inputs (like 0 and pi/2 (90)), 
        it will use the formula that contains those points to find the desired motor's speed
    */

    // Define an array that will carry each point, as well as what number will be divisor when getting each section of a circle.
    // Four points would iterate through fourths of 2pi, so iterate by 2pi/4, ot pi/2
    double setOfPoints[numOfPoints][2][2];
    double angleDiv = (2*M_PI)/numOfPoints;

    // Generates a list of pairs of points. Each pair's X neighbors each other on a circle.
    // These pairs will then be used to generate the formulas.
    for (int i = 0; i < numOfPoints; i++)
    {
        setOfPoints[i][0][0] = i*angleDiv;
        setOfPoints[i][0][1] = points[i];
        setOfPoints[i][1][0] = (i+1)*angleDiv;
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

// What those methods would be for a *.ino file
// The point-array and its size would be calculated in the loop() method and be input as parameters.
int motorSpeed(int joyY, int joyX, const double points[], int size)
{
    // Finds the distance of the joystick from center and multiplies it by the constant speed modifier.
    int moveVector = sqrt(joyY*joyY + joyX*joyX) * _speedMod;

    // Does the modifier thing for each motor and returns their values
    return (int) (moveVector*motorMod(joyX, joyY, points, size));
}

int main(void)
{
    int iterator = 15;
    int amp = 100;
    std::vector<int> yVals = genJoyY(iterator, amp);
    std::vector<int> xVals = genJoyX(iterator, amp);
    std::cout << "y:x | lMotor:rMotor | joyAngle | lMotorMod:rMotorMod" << "\n";
    for (int i = 0; i < yVals.size(); i++)
    {
        int y = yVals.at(i);
        int x = xVals.at(i);
        std::cout << y << ":" << x << " | " 
        << motorSpeed(y, x, leftPoints, leftSize) << ":" << motorSpeed(y, x, rightPoints, rightSize) << " | " 
        << i*iterator <<  " | "
        <<"\n";
    }
    std::cout << "\n";
    return 0;
}