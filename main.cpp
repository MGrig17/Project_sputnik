#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <cmath>
#include <math.h>

using namespace sf;

int main()
{
    float angle0 = 0;
    float x0 = 0, y0 = 0, x1, y1;
    int k = 1, z = 0;
    float pi = 3.14159265358979323846f;
    int shirina = 120, visota = 50;

    RenderWindow window(VideoMode(1000, 1000), "Helllo!");

    RectangleShape rectangle(sf::Vector2f(shirina, visota));

    rectangle.setFillColor(sf::Color(100, 250, 50));
    float coord1 = window.getSize().x / 2.f;
    float coord2 = window.getSize().y / 2.f;
    rectangle.setPosition(coord1, coord2);
    rectangle.setOrigin(shirina / 2, visota / 2);

    float rotation = rectangle.getRotation();
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        // ѕолучим положение курсора мыши

        if (sf::Mouse::isButtonPressed(Mouse::Left)) // ≈сли зажата лева€ кнопка мыши
        { // тогда новый угол будем отсчитывать  в зависимости от старого
            if (z == 0)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                x1 = mousePos.x - coord1;
                y1 = mousePos.y - coord2;
                angle0 = atan2(y1, x1) * 180 / pi;
                std::cout << "ZAZATA" << std::endl;
                std::cout << rectangle.getRotation() << std::endl;
                //std::cout << x1 << " " << y1 << std::endl;
                z = 1;
            }
            k = 0;
            Vector2i mousePos = Mouse::getPosition(window);
            Vector2f rectanglePos = rectangle.getPosition();
            float dx = -(x1 - (mousePos.x - coord1) - x0);
            float dy = -(y1 - (mousePos.y - coord2) - y0);
            float angle = atan2(dy, dx) * 180 / pi; //т.е. получаем из арктангенса угол в радианах
            //std::cout << dx << " " << dy << std::endl;
            rectangle.setRotation(angle);
            std::cout << rectangle.getRotation() << std::endl;
        }
        else // момент отжати€ мыши
        {
            if (k == 0) {
                Vector2i mousePos = Mouse::getPosition(window);
                x0 = mousePos.x - coord1;
                y0 = mousePos.y - coord2;
                std::cout << "OTZATA" << std::endl;
                std::cout << rectangle.getRotation() << std::endl;
                //std::cout << x0 << " " << y0 << std::endl;
                // x0, y0 - это координаты, определ€ющие угол между пр€моугольником и осью в момент отжати€ мышки
                k = 1;
            }
            z = 0;
        }
        //a.setPosition(a.getPosition() + sf::Vector2f(k, k));
        //rectangle.setPosition(a, b);
        window.clear();
        window.draw(rectangle);
        window.display();

    }
    return 0;
}