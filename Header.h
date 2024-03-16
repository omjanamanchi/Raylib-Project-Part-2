#pragma 
#include "raylib.h"
#include "raymath.h"
class Vehicle
{
    public:
        Vector2 loc, vel, acc, new_loc;
        float radius, max_force, max_speed;
        Vector2 steer;
        Vehicle();
        Vehicle(float x, float y, float maxSpeed);
        void update();
        void applyForce(Vector2 force);
        void arrive(Vector2 target, float deltaTime);
        float map(float value, float from_low, float from_high, float to_low, float to_high);
};