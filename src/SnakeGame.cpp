#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
using namespace std;

Color mintcream = {165, 255, 251, 100};
Color lavender = {106, 121, 247, 255};
Color pink = {238,130,238,255};

//snake and food size and position will be on the basis of cells, imaginary.
int cellSize = 20;

//no. of cell
int cellCount = 50;

double lastUpdateTime = 0;



bool ElementInDeque(Vector2 element, deque<Vector2> deque)

{
    for(unsigned int i = 0; i<deque.size(); i++)
    {
    if(Vector2Equals(deque[i], element))
    {
        return true;
    }
    
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    
    return false;
}

class Snake
{
    //snake is going to be a deque as we will remove the last cell and place it in the front
    public:
    //initial position of snake
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 direction = {1, 0};

    bool addSegment = true;
        
    void Draw()
    {
        for(unsigned int i = 0; i<body.size(); i++)
        {
            //making body of the snake
            float x = body[i].x;
            float y = body[i].y;
            //placing the snake on the board and making its shape and size
            Rectangle segment = Rectangle{x * cellSize, y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, lavender);
        }
    }
    
    void Update()
    {   
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true)
        {
            
            addSegment = false;
        }
        else
        {
        //to make it move
        body.pop_back();
        
        }
    }
    
    void Reset()
    {
    body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    direction = {1, 0};
    }
};



class Food
{
    public:
    Vector2 position;
    //starting position of food 
    
    Food(deque<Vector2> snakeBody)
    {
        position = GenerateRandomPos(snakeBody);
    }
    //shape and size of food
    void Draw()
    {   //(x coodinate, y coodinate, dimensions, color)
        DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, pink);
    }
    
    //need to make destructor
    
    Vector2 GenerateRandomCell()
    {
         float x = GetRandomValue(0, cellCount - 1);
        
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
        
    }
    
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        
        Vector2 position = GenerateRandomCell();
        
        while(ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        
        return position;
    }
};

class Game
{
    public:
    Snake snake = Snake();
    Food food = Food(snake.body);

    
    bool running = true;
    
    void Draw()
    {
        food.Draw();
        snake.Draw();
    }
    
    void Update()
    {
        if(running)
        {
        snake.Update();
        CheckCollisionWithEdges();
        CheckCollisionWithFood();
        CheckCollisionWithTail();
        }
    }
    
    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
        }
    }
    
    void CheckCollisionWithEdges()
    {
        if(snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        
        if(snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
            
        }
    }
    
    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        
    }
    
    void CheckCollisionWithTail()
    {
        
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }
};


int main()
{   //window size or board size
    InitWindow(1000, 1000, "Snake game");
    //speed of everything on board
    SetTargetFPS(60);
    
    Game game = Game();
    
    //JAB TAK USER CROSS YA ESC NA DABA DE TAN TAK
    while(WindowShouldClose() == false)
    {
        //to begin making anything on the screen
        BeginDrawing();
        
        //to make the snake go slow
        if(eventTriggered(0.2))
        {
            game.Update();
        }
                                  //if direction is down this means
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
            game.running = true;
            
        }
        
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
    {
        game.snake.direction = {0, 1};
        game.running = true;
    }
    
    if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
    {
        game.snake.direction = {-1, 0};
        
        game.running = true;
    }
    
    if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x !=-1)
    {
        game.snake.direction = {1, 0};
        game.running = true;
    }
        ClearBackground(mintcream);
        
        game.Draw();
        EndDrawing();
    }
    
    
    
    CloseWindow();
    return 0;
}