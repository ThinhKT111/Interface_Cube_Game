#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <fstream> // Include this header for file I/O

#include "LoginScreen.h"

#include ".././Admin/LoggedinScreen.h"
#include ".././User/LoggedinScreenUser.h"
#include "../StartScreen.h"

#include ".././Object/ServerCommune.hpp"


using namespace std;
using json = nlohmann::json;


int LoginScreen(sf::RenderWindow &window)
{
    //Giá trị theo dạng string của username và password
    std::string username = "";
    std::string password = "";
    int checkInput = 0;
    
    sf::Text usernameLabel, passwordLabel, title, alert;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    title.setFont(font);
    title.setString("Login");
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::Black);
    title.setPosition(50, 50);

    usernameLabel.setFont(font);
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(20);
    usernameLabel.setFillColor(sf::Color::Black);
    usernameLabel.setPosition(50, 120);

    passwordLabel.setFont(font);
    passwordLabel.setString("Password:");
    passwordLabel.setCharacterSize(20);
    passwordLabel.setFillColor(sf::Color::Black);
    passwordLabel.setPosition(50, 190);

    alert.setFont(font);
    alert.setString("Error: Wrong username or password");
    alert.setCharacterSize(20);
    alert.setFillColor(sf::Color::Red);
    alert.setPosition(475, 475);

    Button submitButton("Submit", font, 20, {549.5, 525}, {150, 50}, sf::Color(100, 100, 250));
    
    Button backButton("Back", font, 20, {549.5, 600}, {150, 50}, sf::Color(100, 100, 250));

    TextBox usernameBox(190, 120, 600, 40, "arial.ttf");
    TextBox passwordBox(190, 190, 600, 40, "arial.ttf");
    // Nhập username và password

    

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            usernameBox.handleEvent(event, window);
            passwordBox.handleEvent(event, window);
            // Khi nhấn phím Enter, lưu giá trị TextBox vào string
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                username = usernameBox.getText();
                password = passwordBox.getText();

                // Hiển thị giá trị ra console tại đây cần thêm phần xử lý xem thông tin đã đúng hay chưa?
                std::cout << "TextBox 1: " << username << std::endl;
                std::cout << "TextBox 2: " << password << std::endl;
            }

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (submitButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    // Create JSON payload
                    json payload;
                    payload["data"]["password"] = password;
                    payload["data"]["username"] = username;
                    payload["type"] = "SIGN_IN";

                    // Convert JSON payload to string
                    std::string pushData = payload.dump(4);
                    std::cout << pushData << std::endl;
                    
                    std::string response = sendData(pushData);

                    // Parse the JSON response
                    json jsonResponse = json::parse(response);

                    // Check the status and user_type fields
                    if (jsonResponse["status"] == "success") {
                        std::cout << "Success: " << jsonResponse["data"]["message"] << std::endl;
                        if (jsonResponse["data"]["user_type"] == "ADMIN") {
                            username = jsonResponse["data"]["username"];
                            std::cout << "User type: " << username << std::endl;
                            LoggedinScreenAdmin(window);
                        }
                        else {
                            username = jsonResponse["data"]["username"];
                            std::cout << "User type: " << username << std::endl;
                            LoggedinScreenUser(window, username);
                        }
                    }
                    else {
                        std::cout << "Fail: " << jsonResponse["data"]["message"] << std::endl;
                        checkInput = 1;
                    }

                }
                if (backButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    StartScreen(window);
                }
            }
        }

        // Drawing
        window.clear(sf::Color(180, 255, 240)); // Background Color

        window.draw(title);
        window.draw(usernameLabel);
        window.draw(passwordLabel);
        if (checkInput > 0)
        {
            window.draw(alert);
        }
        submitButton.draw(window);
        backButton.draw(window);
        usernameBox.draw(window);
        passwordBox.draw(window);

        window.display();
    }

    return 0;
}