#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "ManageUserScreen.h"
#include "LoggedinScreen.h"
#include "../StartScreen.h"
#include "../GuideGame.h"

#include ".././Object/ServerCommune.hpp"

using namespace std;
using json = nlohmann::json;

// Cấu trúc dữ liệu cho mỗi hàng
struct Row {
    std::string id;
    std::string name;
    std::string status;
    bool banState; // true = xanh đậm, false = xanh nhạt
};

// Các thông số kích thước và vị trí
const float TABLE_OFFSET_X = 310; // Vị trí X của bảng
const float TABLE_OFFSET_Y = 200; // Vị trí Y của bảng
const int ROW_HEIGHT = 50;        // Chiều cao mỗi hàng
const int TABLE_HEIGHT = 300;     // Chiều cao vùng hiển thị bảng
const int VISIBLE_ROWS = 6;       // Số hàng hiển thị trong bảng
const int TABLE_WIDTH = 480;      // Chiều rộng bảng

// Hàm tạo văn bản
sf::Text createText(const std::string &str, const sf::Font &font, float x, float y, unsigned int size = 18) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x, y);
    return text;
}

// Hàm tạo nút Ban
sf::RectangleShape createBanButton(float x, float y, bool state) {
    sf::RectangleShape button(sf::Vector2f(50, 30));
    button.setPosition(x, y);
    button.setFillColor(state ? sf::Color(100, 100, 255) : sf::Color(180, 180, 255));
    button.setOutlineThickness(1);
    button.setOutlineColor(sf::Color::Black);
    return button;
}

int ManageUserScreenAdmin(sf::RenderWindow &window)
{
    sf::Text nameLabel, nameWindow;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    nameLabel.setFont(font);
    nameLabel.setString("Admin");
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    nameWindow.setFont(font);
    nameWindow.setString("MANAGE USERS");
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

    // Create JSON payload
    json payload;
    payload["type"] = "VIEW_USERS";
    payload["data"] = {};

    // Convert JSON payload to string
    std::string pushData = payload.dump(4);
    std::cout << pushData << std::endl;

    std::string response = sendData(pushData);

    // Check if the response is empty
    if (response.empty()) {
        std::cerr << "Received empty response from server" << std::endl;
        return -1;
    }

    // Parse the JSON response
    json jsonResponse;
    try {
        jsonResponse = json::parse(response);
    } catch (const json::parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return -1;
    }

    // Cần xử lý dữ liệu trả về để hiển thị lên bảng

    // Tạo bộ dữ liệu 
    std::vector<Row> rows;
    if (jsonResponse["status"] == "success") {
        // Extract user data
        rows.push_back({"ID", "USERNAME", "STATUS", false});
        for (const auto& user : jsonResponse["data"]["users"]) {
            std::string id = to_string(user["id"]);
            std::string username = user["username"];
            std::string status = user["status"];
            bool isBanned = !user["ban_by"].is_null() && user["ban_by"] != nullptr;
            rows.push_back({id, username, status, isBanned});
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
                    LoggedinScreenAdmin(window);
                    std::cout << "Back button clicked\n";
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

            // Xử lý nút "Ban"
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < rows.size(); ++i) {
                    float y = TABLE_OFFSET_Y + 50 + i * ROW_HEIGHT - scrollOffset;
                    if (event.mouseButton.x >= TABLE_OFFSET_X + 400 && event.mouseButton.x <= TABLE_OFFSET_X + 450 &&
                        event.mouseButton.y >= y && event.mouseButton.y <= y + 30) {
                        rows[i].banState = false;
                        std::cout << "Ban button clicked on row " << i << std::endl;
                        json banPayload;
                        banPayload["type"] = "BAN_PLAYER";
                        banPayload["data"]["player_id"] = std::stoi(rows[i].id);
                        banPayload["data"]["admin_id"] = "3"; // Replace with actual admin ID

                        // Convert JSON payload to string
                        std::string banData = banPayload.dump(4);
                        std::cout << banData << std::endl;

                        // Send data to server
                        std::string banResponse = sendData(banData);

                        // Parse the JSON response
                        json banJsonResponse = json::parse(banResponse);

                        // Check the status field
                        if (banJsonResponse["status"] == "success") {
                            std::cout << "Player banned successfully: " << banJsonResponse["data"]["message"] << std::endl;
                        } else {
                            std::cerr << "Failed to ban player: " << banJsonResponse["data"]["message"] << std::endl;
                        }
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
                window.draw(createText(rows[i].id, font, TABLE_OFFSET_X + 20, y));
                window.draw(createText(rows[i].name, font, TABLE_OFFSET_X + 100, y));
                window.draw(createText(rows[i].status, font, TABLE_OFFSET_X + 250, y));

                // Nút Ban
                sf::RectangleShape banButton = createBanButton(TABLE_OFFSET_X + 400, y, rows[i].banState);
                window.draw(banButton);
                sf::Text banText = createText("Ban", font, TABLE_OFFSET_X + 410, y + 5, 14);
                window.draw(banText);
            }
        }

        // Vẽ thanh kéo
        window.draw(scrollbar);

        window.display();
    }

    return 0;
}