#include "Button.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

// Constructor
Button::Button(const std::string& label, const sf::Font& font, unsigned int fontSize, 
               sf::Vector2f position, sf::Vector2f size, sf::Color color) {
    // Setup shape
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(color);

    // Setup text
    text.setString(label);
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x + (size.x - text.getLocalBounds().width) / 2,
                     position.y + (size.y - text.getLocalBounds().height) / 2);
}

// Set callback function
void Button::setOnClick(std::function<void()> callback) {
    onClick = callback;
}

bool Button::isClicked(sf::Vector2i mousePos) {
    sf::FloatRect buttonBounds = shape.getGlobalBounds();
    return buttonBounds.contains(static_cast<sf::Vector2f>(mousePos));
}

// Handle mouse click
void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            if (onClick) {
                onClick();
            }
        }
    }
}

// Draw button
void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}
