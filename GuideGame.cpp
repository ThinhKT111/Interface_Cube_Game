#include <SFML/Graphics.hpp>
#include <string>
#include "./Object/Button.hpp"

int GuideGame() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rubik Game Guide");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Error loading font
    }

    // Title
    sf::Text title("How to Play Rubik's Cube", font, 36);
    title.setFillColor(sf::Color::White);
    title.setPosition(200, 20);

    // Instructions text
    std::string instructions = 
        "Game Rules:\n\n"
        "You can press the keys U, L, F, R, B, D to select your desired\n"
        "rotation method. Each key corresponds to rotating a Rubik's Cube\n"
        "face clockwise:\n"
        " - U: Yellow face\n"
        " - L: Blue face\n"
        " - F: Red face\n"
        " - R: Green face\n"
        " - B: Orange face\n"
        " - D: White face\n\n"
        "To rotate a face counterclockwise, simply hold the Shift key while\n"
        "pressing the corresponding key.\n\n"
        "To adjust the Rubik's Cube view, use the arrow keys (Left, Right,\n"
        "Up, Down) to change the perspective.\n";

    sf::Text instructionText(instructions, font, 20);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(50, 100);

    Button ExitButton("Exit", font, 20, {350, 550}, {100, 40}, sf::Color(100, 100, 250));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (ExitButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    std::cout << "Exit button clicked\n";
                    window.close();
                }
            }
        }

        // Clear and draw
        window.clear(sf::Color::Black);
        window.draw(title);
        window.draw(instructionText);
        ExitButton.draw(window);
        window.display();
    }

    return 0;
}
