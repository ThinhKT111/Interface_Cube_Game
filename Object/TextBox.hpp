#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox {
public:
    TextBox(float x, float y, float width, float height, const std::string& fontPath);

    void handleEvent(sf::Event event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    std::string getText() const;

private:
    sf::RectangleShape box;
    sf::Font font;
    sf::Text text;
    std::string textString;
    bool isFocused;
};

#endif // TEXTBOX_H
