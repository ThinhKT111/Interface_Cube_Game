#include <SFML/Graphics.hpp>

#include ".././Object/Button.hpp"
#include ".././Object/TextBox.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include ".././User/JoinRoomScreenUser.h"
#include "PlayingScreenUser.h"
#include "type_cube_3D.h"
#include "../Solve_Cube/type_cube.h"
#include "../GuideGame.h"

using namespace std;

std::string formatTime(sf::Time elapsed) {
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

int PlayingScreenUser( std::string roomid)
{
    int i, j, k;
    int rotateX = 0, rotateY = 0, rotateZ = 0;
    std::string usersteps = ""; // Chuỗi cập nhật liên tục để hiển thị các bước  của người chơi đã đi
    char step[3]; // Tại đây khai báo để các bạn lấy biến step này để lưu bước đi hiện tại của người chơi. Ví dụ: F0 là Clockwise
                                                                                                                // F' là Counter Clockwise
                                                                                                                
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

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

            // Handle click on settingSprite
            if (event.type == sf::Event::MouseButtonPressed) {
                if (settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    // Call the function from GuideGame.cpp
                    GuideGame(); // Replace with the actual function name
                    std::cout << "Setting image clicked\n";
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
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
                {
                    step[1] = '\'';
                    if (event.key.code == sf::Keyboard::U)
                    {
                        input_Cube = Rotate_Anti_Clockwise(input_Cube, 0);
                        step[0] = 'U';
                    }
                    if (event.key.code == sf::Keyboard::L)
                    {
                        input_Cube = Rotate_Anti_Clockwise(input_Cube, 1);
                        step[0] = 'L';
                    }
                    if (event.key.code == sf::Keyboard::F)
                    {
                        input_Cube = Rotate_Anti_Clockwise(input_Cube, 2);
                        step[0] = 'F';
                    }
                    if (event.key.code == sf::Keyboard::R)
                    {
                        input_Cube = Rotate_Anti_Clockwise(input_Cube, 3);
                        step[0] = 'R';
                    }
                    if (event.key.code == sf::Keyboard::B)
                    {
                        input_Cube = Rotate_Anti_Clockwise(input_Cube, 4);
                        step[0] = 'B';
                    }
                    if (event.key.code == sf::Keyboard::D)
                    {
                        input_Cube = Rotate_Anti_Clockwise(input_Cube, 5);
                        step[0] = 'D';
                    }
                    usersteps += step;
                    std::cout << "///" << usersteps << "???" << std::endl;
                }     
                
                else
                {
                    if (event.key.code == sf::Keyboard::U)
                    {
                        input_Cube = Rotate_Clockwise(input_Cube, 0);
                        step[0] = 'U';
                        step[1] = ' ';
                        usersteps += step;
                        std::cout << "///" << usersteps << "???" << std::endl;
                    }
                    if (event.key.code == sf::Keyboard::L)
                    {
                        input_Cube = Rotate_Clockwise(input_Cube, 1);
                        step[0] = 'L';
                        step[1] = ' ';
                        usersteps += step;
                        std::cout << "///" << usersteps << "???" << std::endl;
                    }
                    if (event.key.code == sf::Keyboard::F)
                    {
                        input_Cube = Rotate_Clockwise(input_Cube, 2);
                        step[0] = 'F';
                        step[1] = ' ';
                        usersteps += step;
                        std::cout << "///" << usersteps << "???" << std::endl;
                    }
                    if (event.key.code == sf::Keyboard::R)
                    {
                        input_Cube = Rotate_Clockwise(input_Cube, 3);
                        step[0] = 'R';
                        step[1] = ' ';
                        usersteps += step;
                        std::cout << "///" << usersteps << "???" << std::endl;
                    }
                    if (event.key.code == sf::Keyboard::B)
                    {
                        input_Cube = Rotate_Clockwise(input_Cube, 4);
                        step[0] = 'B';
                        step[1] = ' ';
                        usersteps += step;
                        std::cout << "///" << usersteps << "???" << std::endl;
                    }
                    if (event.key.code == sf::Keyboard::D)
                    {
                        input_Cube = Rotate_Clockwise(input_Cube, 5);
                        step[0] = 'D';
                        step[1] = ' ';
                        usersteps += step;
                        std::cout << "///" << usersteps << "???" << std::endl;
                    }
                }
                
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

        sf::Time elapsed = clock.getElapsedTime();
        timerText.setString(formatTime(elapsed));

        if (clock.getElapsedTime().asSeconds() > 10) {
            return 3;
        } //Check finish screen
        

        userstepText.setString(usersteps);
        window.draw(userstepText);
        window.draw(timerText);
        window.popGLStates();

        window.display();        
    }
    return 2;
}