#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>

#include "LoggedinScreen.h"
#include "ManageUserScreen.h"
#include "ManageRoomScreen.h"
#include "../StartScreen.h"
#include "../GuideGame.h"

using namespace std;

int LoggedinScreenAdmin(sf::RenderWindow &window)
{
    sf::Text nameLabel;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    nameLabel.setFont(font);
    nameLabel.setString("Admin");
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    // Load images
    sf::Texture settingTexture;
    if (!settingTexture.loadFromFile("./Image/setting.png")) {
        std::cerr << "Error loading setting image\n";
        return -1;
    }
    sf::Sprite settingSprite(settingTexture);
    settingSprite.setPosition(20, 20);

    sf::Texture peopleTexture;
    if (!peopleTexture.loadFromFile("./Image/people.png")) {
        std::cerr << "Error loading people image\n";
        return -1;
    }
    sf::Sprite peopleSprite(peopleTexture);
    peopleSprite.setPosition(1000, 20);

    Button manageRoomButton("Manage Room", font, 30, {274.5, 375}, {250, 125}, sf::Color(100, 100, 250));
    
    Button manageUserButton("Manage User", font, 30, {724.5, 375}, {250, 125}, sf::Color(100, 100, 250));

    Button logoutButton("logout", font, 20, {574.5, 600}, {100, 50}, sf::Color(100, 100, 250));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (manageRoomButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    std::cout << "Manage Room button clicked\n";
                    ManageRoomScreenAdmin(window);
                }
                else if (manageUserButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    ManageUserScreenAdmin(window);
                    std::cout << "Manage User button clicked\n";
                }
                else if (logoutButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    StartScreen(window);
                    std::cout << "Logout button clicked\n";
                }
            }

            // Handle click on settingSprite
            if (event.type == sf::Event::MouseButtonPressed) {
                if (settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Call the function from GuideGame.cpp
                    GuideGame(); // Replace with the actual function name
                    std::cout << "Setting image clicked\n";
                }
            }
        }

        // Drawing
        window.clear(sf::Color(180, 255, 240)); // Background Color

        window.draw(settingSprite);
        window.draw(peopleSprite);
        window.draw(nameLabel);
        manageRoomButton.draw(window);
        manageUserButton.draw(window);
        logoutButton.draw(window);

        window.display();
    }

    return 0;
}