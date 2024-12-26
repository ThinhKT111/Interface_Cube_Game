#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick; // Hàm callback

public:
    // Constructor
    Button(const std::string& label, const sf::Font& font, unsigned int fontSize, 
           sf::Vector2f position, sf::Vector2f size, sf::Color color);

    // Set callback function
    void setOnClick(std::function<void()> callback);

    bool isClicked(sf::Vector2i mousePos);

    // Xử lý sự kiện click chuột
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    // Vẽ nút
    void draw(sf::RenderWindow& window) const;
};

#endif
