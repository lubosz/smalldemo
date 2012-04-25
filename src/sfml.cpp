#include <SFML/Window.hpp>

int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML Window");

    // Start main loop
    bool Running = true;
    while (Running)
    {
        App.display();
    }

    return EXIT_SUCCESS;
}
