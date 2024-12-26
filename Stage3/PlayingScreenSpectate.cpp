#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include ".././User/JoinRoomScreenUser.h"
#include "PlayingScreenSpectate.h"
#include "type_cube_3D.h"
#include "../Solve_Cube/type_cube.h"
#include "../GuideGame.h"

using namespace std;

class ScrollableColumn {
private:
    sf::RectangleShape background;
    sf::Font font;
    std::vector<std::string> items;
    sf::Text textTemplate;
    float itemHeight;
    int visibleItems;
    int scrollOffset;

    sf::RectangleShape scrollbar;
    float scrollbarHeight;
    float scrollbarPosition;

public:
    ScrollableColumn(const sf::Vector2f& position, const sf::Vector2f& size, float itemHeight)
        : itemHeight(itemHeight), scrollOffset(0) {
        background.setPosition(position);
        background.setSize(size);
        background.setFillColor(sf::Color(200, 255, 255));

        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }

        textTemplate.setFont(font);
        textTemplate.setCharacterSize(24);
        textTemplate.setFillColor(sf::Color::Black);

        visibleItems = size.y / itemHeight;

        scrollbar.setSize({20, size.y});
        scrollbar.setPosition(position.x + size.x - 20, position.y);
        scrollbar.setFillColor(sf::Color(150, 150, 150));
    }

    void setItems(const std::vector<std::string>& newItems) {
        items = newItems;

        float viewRatio = (float)visibleItems / items.size();
        scrollbarHeight = viewRatio * background.getSize().y;
        scrollbar.setSize({20, scrollbarHeight});
    }

    std::string handleMouseClick(const sf::Vector2f& mousePosition) {
        sf::Vector2f position = background.getPosition();

        if (mousePosition.x >= position.x && mousePosition.x <= position.x + background.getSize().x - 20) {
            for (int i = 0; i < visibleItems; ++i) {
                int itemIndex = i + scrollOffset;
                if (itemIndex >= items.size()) break;

                sf::FloatRect itemBounds(
                    position.x,
                    position.y + i * itemHeight,
                    background.getSize().x - 20,
                    itemHeight);

                if (itemBounds.contains(mousePosition)) {
                    return items[itemIndex];
                }
            }
        }
        return "";
    }

    void handleScroll(float delta) {
        if (items.size() <= visibleItems) return;

        scrollOffset -= delta > 0 ? 1 : -1;
        scrollOffset = std::max(0, std::min((int)items.size() - visibleItems, scrollOffset));

        float positionRatio = (float)scrollOffset / (items.size() - visibleItems);
        scrollbarPosition = positionRatio * (background.getSize().y - scrollbarHeight);
        scrollbar.setPosition(scrollbar.getPosition().x, background.getPosition().y + scrollbarPosition);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);

        sf::Vector2f position = background.getPosition();
        for (int i = 0; i < visibleItems; ++i) {
            int itemIndex = i + scrollOffset;
            if (itemIndex >= items.size()) break;

            textTemplate.setString(items[itemIndex]);
            textTemplate.setPosition(position.x + 10, position.y + i * itemHeight);
            window.draw(textTemplate);
        }

        if (items.size() > visibleItems) {
            window.draw(scrollbar);
        }
    }
};

