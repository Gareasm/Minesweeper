#include <iostream>
#include <SFML\Graphics.hpp>
using namespace std;
#include "functions.h"

#include <map>

int main() {

    map<string, int> config = config_check();
    int width = config["width"] * 32;
    int height = 100 + (config["height"] * 32);
    int mine_count = config["mines"];



    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);
    sf::Font MyFont;
    if (!MyFont.loadFromFile("font.ttf")) {
        // Error handling
        return EXIT_FAILURE;
    }
    sf::Clock myClock;
    string name;
    int showCursor = 0;
    bool keyPressed = false;
    //Setting up text
    string welcome = "WELCOME TO MINESWEEPER!";
    sf::Text text;
    text.setString(welcome);
    text.setFont(MyFont);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(text, width/2 , (height/2) - 150);

    string enter = "Enter your name:";
    sf::Text belowText;
    belowText.setString(enter);
    belowText.setFont(MyFont);
    belowText.setCharacterSize(20);
    belowText.setFillColor(sf::Color::White);
    belowText.setStyle(sf::Text::Bold);
    setText(belowText, width/2, (height/2) - 75);

    sf::Text inputText;
    inputText.setFont(MyFont);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, width/2, (height/2) - 45);

      sf::Clock cursorClock;
        bool cursorVisible = true;
        sf::RectangleShape cursor(sf::Vector2f(1.f, static_cast<float>(inputText.getCharacterSize())));
        cursor.setFillColor(sf::Color::White);

    string userInput;
    string unfliteredName;
    bool forced_close;

    while(window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                forced_close = true;
            }
            else if (event.type == sf::Event::TextEntered) {
                if (std::isprint(static_cast<char>(event.text.unicode))) {
                    userInput += static_cast<char>(event.text.unicode);
                   userInput = NameFixer(userInput);
                } else if (event.text.unicode == '\b' && !userInput.empty()) {
                    userInput.pop_back();
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                string final_name = removeCursor(userInput);
                if (event.key.code == sf::Keyboard::Enter && (!final_name.empty())) {
                    name = final_name;
                    forced_close = false;
                    window.close();

                }

            }

        }
        if (cursorClock.getElapsedTime().asSeconds() >= 0.5f) {
            cursor.setPosition(inputText.findCharacterPos(userInput.size()).x, (height/2) - 55);
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }
        window.clear(sf::Color(0, 0, 255));
        if (cursorVisible) {
            cursor.setPosition(inputText.findCharacterPos(userInput.size()).x, (height/2) - 55);
            window.draw(cursor);
        }
        inputText.setString(userInput);











        window.draw(text);
        window.draw(belowText);
        setText(inputText, width/2, (height/2) - 45);
        window.draw(inputText);




        window.display();


    }




    if (!forced_close) {
        game_window test;
        test.sprite_loader();
        test.renderwindow(config["width"], config["height"], config["mines"], name);
        cout << "Exit: Game Window" << endl;
    }

}



