#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "ManageRoomScreen.h"
#include "LoggedinScreen.h"
#include "../StartScreen.h"
#include ".././Stage3/PlayingScreenSpectate.h"
#include "../GuideGame.h"

#include ".././Object/ServerCommune.hpp"

using namespace std;
using json = nlohmann::json;

// Cấu trúc dữ liệu cho mỗi hàng
struct Row1 {
    std::string id;
    std::string playerName;
    std::string spectator;
    std::string status;
    bool pickState; // true = picked, false = not picked
};

// Các thông số kích thước và vị trí
const float TABLE_OFFSET_X = 310; // Vị trí X của bảng
const float TABLE_OFFSET_Y = 200; // Vị trí Y của bảng
const int ROW_HEIGHT = 50;        // Chiều cao mỗi hàng
const int TABLE_HEIGHT = 300;     // Chiều cao vùng hiển thị bảng
const int VISIBLE_ROWS = 6;       // Số hàng hiển thị trong bảng
const int TABLE_WIDTH = 480;      // Chiều rộng bảng

// Hàm tạo văn bản
sf::Text createText1(const std::string &str, const sf::Font &font, float x, float y, unsigned int size = 18) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x, y);
    return text;
}

// Hàm tạo nút Pick
sf::RectangleShape createPickButton1(float x, float y, bool state) {
    sf::RectangleShape button(sf::Vector2f(50, 30));
    button.setPosition(x, y);
    button.setFillColor(state ? sf::Color(100, 255, 100) : sf::Color(180, 255, 180));
    button.setOutlineThickness(1);
    button.setOutlineColor(sf::Color::Black);
    return button;
}

int ManageRoomScreenAdmin(sf::RenderWindow &window)
{
    std::string roomid = "";
    
    sf::Text nameLabel, nameWindow;
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    nameLabel.setFont(font);
    nameLabel.setString("Admin");
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    nameWindow.setFont(font);
    nameWindow.setString("MANAGE ROOMS");
    nameWindow.setCharacterSize(30);
    nameWindow.setFillColor(sf::Color::Black);
    nameWindow.setPosition(90, 25);
    nameWindow.setStyle(sf::Text::Bold);

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

    Button SpectateButton("Spectate", font, 20, {574.5, 690}, {100, 40}, sf::Color(100, 100, 250));

    // Create JSON payload
    json payload;
    payload["type"] = "VIEW_ROOMS";
    payload["data"] = {};

    // Convert JSON payload to string
    std::string pushData = payload.dump(4);
    std::cout << pushData << std::endl;

    std::string response = sendData(pushData);

    // Parse the JSON response
    json jsonResponse = json::parse(response);

    // Tạo bộ dữ liệu 
    std::vector<Row1> rows;
    if (jsonResponse["status"] == "success") {
        // Extract user data
        rows.push_back({"ID", "PLAYER", "SPEC", "STATUS", false});
        for (const auto& room : jsonResponse["data"]["users"]) {
            std::string room_id = to_string(room["id"]);
            std::string numberuser = to_string(room["current_players"]) + "/" + to_string(room["max_players"]);
            std::string numberspec = to_string(room["current_spectators"]) + "/" + to_string(room["max_spectators"]);
            std::string status = room["status"];
            rows.push_back({room_id, numberuser, numberspec, status, false});
        }
    }
    else
    {
        std::cerr << "Failed to get user data\n";
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


    std::cout << "Starting main loop\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            /// Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (BackButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    LoggedinScreenAdmin(window);
                    std::cout << "Back button clicked\n";
                }
                if (SpectateButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    PlayingScreenSpectate(roomid, 1);
                    std::cout << "Spectate button clicked\n";
                }
            }

            // Handle click on settingSprite
            if (event.type == sf::Event::MouseButtonPressed) {
                if (settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Call the function from GuideGame.cpp
                    GuideGame(); // Replace with the actual function name
                    std::cout << "Setting image clicked\n";
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
                        if(rows[i].pickState == true)
                            rows[i].pickState = false;
                        else
                        {
                            rows[i].pickState = true;
                            roomid = rows[i].id;
                        }
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
        SpectateButton.draw(window);

        // Vẽ bảng
        for (size_t i = 0; i < rows.size(); ++i) {
            float y = TABLE_OFFSET_Y + 50 + i * ROW_HEIGHT - scrollOffset;

            if (y >= TABLE_OFFSET_Y + 50 && y <= TABLE_OFFSET_Y + 350) {
                // Đường ngang
                sf::RectangleShape line(sf::Vector2f(TABLE_WIDTH - 20, 1));
                line.setPosition(TABLE_OFFSET_X + 10, y + ROW_HEIGHT - 1);
                line.setFillColor(sf::Color::Black);
                window.draw(line);

                // Vẽ ID, Player, Spectator, Status
                window.draw(createText1(rows[i].id, font, TABLE_OFFSET_X + 20, y));
                window.draw(createText1(rows[i].playerName, font, TABLE_OFFSET_X + 100, y));
                window.draw(createText1(rows[i].spectator, font, TABLE_OFFSET_X + 200, y));
                window.draw(createText1(rows[i].status, font, TABLE_OFFSET_X + 300, y));

                // Nút Pick
                sf::RectangleShape pickButton = createPickButton1(TABLE_OFFSET_X + 400, y, rows[i].pickState);
                window.draw(pickButton);
                sf::Text pickText = createText1("Pick", font, TABLE_OFFSET_X + 410, y + 5, 14);
                window.draw(pickText);
            }
        }

        // Vẽ thanh kéo
        window.draw(scrollbar);

        window.display();
    }


    return 0;
}