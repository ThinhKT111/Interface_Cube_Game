#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>

#include "JoinRoomScreenUser.h"
#include "../StartScreen.h"
#include "LoggedinScreenUser.h"
#include "LobbyScreenUser.h"
#include "SpectateScreenUser.h"
#include "../GuideGame.h"

using namespace std;

// Cấu trúc dữ liệu cho mỗi hàng
struct Row {
    std::string id;
    std::string name;
    std::string status;
    bool pickState; // true = xanh đậm, false = xanh nhạt
};

// Các thông số kích thước và vị trí
const float TABLE_OFFSET_X = 310; // Vị trí X của bảng
const float TABLE_OFFSET_Y = 200; // Vị trí Y của bảng
const int ROW_HEIGHT = 50;        // Chiều cao mỗi hàng
const int TABLE_HEIGHT = 300;     // Chiều cao vùng hiển thị bảng
const int VISIBLE_ROWS = 6;       // Số hàng hiển thị trong bảng
const int TABLE_WIDTH = 480;      // Chiều rộng bảng

// Hàm tạo văn bản
sf::Text createText2(const std::string &str, const sf::Font &font, float x, float y, unsigned int size = 18) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x, y);
    return text;
}

// Hàm tạo nút Pick
sf::RectangleShape createPickButton2(float x, float y, bool state) {
    sf::RectangleShape button(sf::Vector2f(50, 30));
    button.setPosition(x, y);
    button.setFillColor(state ? sf::Color(100, 100, 255) : sf::Color(180, 180, 255));
    button.setOutlineThickness(1);
    button.setOutlineColor(sf::Color::Black);
    return button;
}

