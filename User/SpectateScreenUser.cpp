#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

#include "SpectateScreenUser.h"
#include "JoinRoomScreenUser.h"
#include ".././Stage3/PlayingScreenSpectate.h"
#include "LoggedinScreenUser.h"
#include ".././FinishScreen.h"
#include ".././GuideGame.h"

#include ".././Object/ServerCommune.hpp"

using namespace std;
using json = nlohmann::json;

// Cấu trúc dữ liệu cho mỗi hàng
struct Row5 {
    std::string playerName;
    std::string status;
    std::string spectator;
};

// Các thông số kích thước và vị trí
const float TABLE_OFFSET_X1 = 310; // Vị trí X của bảng 1
const float TABLE_OFFSET_X2 = 820; // Vị trí X của bảng 2
const float TABLE_OFFSET_Y = 200;  // Vị trí Y của bảng
const int ROW_HEIGHT = 50;         // Chiều cao mỗi hàng
const int TABLE_HEIGHT = 300;      // Chiều cao vùng hiển thị bảng
const int VISIBLE_ROWS = 6;        // Số hàng hiển thị trong bảng
const int TABLE_WIDTH = 480;       // Chiều rộng bảng

// Hàm tạo văn bản
sf::Text createText5(const std::string &str, const sf::Font &font, float x, float y, unsigned int size = 18) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x, y);
    return text;
}

// Hàm vẽ bảng
void drawTable(sf::RenderWindow& window, const std::vector<Row5>& rows, float offsetX, float offsetY, const sf::Font& font, int scrollOffset) {
    for (size_t i = 0; i < rows.size(); ++i) {
        float y = offsetY + 50 + i * ROW_HEIGHT - scrollOffset;

        if (y >= offsetY + 50 && y <= offsetY + 350) {
            // Đường ngang
            sf::RectangleShape line(sf::Vector2f(TABLE_WIDTH - 20, 1));
            line.setPosition(offsetX + 10, y + ROW_HEIGHT - 1);
            line.setFillColor(sf::Color::Black);
            window.draw(line);

            // Vẽ Player, Status, Spectator
            if (offsetX == TABLE_OFFSET_X1) {
                window.draw(createText5(rows[i].playerName, font, offsetX + 20, y));
                window.draw(createText5(rows[i].status, font, offsetX + 200, y));
            } else {
                window.draw(createText5(rows[i].spectator, font, offsetX + 20, y));
            }
        }
    }
}

