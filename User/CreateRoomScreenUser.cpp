#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>

#include "CreateRoomScreenUser.h"
#include "../StartScreen.h"
#include "LoggedinScreenUser.h"
#include "LobbyScreenUser.h"
#include "../GuideGame.h"

using namespace std;



int CreateRoomScreenUser(sf::RenderWindow &window)
{
    std::string roomid = "";
    std::string limitnum = "";
    
    sf::Text nameLabel, tittle1, tittle2, nameWindow;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    nameLabel.setFont(font);
    nameLabel.setString("User");
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    nameWindow.setFont(font);
    nameWindow.setString("CREATE ROOM");
    nameWindow.setCharacterSize(30);
    nameWindow.setFillColor(sf::Color::Black);
    nameWindow.setPosition(90, 25);
    nameWindow.setStyle(sf::Text::Bold);

    tittle1.setFont(font);
    tittle1.setString("Enter room ID:");
    tittle1.setCharacterSize(20);
    tittle1.setFillColor(sf::Color::Black);
    tittle1.setPosition(450, 100);
    tittle1.setStyle(sf::Text::Bold);

    tittle2.setFont(font);
    tittle2.setString("Enter player limit:");
    tittle2.setCharacterSize(20);
    tittle2.setFillColor(sf::Color::Black);
    tittle2.setPosition(450, 200);
    tittle2.setStyle(sf::Text::Italic);


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

    Button BackButton("Back", font, 20, {574.5, 760}, {100, 40}, sf::Color(100, 100, 250));

    Button CreateButton("Create!", font, 20, {574.5, 700}, {100, 40}, sf::Color(100, 100, 250));


    TextBox roomidBox(470.5, 150, 200, 40, "arial.ttf");

    TextBox limitBox(470.5, 250, 200, 40, "arial.ttf");

    

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            roomidBox.handleEvent(event, window);
            limitBox.handleEvent(event, window);

            // Khi nhấn phím Enter, lưu giá trị TextBox vào string
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                roomid = roomidBox.getText();
                limitnum = limitBox.getText();
                

                // Hiển thị giá trị ra console (hoặc sử dụng giá trị này ở nơi khác)
                std::cout << "TextBox 1: " << roomid << std::endl;
                std::cout << "TextBox 2: " << limitnum << std::endl;
            }

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
                if (BackButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    LoggedinScreenUser(window);
                    std::cout << "Back button clicked\n";
                }
                else if (CreateButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    LobbyScreenUser(window, roomid);
                    std::cout << "Create button clicked\n";
                }
            }

            
        }
        // Drawing
        window.clear(sf::Color(180, 255, 240)); // Background Color

        window.draw(settingSprite);
        window.draw(peopleSprite);
        window.draw(nameLabel);
        window.draw(nameWindow);

        BackButton.draw(window);
        CreateButton.draw(window);
        roomidBox.draw(window);
        limitBox.draw(window);
        window.draw(tittle1);
        window.draw(tittle2);


        

        window.display();
    }

    return 0;
    
}