int JoinRoomScreenUser(sf::RenderWindow &window)
{
    std::string roomid = "";
    
    sf::Text nameLabel, tittle1, tittle2, nameWindow;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    nameLabel.setFont(font);
    nameLabel.setString("User");
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    nameWindow.setFont(font);
    nameWindow.setString("JOIN ROOM");
    nameWindow.setCharacterSize(30);
    nameWindow.setFillColor(sf::Color::Black);
    nameWindow.setPosition(90, 25);
    nameWindow.setStyle(sf::Text::Bold);

    tittle1.setFont(font);
    tittle1.setString("Enter your room ID:");
    tittle1.setCharacterSize(20);
    tittle1.setFillColor(sf::Color::Black);
    tittle1.setPosition(450, 100);
    tittle1.setStyle(sf::Text::Bold);

    tittle2.setFont(font);
    tittle2.setString("Or browse current room:");
    tittle2.setCharacterSize(20);
    tittle2.setFillColor(sf::Color::Black);
    tittle2.setPosition(450, 200);
    tittle2.setStyle(sf::Text::Italic);


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

    Button JoinButton("Join", font, 20, {424.5, 700}, {150, 40}, sf::Color(100, 100, 250));

    Button SpectateButton("Spectate", font, 20, {674.5, 700}, {150, 40}, sf::Color(100, 100, 250));

    TextBox roomidBox(470.5, 150, 200, 40, "arial.ttf");

    // Tạo bộ dữ liệu 20 hàng
    std::vector<Row> rows;
    rows.push_back({"ID", "PLAYER", "SPECTATORS", false});
    for (int i = 2; i <= 20; ++i) {
        rows.push_back({std::to_string(i-1), std::to_string(i-2) + "/" + std::to_string(i-1), std::to_string(i+3), false});
    }

    // Biến điều khiển cuộn
    int scrollOffset = 0; // Vị trí cuộn hiện tại

    // Tính toán thanh kéo
    float scrollbarHeight = (float)TABLE_HEIGHT / (rows.size() * ROW_HEIGHT) * TABLE_HEIGHT;
    sf::RectangleShape scrollbar(sf::Vector2f(15, scrollbarHeight));
    scrollbar.setPosition(TABLE_OFFSET_X + TABLE_WIDTH - 10, TABLE_OFFSET_Y + 50);
    scrollbar.setFillColor(sf::Color(150, 150, 150));

    bool dragging = false;
    float dragOffset = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            roomidBox.handleEvent(event, window);

            // Khi nhấn phím Enter, lưu giá trị TextBox vào string
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                roomid = roomidBox.getText();

                // Hiển thị giá trị ra console (hoặc sử dụng giá trị này ở nơi khác)
                std::cout << "TextBox 1: " << roomid << std::endl;
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
                    LoggedinScreenUser(window);
                    std::cout << "Back button clicked\n";
                }
                else if (JoinButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    LobbyScreenUser(window, roomid);
                    std::cout << "Join button clicked\n";
                }
                else if (SpectateButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    SpectateScreenUser(window, roomid);
                    std::cout << "Spectate button clicked\n";
                }
            }

            // Sự kiện cuộn chuột
            if (event.type == sf::Event::MouseWheelScrolled) {
                scrollOffset += event.mouseWheelScroll.delta * -20;
                scrollOffset = std::max(0, std::min(scrollOffset, (int)(rows.size() * ROW_HEIGHT - TABLE_HEIGHT)));
                scrollbar.setPosition(TABLE_OFFSET_X + TABLE_WIDTH - 10,
                                      TABLE_OFFSET_Y + 50 + (float)scrollOffset / (rows.size() * ROW_HEIGHT) * TABLE_HEIGHT);
            }

            // Kéo thanh kéo
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (scrollbar.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    dragging = true;
                    dragOffset = event.mouseButton.y - scrollbar.getPosition().y;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
                dragging = false;

            if (event.type == sf::Event::MouseMoved && dragging) {
                float newY = event.mouseMove.y - dragOffset;
                newY = std::max(TABLE_OFFSET_Y + 50.0f, std::min(newY, TABLE_OFFSET_Y + 50.0f + TABLE_HEIGHT - scrollbarHeight));
                scrollbar.setPosition(TABLE_OFFSET_X + TABLE_WIDTH - 10, newY);
                scrollOffset = ((newY - TABLE_OFFSET_Y - 50) / TABLE_HEIGHT) * (rows.size() * ROW_HEIGHT - TABLE_HEIGHT);
            }

            // Xử lý nút "Pick"
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < rows.size(); ++i) {
                    float y = TABLE_OFFSET_Y + 50 + i * ROW_HEIGHT - scrollOffset;
                    if (event.mouseButton.x >= TABLE_OFFSET_X + 400 && event.mouseButton.x <= TABLE_OFFSET_X + 450 &&
                        event.mouseButton.y >= y && event.mouseButton.y <= y + 30) {
                        rows[i].pickState = !rows[i].pickState;
                        std::cout << "Pick button clicked on row " << i << std::endl;
                    }
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
        JoinButton.draw(window);
        SpectateButton.draw(window);
        roomidBox.draw(window);
        window.draw(tittle1);
        window.draw(tittle2);


        // Vẽ bảng
        for (size_t i = 0; i < rows.size(); ++i) {
            float y = TABLE_OFFSET_Y + 50 + i * ROW_HEIGHT - scrollOffset;

            if (y >= TABLE_OFFSET_Y + 50 && y <= TABLE_OFFSET_Y + 350) {
                // Đường ngang
                sf::RectangleShape line(sf::Vector2f(TABLE_WIDTH - 20, 1));
                line.setPosition(TABLE_OFFSET_X + 10, y + ROW_HEIGHT - 1);
                line.setFillColor(sf::Color::Black);
                window.draw(line);

                // Vẽ ID, Name, Status
                window.draw(createText2(rows[i].id, font, TABLE_OFFSET_X + 20, y));
                window.draw(createText2(rows[i].name, font, TABLE_OFFSET_X + 100, y));
                window.draw(createText2(rows[i].status, font, TABLE_OFFSET_X + 250, y));

                // Nút Pick
                sf::RectangleShape pickButton = createPickButton2(TABLE_OFFSET_X + 400, y, rows[i].pickState);
                window.draw(pickButton);
                sf::Text pickText = createText2("Pick", font, TABLE_OFFSET_X + 410, y + 5, 14);
                window.draw(pickText);
            }
        }

        // Vẽ thanh kéo
        window.draw(scrollbar);

        window.display();
    }

    return 0;
    
}
