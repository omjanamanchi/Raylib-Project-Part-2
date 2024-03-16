#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "Header.h"
using namespace std;

Vehicle::Vehicle(void) 
{

}

Vehicle::Vehicle(float x, float y, float maxSpeed)
{
    acc = Vector2{ 0.0f, 0.0f };
    vel = Vector2{ 0.0f, 0.0f };
    loc = Vector2{ x, y };
    radius = 3.0;
    max_speed = maxSpeed;
    max_force = 0.5f;
    new_loc = Vector2{ 0.0f, 0.0f };
}

void Vehicle::update()
{
    vel = Vector2Add(vel, acc);
    Vector2ClampValue(vel, 0.0f, max_speed);
    loc = Vector2Add(loc, vel);
    acc = Vector2{ 0.0f, 0.0f };
}


void Vehicle::applyForce(Vector2 force)
{
    acc = Vector2Add(force, acc);
}

void Vehicle::arrive(Vector2 target, float deltaTime) 
{
    const float scaleFactor = 0.16f, maxPlayerVelocity = 3.0f, slowingRadius = 115.0f, maxSpeed = 3.5f, maxForce = 4.5f, maxSteeringRate = PI / 10.5f, stopRadius = 1.0f, timeToWait = 6.0;
    float deltaTime1 = deltaTime;

    Vector2 desired = Vector2Subtract(target, loc);
    float distance = Vector2Length(desired);  // Calculate the distance

    if (distance > 0) 
    {
        desired = Vector2Normalize(desired);
        if (distance < stopRadius) 
        {
            
            desired = Vector2{ 0.0f, 0.0f };
        }

        else if (distance < slowingRadius) 
        {
            float m = map(distance, 0, slowingRadius, 0, max_speed);
            desired = Vector2Scale(desired, m);
        }


        else 
        {
            desired = Vector2Scale(desired, max_speed);
        }

        Vector2 steer = Vector2Subtract(desired, vel);
        Vector2ClampValue(steer, 0.0f, maxSteeringRate);
        steer = Vector2Scale(steer, deltaTime1);
        applyForce(steer);
        vel = Vector2Add(vel, Vector2Scale(steer, deltaTime1));
        Vector2ClampValue(vel, 0.0f, max_speed);
    }
}


float Vehicle::map(float value, float from_low, float from_high, float to_low, float to_high)
{
    return (value - from_low) / (from_high - from_low) * (to_high - to_low) + to_low;
}