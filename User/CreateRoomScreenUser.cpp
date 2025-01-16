#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

#include "CreateRoomScreenUser.h"
#include "../StartScreen.h"
#include "LoggedinScreenUser.h"
#include "LobbyScreenUser.h"
#include "../GuideGame.h"

#include ".././Object/ServerCommune.hpp"

using namespace std;
using json = nlohmann::json;



int CreateRoomScreenUser(sf::RenderWindow &window, std::string username)
{
    std::string roomid = "";
    std::string limitnum = "";
    std::string limitspecnum = "";
    
    sf::Text nameLabel, tittle1, tittle2, tittle3, nameWindow;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    // Create JSON payload
    json payload;
    payload["type"] = "SIGN_IN";
    payload["data"]["username"] = username;

    // Convert JSON payload to string
    std::string pushData = payload.dump(4);
    std::cout << pushData << std::endl;

    std::string response = sendData(pushData);

    // Parse the JSON response
    json jsonResponse = json::parse(response);

    nameLabel.setFont(font);
    nameLabel.setString(to_string(jsonResponse["data"]["player"]["username"]));
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

    tittle3.setFont(font);
    tittle3.setString("Enter spectators limit:");
    tittle3.setCharacterSize(20);
    tittle3.setFillColor(sf::Color::Black);
    tittle3.setPosition(450, 300);
    tittle3.setStyle(sf::Text::Italic);


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

    TextBox limitSpecBox(470.5, 350, 200, 40, "arial.ttf");

    

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            roomidBox.handleEvent(event, window);
            limitBox.handleEvent(event, window);
            limitSpecBox.handleEvent(event, window);

            // Khi nhấn phím Enter, lưu giá trị TextBox vào string
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                roomid = roomidBox.getText();
                limitnum = limitBox.getText();
                limitspecnum = limitSpecBox.getText();
                

                // Hiển thị giá trị ra console (hoặc sử dụng giá trị này ở nơi khác)
                std::cout << "TextBox 1: " << roomid << std::endl;
                std::cout << "TextBox 2: " << limitnum << std::endl;
                std::cout << "TextBox 3: " << limitspecnum << std::endl;
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
                    LoggedinScreenUser(window, username);
                    std::cout << "Back button clicked\n";
                }
                else if (CreateButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    // Retrieve values from text boxes
                    int maxPlayers = std::stoi(limitBox.getText());
                    int maxSpectators = std::stoi(limitSpecBox.getText());

                    // Create JSON payload
                    json payload;
                    payload["type"] = "CREATE_ROOM";
                    payload["data"]["max_players"] = maxPlayers;
                    payload["data"]["max_spectators"] = maxSpectators;

                    // Convert JSON payload to string
                    std::string pushData = payload.dump(4); // Pretty print with 4 spaces indentation
                    std::cout << pushData << std::endl;

                    // Send data to server
                    std::string response = sendData(pushData);

                    // Parse the JSON response
                    json jsonResponse = json::parse(response);

                    // Check the status field
                    if (jsonResponse["status"] == "success") {
                        std::cout << "Room created successfully: " << jsonResponse["data"]["message"] << std::endl;
                    } else {
                        std::cerr << "Failed to create room: " << jsonResponse["data"]["message"] << std::endl;
                    }
                    std::cout << "Create button clicked\n";
                    LobbyScreenUser(window, roomid, username);
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
        limitSpecBox.draw(window);
        window.draw(tittle1);
        window.draw(tittle2);
        window.draw(tittle3);


        

        window.display();
    }

    return 0;
    
}
