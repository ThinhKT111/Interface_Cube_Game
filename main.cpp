#include <SFML/Graphics.hpp>
#include "./Object/Button.hpp"
#include <iostream>

#include "./Stage2/LoginScreen.h"
#include "./Stage2/RegisterScreen.h"

#include "StartScreen.h"

using namespace std;

int main() {
    

    const int windowWidth = 1249;
    const int windowHeight = 829;
    // Tạo cửa sổ
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Reusable Button with Background");

    

    // Vòng lặp chính
    while (window.isOpen()) {
        StartScreen(window);
    }

    return 0;
}
