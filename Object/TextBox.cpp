#include "TextBox.hpp"

TextBox::TextBox(float x, float y, float width, float height, const std::string& fontPath)
    : isFocused(false) {
    box.setSize(sf::Vector2f(width, height));
    box.setPosition(x, y);
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color::Black);

    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Failed to load font");
    }

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 5, y + 5);
}

void TextBox::handleEvent(sf::Event event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (box.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
            isFocused = true;
            box.setOutlineColor(sf::Color::Blue);
        } else {
            isFocused = false;
            box.setOutlineColor(sf::Color::Black);
        }
    }

    if (isFocused && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !textString.empty()) {
            textString.pop_back();
        } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
            textString += static_cast<char>(event.text.unicode);
        }
        text.setString(textString);
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

std::string TextBox::getText() const {
    return textString;
}
