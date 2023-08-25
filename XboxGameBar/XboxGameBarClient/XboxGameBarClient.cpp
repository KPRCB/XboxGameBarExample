#include <iostream>
#include "XboxGameBarServer.h"

int main()
{
	XboxGameBarServer draw; //initialise the object

    std::srand(std::time(nullptr));

    int radius = 100;

    const int xMin = radius;
    const int xMax = GetSystemMetrics(SM_CXSCREEN) - radius;
    const int yMin = radius;
    const int yMax = GetSystemMetrics(SM_CYSCREEN) - radius;

    int x = std::rand() % (xMax - xMin) + xMin;
    int y = std::rand() % (yMax - yMin) + yMin;

    const int speed = 10;

    int velocityX = (std::rand() % 2 == 0 ? 1 : -1) * speed;
    int velocityY = (std::rand() % 2 == 0 ? 1 : -1) * speed;

    for (;;) {
        x += velocityX;
        y += velocityY;

        if (x <= xMin || x >= xMax) {
            velocityX = -velocityX;
            x += velocityX;         
        }
        if (y <= yMin || y >= yMax) {
            velocityY = -velocityY; 
            y += velocityY; 
        }

        draw.beginFrame();
        draw.addText("Text", 960, 540, draw.getColorFromArgb(255, 255, 0, 0), 500);
        draw.addCircle(x, y, radius, draw.getColorFromArgb(255, 0, 255, 0));
        draw.endFrame();
    }
        

}

