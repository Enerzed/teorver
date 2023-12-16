#include "Math.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1440, 900), L"Создатель - Никита Ошурков", Style::Close);
    View view = window.getDefaultView();
    window.setView(view);
    bool moving = false;
    Vector2f oldPos;
    float zoom = 1;
    float azoom = 1;

    Math math;
    math.printGroupX();
    math.printGroupY();
    //math.printGroupVarX();
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == 0)
                {
                    moving = true;
                    oldPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            if (event.type == Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == 0)
                {
                    moving = false;
                }
            }
            if (event.type = Event::MouseMoved)
            {
                if (!moving)
                    continue;
                Vector2f newPos = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y));
                Vector2f deltaPos = oldPos - newPos;
                deltaPos.x *= azoom;
                deltaPos.y *= azoom;
                view.setCenter(view.getCenter() + deltaPos);
                window.setView(view);
                oldPos = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
            if (event.type = Event::MouseWheelScrolled)
            {
                if (moving)
                    continue;
                if (event.mouseWheelScroll.delta < 0)
                    zoom = std::min(2.f, zoom + .1f);
                else if (event.mouseWheelScroll.delta > 0)
                    zoom = std::max(.5f, zoom - .1f);
                azoom *= zoom;
                view.setSize(window.getDefaultView().getSize());
                view.zoom(zoom);
                window.setView(view);
            }
        }

        window.clear(Color::White);
        math.draw(window);
        window.display();
    }

    return 0;
}