std::string formatTime8(sf::Time elapsed) {
    int totalSeconds = static_cast<int>(elapsed.asSeconds());
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

int PlayingScreenSpectate( std::string roomid, int status_user)
{
    int i, j, k;
    int rotateX = 0, rotateY = 0, rotateZ = 0;
    std::string usersteps = "YOU NEED TO PUBLISH THE VALUE TO THIS STRING"; // Chuỗi cập nhật liên tục các bước di chuyển từ server truyền về và hiển thị, 
    char step[3]; // Khác với bên Playing Screen User, đây là nơi truyền dữ liệu mặc dù nó có thể không cần thiết (Nếu các bạn cần 
    // thì sử dụng nhé, cần thì thêm hàm từ string suy ra bước xoay Cube tương ứng)

                                                                                                                
    step[2] = '\0';

    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML window with OpenGL", sf::Style::Default, contextSettings);

    // Make it the active window for OpenGL calls
    window.setActive();

    // Set the color and depth clear values
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Disable lighting and texturing
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

    // Font and Text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Error if font not loaded
    }

    sf::Text nameLabel("User", font, 30);
    nameLabel.setFillColor(sf::Color::Black);
    nameLabel.setPosition(1040, 20);

    sf::Text nameWindow("Time", font, 30);
    nameWindow.setFillColor(sf::Color::Black);
    nameWindow.setStyle(sf::Text::Bold);
    nameWindow.setPosition(90, 25);

    sf::Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(30);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(90, 60);

    sf::Text userstepText;
    userstepText.setFont(font);
    userstepText.setCharacterSize(15);
    userstepText.setFillColor(sf::Color::Black);
    userstepText.setPosition(20, 400);

    sf::Clock clock; // Timer

    
    
    sf::Texture settingTexture, peopleTexture;
    if (!settingTexture.loadFromFile("./Image/setting.png") || 
        !peopleTexture.loadFromFile("./Image/people.png")) {
        std::cerr << "Error loading textures\n";
        return -1;
    }

    sf::Sprite settingSprite(settingTexture);
    settingSprite.setPosition(20, 20);

    sf::Sprite peopleSprite(peopleTexture);
    peopleSprite.setPosition(1000, 20);

    Button ExitButton("Exit Lobby", font, 20, {270, 430}, {100, 40}, sf::Color(100, 100, 250));

    GLfloat cube3X3_location[27][108];
    init_cube3x3_location(cube3X3_location);

    GLfloat cube3x3_color[27][144];
    init_cube3x3_color(cube3x3_color);

    Cube input_Cube;
    freopen("../Samsung/Solve_Cube/test_case/testcase4.txt", "r", stdin);
    for (i=0; i<6; i++)
    {
        for (j=0; j<8; j++)
        {
            cin >> input_Cube.color[i][j];
        }
    }

    Spray_Cube3D(cube3x3_color, input_Cube);

    // Enable position and color vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Disable normal and texture coordinates vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Draw the cube
    glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), cube);
    glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), &cube[3]);

    ScrollableColumn column({500, 0}, {140, 200}, 20);
    column.setItems({"Phong", "Thinh", "Phuong", "Some1", "Player5", "Player6", "Player7", "Player8", "Player9", "Player5", "Player6", "Player7", "Player8", "Player9"});

    std::string selectedName;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle click on settingSprite
            if (event.type == sf::Event::MouseButtonPressed) {
                if (settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Call the function from GuideGame.cpp
                    GuideGame(); // Replace with the actual function name
                    std::cout << "Setting image clicked\n";
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) 
            {
                column.handleScroll(event.mouseWheelScroll.delta);
            } 
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                std::string clickedName = column.handleMouseClick(mousePos);
                if (!clickedName.empty()) {
                    selectedName = clickedName;
                    std::cout << "Selected: " << selectedName << std::endl; // Ở đây ta in ra tên người chơi được chọn, 
                    // freopen("../Samsung/Solve_Cube/test_case/testcase4.txt", "r", stdin);
                    // for (i=0; i<6; i++)
                    // {
                    //     for (j=0; j<8; j++)
                    //     {
                    //         cin >> input_Cube.color[i][j];
                    //     }
                    // }
                    // Spray_Cube3D(cube3x3_color, input_Cube);

                    //Mục này cần lưu ý khi nào Phong và Phương làm tới thì mình sẽ làm tiếp nhé

                }
            }

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }

            // Xử lý sự kiện cho nút
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (ExitButton.isClicked(sf::Mouse::getPosition(window)))
                {
                    std::cout << "Exit button clicked\n";
                    window.close();
                }
            }

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);

            //Rotate the perpective
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                rotateX += 1.f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                rotateX -= 1.f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                rotateY += 1.f;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                rotateY -= 1.f;
            }

            if (event.type == sf::Event::KeyPressed)
            {

                //Các bạn nhớ thêm hàm để cập nhật trạng thái rubik mới khi mà server gửi tín hiện về nhé

                Spray_Cube3D(cube3x3_color, input_Cube);
            }
        }

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.clear(sf::Color(180, 255, 240)); // Background Color

        // Apply some transformations to rotate the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        // glRotatef(clock.getElapsedTime().asSeconds() * 50, 1.f, 0.f, 0.f);
        // glRotatef(clock.getElapsedTime().asSeconds() * 30, 0.f, 1.f, 0.f);
        // glRotatef(clock.getElapsedTime().asSeconds() * 90, 0.f, 0.f, 1.f);
        glRotatef(rotateX, 1.f, 0.f, 0.f);
        glRotatef(rotateY, 0.f, 1.f, 0.f);
        
        // Draw the cube
        // Loop through each of the 27 cubes in the 3x3 cube
        for (i = 0; i < 27; i++)
        {
            // Push the matrix so each cube transformation is independent
            glPushMatrix();

            // Set up vertex and color pointers for the cube
            glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), &cube3X3_location[i]);
            glColorPointer(4, GL_FLOAT, 4 * sizeof(GLfloat), &cube3x3_color[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            // Pop the matrix to undo the translation for the next cube
            glPopMatrix();
        }

        // UI Overlay
        window.pushGLStates();
        window.draw(settingSprite);
        window.draw(peopleSprite);
        window.draw(nameLabel);
        window.draw(nameWindow);
        ExitButton.draw(window);
        column.draw(window);

        sf::Time elapsed = clock.getElapsedTime();
        timerText.setString(formatTime8(elapsed));

        if (clock.getElapsedTime().asSeconds() > 10) {
            return 3;
        }

        userstepText.setString(usersteps);
        window.draw(userstepText);
        window.draw(timerText);
        window.popGLStates();

        window.display();        
    }
    return 2;
}