#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>

#include "RegisterScreen.h"
#include "LoginScreen.h"
#include "../StartScreen.h"


using namespace std;

int RegisterScreen(sf::RenderWindow &window)
{
    std::string email = "";
    std::string password = "";
    std::string repassword = "";
    int checkInput = 0;
    
    sf::Text emailLabel, passwordLabel, title, repasswordLabel, alert1, alert2;
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

    repasswordLabel.setFont(font);
    repasswordLabel.setString("Confirm Password:");
    repasswordLabel.setCharacterSize(20);
    repasswordLabel.setFillColor(sf::Color::Black);
    repasswordLabel.setPosition(50, 260);

    alert1.setFont(font);
    alert1.setString("Error: Password mismatch");
    alert1.setCharacterSize(20);
    alert1.setFillColor(sf::Color::Red);
    alert1.setPosition(475, 475);

    alert2.setFont(font);
    alert2.setString("Error: Username Taken");
    alert2.setCharacterSize(20);
    alert2.setFillColor(sf::Color::Red);
    alert2.setPosition(475, 475);

    Button submitButton("Submit", font, 20, {549.5, 525}, {150, 50}, sf::Color(100, 100, 250));
    
    Button backButton("Back", font, 20, {549.5, 600}, {150, 50}, sf::Color(100, 100, 250));
    

    TextBox emailBox(190, 120, 600, 40, "arial.ttf");
    TextBox passwordBox(190, 190, 600, 40, "arial.ttf");
    TextBox repasswordBox(190, 260, 600, 40, "arial.ttf");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            emailBox.handleEvent(event, window);
            passwordBox.handleEvent(event, window);
            repasswordBox.handleEvent(event, window);

            // Khi nhấn phím Enter, lưu giá trị TextBox vào string
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                email = emailBox.getText();
                password = passwordBox.getText();
                repassword = repasswordBox.getText();

                // Hiển thị giá trị ra console (hoặc sử dụng giá trị này ở nơi khác)
                std::cout << "TextBox 1: " << email << std::endl;
                std::cout << "TextBox 2: " << password << std::endl;
                std::cout << "TextBox 3: " << repassword << std::endl;
            }

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (submitButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    if (email == "admin" && password == "admin")
                    {
                        LoginScreen(window);
                    }
                    else if (password != repassword)
                    {
                        checkInput = 1;
                    }
                    else
                    {
                        checkInput = 2;
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

        if (checkInput == 1)
        {
            window.draw(alert1);
        }
        else if (checkInput == 2)
        {
            window.draw(alert2);
        }

        window.draw(repasswordLabel);
        submitButton.draw(window);
        backButton.draw(window);

        emailBox.draw(window);
        passwordBox.draw(window);
        repasswordBox.draw(window);

        window.display();
    }

    return 0;
}