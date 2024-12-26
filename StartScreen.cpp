#include <SFML/Graphics.hpp>
#include "./Object/Button.hpp"
#include <iostream>

#include "./Stage2/LoginScreen.h"
#include "./Stage2/RegisterScreen.h"
#include "StartScreen.h"

using namespace std;

int StartScreen(sf::RenderWindow &window) {

    const int windowWidth = 1249;
    const int windowHeight = 829;
    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    // Load background image
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./Image/rubik.png")) {
        std::cerr << "Error loading background image\n";
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    float scale = std::min(windowWidth / (float)backgroundTexture.getSize().x, windowHeight / (float)backgroundTexture.getSize().y);
    backgroundSprite.setScale(scale, scale);
    backgroundSprite.setPosition(
        (windowWidth - backgroundTexture.getSize().x * scale) / 2.0f,
        (windowHeight - backgroundTexture.getSize().y * scale) / 2.0f
    );

    // Tạo nút Login
    Button loginButton("Login", font, 20, {549.5, 525}, {150, 50}, sf::Color(100, 100, 255));

    // Tạo nút Register
    Button registerButton("Register", font, 20, {549.5, 600}, {150, 50}, sf::Color(100, 100, 255));

    // Vòng lặp chính
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (loginButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    LoginScreen(window);
                    std::cout << "Login button clicked\n";
                }
                if (registerButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    RegisterScreen(window);
                    std::cout << "Register button clicked\n";
                }
            }
        }

        // Vẽ background trước
        window.clear();
        window.draw(backgroundSprite);

        // Vẽ nút bấm
        loginButton.draw(window);
        registerButton.draw(window);
        window.display();
    }

    return 0;
}
