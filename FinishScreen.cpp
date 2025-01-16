#include <SFML/Graphics.hpp>
#include "./Object/Button.hpp"
#include <iostream>

#include "./Stage2/LoginScreen.h"
#include "./Stage2/RegisterScreen.h"
#include "FinishScreen.h"
#include "./Admin/LoggedinScreen.h"
#include "./User/LoggedinScreenUser.h"

using namespace std;

const int windowWidth = 1249;
const int windowHeight = 829;

class ScrollableList {
private:
    sf::RectangleShape background;
    sf::RectangleShape scrollbar;
    std::vector<sf::Text> items;
    sf::Font font;
    float scrollOffset;
    const float itemHeight = 40.0f;
    const float scrollbarWidth = 15.0f;

public:
    ScrollableList(float x, float y, float width, float height, const std::vector<std::string>& data) {
        // Load font
        font.loadFromFile("arial.ttf");

        // Background
        background.setPosition(x, y);
        background.setSize({width, height});
        background.setFillColor(sf::Color(230, 255, 250));

        // Scrollbar
        scrollbar.setPosition(x + width - scrollbarWidth, y);
        scrollbar.setSize({scrollbarWidth, height});
        scrollbar.setFillColor(sf::Color(200, 200, 200));

        // Items
        scrollOffset = 0;
        for (size_t i = 0; i < data.size(); i++) {
            sf::Text text;
            text.setFont(font);
            text.setString(data[i]);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
            text.setPosition(x + 10, y + i * itemHeight);
            items.push_back(text);
        }
    }

    void handleScroll(float delta) {
        scrollOffset += delta;
        float maxOffset = std::max(0.0f, static_cast<float>(items.size()) * itemHeight - background.getSize().y);
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxOffset) scrollOffset = maxOffset;

        for (size_t i = 0; i < items.size(); i++) {
            items[i].move(0, -delta);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);

        for (const auto& item : items) {
            if (item.getPosition().y >= background.getPosition().y &&
                item.getPosition().y <= background.getPosition().y + background.getSize().y - itemHeight) {
                window.draw(item);
            }
        }

        window.draw(scrollbar);
    }
};

int FinishScreen(sf::RenderWindow &window, int status_user, std::string username) {

    sf::Text tittle1, tittle2 ;

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    // Tạo nút Login
    Button BackButton("BackToLoggedInScreen", font, 20, {549.5, 750}, {150, 50}, sf::Color(100, 100, 255));

    tittle1.setFont(font);
    tittle1.setString("Game Over");
    tittle1.setCharacterSize(40);
    tittle1.setFillColor(sf::Color::Black);
    tittle1.setPosition(600, 80);
    tittle1.setStyle(sf::Text::Bold);

    tittle2.setFont(font);
    tittle2.setString("Result");
    tittle2.setCharacterSize(30);
    tittle2.setFillColor(sf::Color::Black);
    tittle2.setPosition(600, 150);
    tittle2.setStyle(sf::Text::Bold);

    // Data for the list
    std::vector<std::string> data = {
        "1. Thinh      60:01:46",
        "2. Phong      61:01:31",
        "3. Phuong     Abandoned",
        "4. Player 4   Abandoned",
        "5. Player 5   45:00:00"
    };

    // Scrollable list
    ScrollableList list(224.5, 250, 800, 400, data);


    // Vòng lặp chính
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (BackButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    if (status_user == 1)
                    {
                        LoggedinScreenAdmin(window);
                    }
                    else if (status_user == 2)
                    {
                        LoggedinScreenUser(window, "username");
                    }
                    std::cout << "Back button clicked\n";
                }

                if (event.type == sf::Event::MouseWheelScrolled) {
                list.handleScroll(-event.mouseWheelScroll.delta * 20);
            }
            }
        }

        // Vẽ background trước
        window.clear(sf::Color(180, 255, 240)); // Background Color


        // Vẽ nút bấm
        BackButton.draw(window);
        window.draw(tittle1);
        window.draw(tittle2);

        // Draw components
        list.draw(window);
        window.display();
    }

    return 0;
}
