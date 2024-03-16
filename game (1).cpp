#include "raylib.h"
#include "raymath.h"
#include <thread>
#include <iostream>
#include "Header.h"
#include <string>

using namespace std;

typedef struct Food {
    public:
        float x_position;
        float y_position;
        Color color;
        Food() {};
        Food(float x, float y, Color colorS) 
        {
            x_position = x;
            y_position = y;
            color = colorS;
        }
} Food;


typedef struct Star 
{
    public:
        bool valid1, hasRadius;
        float visionRange, x_position, y_position, x_target, y_target;
        float maxSpeed = 3.0f;
        Vehicle vehicle;
        int foodEaten = 0;
        float star_rotation = 0;
};

typedef struct Pentagon 
{
    bool valid2, hasBox;
    float visionRange = 5.0f;
    float x_position, y_position, maxSpeed, x_target, y_target;
    Vehicle vehicle;
    int foodEaten = 0;
    float pentagon_rotation = 0;
};

float randomCircleInterval = 30.0f;
const int ROUND_DURATION = 30;

int main(void) 
{
    InitWindow(1775, 1000, "Raylib Project - Part 2");
    SetTargetFPS(60);
    Texture2D star_texture = LoadTexture("C:\\Users\\jshri\\Downloads\\star.png");
    star_texture.width /= 5; star_texture.height /= 5;
    Texture2D pentagon_texture = LoadTexture("C:\\Users\\jshri\\Downloads\\pentagon.png");
    pentagon_texture.width /= 5; pentagon_texture.height /= 5;
    Star stars[5];
    Pentagon pentagons[5];
    clock_t currentTime = clock();
    clock_t roundTimer = clock();
    Color colors[5] = { ORANGE, BLUE, GREEN, RED, VIOLET };
    Food foods[50];

    for (int i = 0; i < 50; i++) 
    {
        foods[i] = Food(rand() % 1775, rand() % 1000, colors[rand() % 5]);
    }

    for (int i = 0; i < 5; i++) 
    {
        stars[i].vehicle = Vehicle(rand() % 1775, rand() % 1000, stars->maxSpeed);
        stars[i].vehicle.max_speed = 3.0f;
        stars[i].vehicle.max_force = 0.1f;
        stars[i].vehicle.radius = 3.0f;
        stars[i].visionRange = 5.0f;
        stars[i].x_target = rand() % 1775;
        stars[i].y_target = rand() % 1000;
        stars[i].valid1 = true;
        stars[i].hasRadius = false;

        pentagons[i].vehicle = Vehicle(rand() % 1775, rand() % 1000, pentagons->maxSpeed);
        pentagons[i].vehicle.max_speed = 1.5f;
        pentagons[i].vehicle.max_force = 0.1f;
        pentagons[i].vehicle.radius = 3.0f;
        pentagons[i].visionRange = 100.0f;
        pentagons[i].x_target = rand() % 1775;
        pentagons[i].y_target = rand() % 1000;
        pentagons[i].valid2 = true;
        pentagons[i].hasBox = false;
    }

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < 50; i++) 
        {
            DrawCircle(foods[i].x_position, foods[i].y_position, 5.0f, foods[i].color);
        }

        for (int i = 0; i < 5; i++) 
        {
            int numFood = sizeof(foods)/sizeof(Food);
            bool foundFood = false;
            for (int j = 0; j < numFood; j++) 
            {
                float dx = foods[j].x_position - pentagons[i].vehicle.loc.x;
                float dy = foods[j].y_position - pentagons[i].vehicle.loc.y;
                float distance = sqrt(dx * dx + dy * dy);
                if (distance < pentagons[i].visionRange) 
                {
                    pentagons[i].x_target = foods[j].x_position;
                    pentagons[i].y_target = foods[j].y_position;
                    foundFood = true;
                    break;
                }
            }

            if (stars[i].valid1)
            {
                int x1 = stars[i].vehicle.loc.x - star_texture.width / 2;
                int y1 = stars[i].vehicle.loc.y - star_texture.height / 2;

                Rectangle shipRectangle = { 0,0, star_texture.width, star_texture.height };
                Rectangle newRectangleOne = { stars[i].vehicle.loc.x,stars[i].vehicle.loc.y, star_texture.width, star_texture.height };
                float angleOne = atan2f(stars[i].y_target - stars[i].vehicle.loc.y, stars[i].x_target - stars[i].vehicle.loc.x);
                float angleDiffOne = angleOne - stars[i].star_rotation;
                stars[i].star_rotation += angleDiffOne * 0.05f;
                Vector2 shipCenter = { star_texture.width/2, star_texture.height/2 };
                DrawTexturePro(star_texture, shipRectangle, newRectangleOne, shipCenter, 90 + (atan2(stars[i].vehicle.vel.y, stars[i].vehicle.vel.x) * 180 / PI), WHITE);

                if (stars[i].hasRadius)
                {
                    DrawCircleLines(stars[i].vehicle.loc.x, stars[i].vehicle.loc.y, stars[i].visionRange, WHITE);
                }

                float speed_ratio = stars[i].vehicle.max_speed / 10.0f;
                int red_comp = static_cast<int>(255 * (1.0f - speed_ratio));
                int green_comp = static_cast<int>(255 * speed_ratio);
                Color highlight = { red_comp, green_comp, 0, 128 };
                int outline_thick = 2;
                Color outline_color = { 0, 0, 0, 200 };
                DrawRectangleLinesEx({ (float)x1 - outline_thick, (float)y1 - outline_thick, (float)star_texture.width + 2 * outline_thick, (float)star_texture.height + 2 * outline_thick }, outline_thick, outline_color);
                DrawRectangle(x1, y1, star_texture.width, star_texture.height, highlight);
                DrawText(std::to_string(stars[i].foodEaten).c_str(), stars[i].vehicle.loc.x, stars[i].vehicle.loc.y, 20, WHITE);

                float dx = stars[i].x_target - stars[i].vehicle.loc.x;
                float dy = stars[i].y_target - stars[i].vehicle.loc.y;
                float distance = sqrt(dx * dx + dy * dy);
                if (distance < stars[i].visionRange) 
                {
                    stars[i].x_target = rand() % 1775;
                    stars[i].y_target = rand() % 1000;
                }

                stars[i].vehicle.arrive(Vector2{ stars[i].x_target, stars[i].y_target }, GetFrameTime());
                stars[i].vehicle.update();

                //Eat Food
                for (int j = 0; j < numFood; j++) 
                {
                    float dx = foods[j].x_position - stars[i].vehicle.loc.x;
                    float dy = foods[j].y_position - stars[i].vehicle.loc.y;
                    float distance = sqrt(dx * dx + dy * dy);
                    if (distance < stars[i].visionRange) 
                    {
                        foods[j].x_position = 10000;
                        foods[j].y_position = 10000;
                        foods[j].color = colors[rand() % 5];
                        stars[i].foodEaten++;
                    }
                }

            }

            if (pentagons[i].valid2)
            {
                int x2 = pentagons[i].vehicle.loc.x - pentagon_texture.width / 2;
                int y2 = pentagons[i].vehicle.loc.y - pentagon_texture.height / 2;

                Rectangle shipRectangle = { 0,0, pentagon_texture.width, pentagon_texture.height };
                Rectangle newRectangleOne = { pentagons[i].vehicle.loc.x,pentagons[i].vehicle.loc.y, pentagon_texture.width, pentagon_texture.height };
                float angleOne = atan2f(pentagons[i].y_target - pentagons[i].vehicle.loc.y, pentagons[i].x_target - pentagons[i].vehicle.loc.x);
                float angleDiffOne = angleOne - pentagons[i].pentagon_rotation;
                pentagons[i].pentagon_rotation += angleDiffOne * 0.05f;
                Vector2 shipCenter = { pentagon_texture.width / 2, pentagon_texture.height / 2 };
                DrawTexturePro(pentagon_texture, shipRectangle, newRectangleOne, shipCenter, 90 + (atan2(pentagons[i].vehicle.vel.y, pentagons[i].vehicle.vel.x) * 180 / PI), WHITE);
                DrawCircleLines(pentagons[i].vehicle.loc.x, pentagons[i].vehicle.loc.y, pentagons[i].visionRange, BLACK);
                DrawText(std::to_string(pentagons[i].foodEaten).c_str(), pentagons[i].vehicle.loc.x, pentagons[i].vehicle.loc.y, 20, WHITE);

                if (pentagons[i].hasBox)
                {
                    float speed_ratio = stars[i].vehicle.max_speed / 10.0f;
                    int red_comp = static_cast<int>(255 * (1.0f - speed_ratio));
                    int green_comp = static_cast<int>(255 * speed_ratio);
                    Color highlight = { red_comp, green_comp, 0, 128 };
                    int outline_thick = 2;
                    Color outline_color = { 0, 0, 0, 200 };
                    DrawRectangleLinesEx({ (float)x2 - outline_thick, (float)y2 - outline_thick, (float)pentagon_texture.width + 2 * outline_thick, (float)pentagon_texture.height + 2 * outline_thick }, outline_thick, outline_color);
                    DrawRectangle(x2, y2, pentagon_texture.width, pentagon_texture.height, highlight);
                }

                float dx = pentagons[i].x_target - pentagons[i].vehicle.loc.x;
                float dy = pentagons[i].y_target - pentagons[i].vehicle.loc.y;
                float distance = sqrt(dx * dx + dy * dy);
                if (!foundFood && distance < 5.0f) {
                    pentagons[i].x_target = rand() % 1775;
                    pentagons[i].y_target = rand() % 1000;
                }

                pentagons[i].vehicle.arrive(Vector2{ pentagons[i].x_target, pentagons[i].y_target }, GetFrameTime());
                pentagons[i].vehicle.update();

                //Delete Food
                for (int j = 0; j < numFood; j++) 
                {
                    float dx = foods[j].x_position - pentagons[i].vehicle.loc.x;
                    float dy = foods[j].y_position - pentagons[i].vehicle.loc.y;
                    float distance = sqrt(dx * dx + dy * dy);
                    if (distance < 5.0f) 
                    {
                        foods[j].x_position = 10000;
                        foods[j].y_position = 10000;
                        foods[j].color = colors[rand() % 5];
                        pentagons[i].foodEaten++;
                    }
                }

            }

            //Count-down Timer
            std::string textToDisplay = "TIME LEFT: " + std::to_string(ROUND_DURATION - ((clock() - roundTimer) / CLOCKS_PER_SEC));
            DrawText(textToDisplay.c_str(), 1775 - 210, 50, 25, WHITE);

            if (clock() - roundTimer > ROUND_DURATION * CLOCKS_PER_SEC) 
            {
                roundTimer = clock();
                for (int j = 0; j < 50; j++) 
                {
                    foods[j] = Food(rand() % 1775, rand() % 1000, colors[rand() % 5]);
                }

                for (int j = 0; j < 5; j++) 
                {
                    if (stars[j].valid1)
                    {
                        for (int k = 0; k < 5; k++) 
                        {
                            if (pentagons[k].valid2)
                            {
                                float dx = stars[j].vehicle.loc.x - pentagons[k].vehicle.loc.x;
                                float dy = stars[j].vehicle.loc.y - pentagons[k].vehicle.loc.y;
                                float distance = sqrt(dx * dx + dy * dy);
                                if (distance < 200.0f) 
                                {
                                    stars[j].visionRange = ((stars[j].visionRange) * 1.1) + 35;
                                    pentagons[k].vehicle.max_speed *= 1.5;
                                    stars[k].hasRadius = true;
                                    pentagons[j].hasBox = true;
                                }
                            }

                        }

                    }

                }

                //Evolution Metrics 
                int star_max_food_first = -1, star_max_food_second = -1, star_max_food_third = -1, star_min_food = INT_MAX, star_max_idx = -1, star_second_idx = -1, star_third_idx = -1, star_min_idx = -1;
                int pentagon_max_food_first = -1, pentagon_max_food_second = -1, pentagon_max_food_third = -1, pentagon_min_food = INT_MAX, pentagon_max_idx = -1, pentagon_second_idx = -1, pentagon_third_idx = -1, pentagon_min_idx = -1;
                for (int i = 0; i < 5; i++) //Stars
                {
                    if (stars[i].valid1)
                    {
                        if (stars[i].foodEaten > star_max_food_first) 
                        {
                            star_max_food_third = star_max_food_second;
                            star_max_food_second = star_max_food_first;
                            star_max_food_first = stars[i].foodEaten;
                            star_third_idx = star_second_idx;
                            star_second_idx = star_max_idx;
                            star_max_idx = i;
                        }
                        if (stars[i].foodEaten < star_min_food) 
                        {
                            star_min_food = stars[i].foodEaten;
                            star_min_idx = i;
                        }

                    }

                    if (pentagons[i].valid2) //Pentagons
                    {
                        if (pentagons[i].foodEaten > pentagon_max_food_first) 
                        {
                            pentagon_max_food_third = 12;
                            pentagon_max_food_second = pentagon_max_food_first;
                            pentagon_max_food_first = pentagons[i].foodEaten;

                            pentagon_third_idx = pentagon_second_idx;
                            pentagon_second_idx = pentagon_max_idx;
                            pentagon_max_idx = i;
                        }
                        if (pentagons[i].foodEaten < pentagon_min_food) 
                        {
                            pentagon_min_food = pentagons[i].foodEaten;
                            pentagon_min_idx = i;
                        }

                    }
                }

                //Grow & Destroy Stars/Pentagons for Next Round
                if (star_max_idx != -1) stars[star_max_idx].vehicle.max_speed *= 1.5;
                if (star_second_idx != -1) stars[star_second_idx].vehicle.max_speed *= 1.35;
                if (star_third_idx != -1) stars[star_third_idx].vehicle.max_speed *= 1.1;
                if (pentagon_max_idx != -1) pentagons[pentagon_max_idx].visionRange *= 1.5;
                if (pentagon_second_idx != -1) pentagons[pentagon_second_idx].visionRange *= 1.35;
                if (pentagon_third_idx != -1) pentagons[pentagon_third_idx].visionRange *= 1.1;
                if (star_min_idx != -1) stars[star_min_idx].valid1 = false;
                if (pentagon_min_idx != -1) pentagons[pentagon_min_idx].valid2 = false;
                for (int i = 0; i < 5; i++) 
                {
                    stars[i].foodEaten = 0;
                    pentagons[i].foodEaten = 0;
                }

            }

        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}