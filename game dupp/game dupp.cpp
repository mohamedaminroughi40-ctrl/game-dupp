#include "MainMenu.h"

int main()
{
    RenderWindow window(VideoMode(1900, 900), "Our Game Menu");
    Clock clock;
    window.setFramerateLimit(60);

    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            menu.handleInput(window, event);
        }
        menu.update(deltaTime);

        window.clear();
        menu.draw(window);
        window.display();
    }
    return 0;
}