int SpectateScreenUser(sf::RenderWindow &window, std::string roomid, std::string username)
{
    int checkOpen=0;
    sf::Clock clock; 

    sf::Text nameLabel, tittle1, tittle2, nameWindow;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Kiểm tra nếu font không tải được
    }

    // Create JSON payload
    json payload;
    payload["type"] = "SIGN_IN";
    payload["data"]["username"] = username;

    // Convert JSON payload to string
    std::string pushData = payload.dump(4);
    std::cout << pushData << std::endl;

    std::string response = sendData(pushData);

    // Parse the JSON response
    json jsonResponse = json::parse(response);

    nameLabel.setFont(font);
    nameLabel.setString(to_string(jsonResponse["data"]["player"]["username"]));
    nameLabel.setCharacterSize(30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    nameWindow.setFont(font);
    nameWindow.setString("LOBBY");
    nameWindow.setCharacterSize(30);
    nameWindow.setFillColor(sf::Color::Black);
    nameWindow.setPosition(90, 25);
    nameWindow.setStyle(sf::Text::Bold);

    tittle1.setFont(font);
    tittle1.setString("WAITING FOR PLAYERS...");
    tittle1.setCharacterSize(20);
    tittle1.setFillColor(sf::Color::Black);
    tittle1.setPosition(450, 100);
    tittle1.setStyle(sf::Text::Bold);

    tittle2.setFont(font);
    tittle2.setString("GAME START IN" + std::to_string(10));
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


    Button ExitButton("Exit Lobby", font, 20, {574.5, 760}, {100, 40}, sf::Color(100, 100, 250));

     // Create JSON payload
    json payload1;
    payload1["type"] = "JOIN_ROOM";
    payload1["data"]["room_id"] = std::stoi(roomid);
    payload1["data"]["participant_type"] = "PLAYER";

    // Convert JSON payload to string
    std::string pushData1 = payload1.dump(4);
    std::cout << pushData1 << std::endl;

    std::string response1 = sendData(pushData1);

    // Parse the JSON response
    json jsonResponse1 = json::parse(response1);

    // Tạo bộ dữ liệu 20 hàng cho mỗi bảng
    std::vector<Row5> rows1, rows2;

    rows1.push_back({"Player", "Status", "Ready"});
    rows2.push_back({"Spectator", "Status", "Ready"});
    for (const auto& participant : jsonResponse1["data"]["room_participants"]) {
        std::string participantType = participant["participant_type"];
        std::string status = participant["is_ready"] ? "Ready" : "Not Ready";
        std::string participantId = to_string(participant["participant_id"]);

        if (participantType == "PLAYER") {
            rows1.push_back({participantId, "Active", status});
        } else if (participantType == "PLAYER_SPECTATOR" || participantType == "ADMIN_SPECTATOR") {
            rows2.push_back({participantId, "Spectator", status});
        }
    }

    // Biến điều khiển cuộn
    int scrollOffset = 0; // Vị trí cuộn hiện tại

    // Tính toán thanh kéo
    float scrollbarHeight = (float)TABLE_HEIGHT / (rows1.size() * ROW_HEIGHT) * TABLE_HEIGHT;
    sf::RectangleShape scrollbar(sf::Vector2f(15, scrollbarHeight));
    scrollbar.setPosition(TABLE_OFFSET_X1 + TABLE_WIDTH - 10, TABLE_OFFSET_Y + 50);
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
                if (ExitButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    JoinRoomScreenUser(window, username);
                    std::cout << "Exit button clicked\n";
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
                scrollOffset = std::max(0, std::min(scrollOffset, (int)(rows1.size() * ROW_HEIGHT - TABLE_HEIGHT)));
                scrollbar.setPosition(TABLE_OFFSET_X1 + TABLE_WIDTH - 10,
                                      TABLE_OFFSET_Y + 50 + (float)scrollOffset / (rows1.size() * ROW_HEIGHT) * TABLE_HEIGHT);
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
                scrollbar.setPosition(TABLE_OFFSET_X1 + TABLE_WIDTH - 10, newY);
                scrollOffset = ((newY - TABLE_OFFSET_Y - 50) / TABLE_HEIGHT) * (rows1.size() * ROW_HEIGHT - TABLE_HEIGHT);
            }
        }
        // Drawing
        window.clear(sf::Color(180, 255, 240)); // Background Color

        window.draw(settingSprite);
        window.draw(peopleSprite);
        window.draw(nameLabel);
        window.draw(nameWindow);

        window.draw(tittle1);
        window.draw(tittle2);

        ExitButton.draw(window);


        // Vẽ bảng 1 (Player và Status)
        drawTable(window, rows1, TABLE_OFFSET_X1, TABLE_OFFSET_Y, font, scrollOffset);

        // Vẽ bảng 2 (Spectator)
        drawTable(window, rows2, TABLE_OFFSET_X2, TABLE_OFFSET_Y, font, scrollOffset);

        // Vẽ thanh kéo
        window.draw(scrollbar);
        
        
        std::cout << "Ready\n";
        if (clock.getElapsedTime().asSeconds() >= 3) {
            std::cout << "3 seconds have passed!" << std::endl;
            checkOpen = 1; //Khi checkOpen == 1 thì nghĩa là từ server đưa ra thông báo bắt đầu game
        }

        if (checkOpen == 1)
        {
            checkOpen = PlayingScreenSpectate( roomid, 2);
        }

        if (checkOpen == 2)
        {
            LoggedinScreenUser(window, username);
        }
        else if (checkOpen == 3)
        {
            FinishScreen(window, 2, username);
        }
        

        window.display();
    }

    return 0;
    
}
