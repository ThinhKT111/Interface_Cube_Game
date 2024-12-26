#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>

#include "LoggedinScreenUser.h"
#include "../StartScreen.h"
#include "JoinRoomScreenUser.h"
#include "CreateRoomScreenUser.h"
#include "../GuideGame.h"

using namespace std;

int LoggedinScreenUser(sf::RenderWindow &window)
{
    sf::Text nameLabel, total, winnum, losenum;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    nameLabel.setFont(font);
    nameLabel.setString("User");
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    total.setFont(font);
    total.setString("Total match: " + std::to_string(10));
    total.setCharacterSize(25);
    total.setFillColor(sf::Color::Black);
    total.setPosition(970, 100);
    total.setStyle(sf::Text::Bold);

    winnum.setFont(font);
    winnum.setString("Win: " + std::to_string(10));
    winnum.setCharacterSize(25);
    winnum.setFillColor(sf::Color::Black);
    winnum.setPosition(970, 135);
    winnum.setStyle(sf::Text::Bold);

    losenum.setFont(font);
    losenum.setString("Lose: " + std::to_string(10));
    losenum.setCharacterSize(25);
    losenum.setFillColor(sf::Color::Black);
    losenum.setPosition(970, 170);
    losenum.setStyle(sf::Text::Bold);

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

    Button joinRoomButton("Join Room", font, 30, {274.5, 375}, {250, 125}, sf::Color(100, 100, 250));
    
    Button createRoomButton("Create Room", font, 30, {724.5, 375}, {250, 125}, sf::Color(100, 100, 250));

    Button logoutButton("logout", font, 20, {574.5, 600}, {100, 50}, sf::Color(100, 100, 250));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle click on settingSprite
            if (event.type == sf::Event::MouseButtonPressed) {
                if (settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Call the function from GuideGame.cpp
                    GuideGame(); // Replace with the actual function name
                    std::cout << "Setting image clicked\n";
                }
            }

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (joinRoomButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    JoinRoomScreenUser(window);
                    std::cout << "Manage Room button clicked\n";
                }
                else if (createRoomButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    CreateRoomScreenUser(window);
                    std::cout << "Manage User button clicked\n";
                }
                else if (logoutButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    StartScreen(window);
                    std::cout << "Logout button clicked\n";
                }
            }
        }

        // Drawing
        window.clear(sf::Color(180, 255, 240)); // Background Color

        window.draw(settingSprite);
        window.draw(peopleSprite);
        window.draw(nameLabel);
        window.draw(total);
        window.draw(winnum);
        window.draw(losenum);
        joinRoomButton.draw(window);
        createRoomButton.draw(window);
        logoutButton.draw(window);

        window.display();
    }

    return 0;
}