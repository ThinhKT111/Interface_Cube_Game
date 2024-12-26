#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <unistd.h>

#include "LoginScreen.h"

#include ".././Admin/LoggedinScreen.h"
#include ".././User/LoggedinScreenUser.h"
#include "../StartScreen.h"


using namespace std;

int LoginScreen(sf::RenderWindow &window)
{
    std::string email = "";
    std::string password = "";
    int checkInput = 0;
    
    sf::Text emailLabel, passwordLabel, title, alert;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    title.setFont(font);
    title.setString("Login");
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::Black);
    title.setPosition(50, 50);

    emailLabel.setFont(font);
    emailLabel.setString("Email:");
    emailLabel.setCharacterSize(20);
    emailLabel.setFillColor(sf::Color::Black);
    emailLabel.setPosition(50, 120);

    passwordLabel.setFont(font);
    passwordLabel.setString("Password:");
    passwordLabel.setCharacterSize(20);
    passwordLabel.setFillColor(sf::Color::Black);
    passwordLabel.setPosition(50, 190);

    alert.setFont(font);
    alert.setString("Error: Wrong email or password");
    alert.setCharacterSize(20);
    alert.setFillColor(sf::Color::Red);
    alert.setPosition(475, 475);

    Button submitButton("Submit", font, 20, {549.5, 525}, {150, 50}, sf::Color(100, 100, 250));
    
    Button backButton("Back", font, 20, {549.5, 600}, {150, 50}, sf::Color(100, 100, 250));

    TextBox emailBox(190, 120, 600, 40, "arial.ttf");
    TextBox passwordBox(190, 190, 600, 40, "arial.ttf");
    // Mặc định TextBox1 có focus

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            emailBox.handleEvent(event, window);
            passwordBox.handleEvent(event, window);
            // Khi nhấn phím Enter, lưu giá trị TextBox vào string
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                email = emailBox.getText();
                password = passwordBox.getText();

                // Hiển thị giá trị ra console (hoặc sử dụng giá trị này ở nơi khác)
                std::cout << "TextBox 1: " << email << std::endl;
                std::cout << "TextBox 2: " << password << std::endl;
            }

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (submitButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    if (email == "admin" && password == "admin")
                    {
                        LoggedinScreenAdmin(window);
                        std::cout << "Login button clicked with Admin\n";
                    }
                    else if (email == "user" && password == "user")
                    {
                        LoggedinScreenUser(window);
                        std::cout << "Login button clicked with User\n";
                    }
                    else
                    {
                        checkInput++;
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
        window.draw(emailLabel);
        window.draw(passwordLabel);
        if (checkInput > 0)
        {
            window.draw(alert);
        }
        submitButton.draw(window);
        backButton.draw(window);
        emailBox.draw(window);
        passwordBox.draw(window);

        window.display();
    }

    return 0;
}