//
// Created by Gabriel on 4/15/2024.
//

#ifndef MINESWEEPER_FUNCTIONS_H
#define MINESWEEPER_FUNCTIONS_H

#endif //MINESWEEPER_FUNCTIONS_H

using namespace std;
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <chrono>
#include "tileandmines.h"
#include <iomanip>


string NameFixer(string input){
    string fixedString;
    bool isFirst = true;
    int count = 0;
    for (char c : input) {
        count++;
        if (count == 11){
            return fixedString;
        }
        if (isalpha(c)) {
            if (isFirst) {
                fixedString += toupper(c);
                isFirst = false;
            } else {
                fixedString += tolower(c);
            }
        }
    }
    return fixedString;
}

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));

}

map<string, int> config_check(){
    int numbers [2];
    map<string, int> my_map;
    fstream file("config.cfg");
    if (!file.is_open()) {
        std::cout << "Failed to open the configuration file." << std::endl;
    }
    string line;
    getline(file, line, '\n');
        int numtostore = stoi(line);
        my_map["width"] = numtostore;
    getline(file, line, '\n');
        numtostore = stoi(line);
        my_map["height"] = numtostore;
    getline(file, line, '\n');
        numtostore = stoi(line);
        my_map["mines"] = numtostore;
    return my_map;
}

string erase(string str){
    if (str.empty()) {
        return str;
    } else {
        return str.substr(0, str.size() - 1);
    }
}

string removeCursor(string stran) {
    if (!stran.empty() && stran.back() == '|') {
        stran.pop_back(); // Remove the last character (the cursor bar '|')
    }
    return stran;
}

int offset_calc(int num){
    int offset = 21 * num;
    return offset;
}



class LeaderBoard{
public:
    map<string, string> leaderboard;
    LeaderBoard()= default;
    void renderwindow(int col, int row, string rawname) {
        int width = (col * 16) + 50;
        int height = row * 16;
        string playername = " " + rawname;
        sf::RenderWindow leaderwindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
        sf::Font MyFont;
        if (!MyFont.loadFromFile("font.ttf")) {
            cout << "Error loading font" << endl;
        }
        string title = "LEADERBOARD";
        sf::Text title_text;
        title_text.setString(title);
        title_text.setFont(MyFont);
        title_text.setCharacterSize(20);
        title_text.setFillColor(sf::Color::White);
        title_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        setText(title_text, width / 2, (height / 2) - 110);

        string content;
        sf::Text leader_conent;
        leader_conent.setFont(MyFont);
        leader_conent.setCharacterSize(18);
        leader_conent.setFillColor(sf::Color::White);
        leader_conent.setStyle(sf::Text::Bold);


        ifstream file("leaderboard.txt");
        if (!file.is_open()) {
            cout << "Failed to open the leaderboard file." << std::endl;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string time, name;
            std::getline(iss, time, ',');
            std::getline(iss, name);
            leaderboard[time] = name;
        }
        file.close();

        int count = 1;
        for (auto it = leaderboard.begin(); it != leaderboard.end() && count < 6; ++it, ++count) {
            cout << "Playername: " << playername << " second: " << it->second <<endl;
            if(playername == it->second){
                std::cout << "Player: " << it->first << ", Score: " << it->second + "*" << std::endl;
                content += to_string(count) + "\t" + it->first + "\t" + it->second + "*" + "\n";
            }
            else {
                std::cout << "Player: " << it->first << ", Score: " << it->second << std::endl;
                content += to_string(count) + "\t" + it->first + "\t" + it->second + "\n";
            }

        }
        leader_conent.setString(content);
        setText(leader_conent, width / 2, (height / 2) + 20);


        while (leaderwindow.isOpen()) {

            sf::Event event;
            while (leaderwindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    leaderwindow.close();
                }
            }
            leaderwindow.clear(sf::Color(0, 0, 255));
            leaderwindow.draw(title_text);
            leaderwindow.draw(leader_conent);
            leaderwindow.display();
        }
    }

    void addscore(string name, string time){
        //name += "*";
        ofstream file("leaderboard.txt",std::ios::app);
        if (!file.is_open()) {
            cout << "Failed to open the leaderboard file." << std::endl;
        }
        if (file.tellp() = 0)
            file << std::endl;
        file << "\n" << time << ", " << name;

        // Close the file
        file.close();
    }
};


class Board{
public:
    int mines;

    void rendertiles(int collums, int rows, sf::RenderWindow &window){
        sf::Sprite tilesprite;
        sf::Texture tiletexture;
        tiletexture.loadFromFile("images/tile_revealed.png");
        tilesprite.setTexture(tiletexture);
        int x_position = 0;
        int y_position = 0;
        int x_offset = 32;
        int y_offset = 32;
        for (int i = 0; i < rows; i++){
            for(int j = 0; j < collums; j++){
                tilesprite.setPosition(x_position, y_position);
                window.draw(tilesprite);
                x_position += x_offset;
            }
            y_position += y_offset;
            x_position = 0;
        }

    }

};




class Timer {
public:
    int minutes;
    int seconds;
    int unconverted_seconds;
    bool paused = false;
    sf::Clock clock;
    sf::Clock pauserclock;
    sf::Time final_time = sf::Time::Zero;
    sf::Time pause_time = sf::Time::Zero;

    Timer() = default;

    void update_time() {
        if (!paused) {
            pause_time;
            current_time = clock.getElapsedTime();
            if (current_time >= pause_time) {
                minutes = (static_cast<int>(current_time.asSeconds())) / 60;
                seconds = (static_cast<int>(current_time.asSeconds())) % 60;
            } else {
                final_time = current_time + pause_time;
                minutes = (static_cast<int>(final_time.asSeconds()))  / 60;
                seconds = (static_cast<int>(final_time.asSeconds())) % 60;

            }
        }
    }

    std::string getTimeString() {
        std::ostringstream stringtoreturn;
        stringtoreturn << std::setw(2) << std::setfill('0') << minutes << ":"
                       << std::setw(2) << std::setfill('0') << seconds;
        return stringtoreturn.str();
    }

    void reset_time() {
        clock.restart();
        pauserclock.restart();
        pause_time = sf::Time::Zero;
        paused = false;
        update_time(); // Update time after resetting
    }

    void pauseTimer() {
            paused = true;
            //pause_time = clock.getElapsedTime();
            //pauserclock.restart();
            cout << "P time: " << static_cast<int>(pause_time.asSeconds())% 60<< " currtime: " << static_cast<int>(current_time.asSeconds())% 60<< endl;
            if (pause_time <= current_time){
                pause_time = current_time;
            }
            else {
                pause_time = final_time;
            }
    }

    void unpauseTimer() {
            clock.restart();
        cout << "P time: " << static_cast<int>(pause_time.asSeconds())% 60<< " currtime: " << static_cast<int>(current_time.asSeconds())% 60<< endl;
            paused = false;
    }

private:
    sf::Time current_time = sf::Time::Zero;
};

void rendercounter(int col, int row, sf::RenderWindow &window, int counter){
    int digitone, digittwo, digitthree, temp;
    bool negative = false;
    //999
    //12
    if(counter < 0){
        counter *= -1;
        negative = true;
    }
    digitone = counter / 100;
    digittwo = (counter / 10) % 10;
    digitthree = counter % 10;


    sf::Texture numbers1;
    sf::Texture numbers2;
    sf::Texture numbers3;
    sf::Texture negative_text;

    sf::Sprite one;
    numbers1.loadFromFile("images/digits.png", sf::IntRect(offset_calc(digitone), 0, 21, 32));
    one.setTexture(numbers1);
    sf::Sprite two;
    numbers2.loadFromFile("images/digits.png", sf::IntRect(offset_calc(digittwo), 0, 21, 32));
    two.setTexture(numbers2);
    sf::Sprite three;
    numbers3.loadFromFile("images/digits.png", sf::IntRect(offset_calc(digitthree), 0, 21, 32));
    three.setTexture(numbers3);
    sf::Sprite negative_nancy;
    negative_text.loadFromFile("images/digits.png", sf::IntRect(offset_calc(10), 0, 21, 32));
    negative_nancy.setTexture(negative_text);

    int x = 33;
    int offset = 21;
    int y = 16 + ((row + 0.5) * 32);
    one.setPosition(x,y);
    two.setPosition(x + offset, y);
    three.setPosition(x + offset + offset, y);
    negative_nancy.setPosition(12, y);

    window.draw(one);
    window.draw(two);
    window.draw(three);
    if(negative){
        window.draw(negative_nancy);
    }

}


    void render(int col, int row, sf::RenderWindow &window, Timer &time) {
        //make sure u have the right time

        time.update_time();

        //define varivables: Minute Top, Minute Bottom, Seconds Top, Seconds Bottom
        int mintop, minbot, sectop, secbot;
        //Calculate time
        mintop = time.minutes / 10;
        minbot = time.minutes % 10;
        sectop = time.seconds / 10;
        secbot = time.seconds % 10;
        //LINE BELOW FOR TROUBLESHOOTING TIMER
        //cout<< mintop << "" << minbot << ":" << sectop << "" << secbot << endl;
        sf::Texture numbers1;
        sf::Texture numbers2;
        sf::Texture numbers3;
        sf::Texture numbers4;


        sf::Sprite twoMins;
        numbers1.loadFromFile("images/digits.png", sf::IntRect(offset_calc(mintop), 0, 21, 32));
        twoMins.setTexture(numbers1);
        sf::Sprite firstMins;
        numbers2.loadFromFile("images/digits.png", sf::IntRect(offset_calc(minbot), 0, 21, 32));
        firstMins.setTexture(numbers2);
        sf::Sprite twoSecs;
        numbers3.loadFromFile("images/digits.png", sf::IntRect(offset_calc(sectop), 0, 21, 32));
        twoSecs.setTexture(numbers3);
        sf::Sprite firstSecs;
        numbers4.loadFromFile("images/digits.png", sf::IntRect(offset_calc(secbot), 0, 21, 32));
        firstSecs.setTexture(numbers4);





        //set coordinates to use
        int x = (col * 32) - 97;
        int y = 16 + ((row + 0.5) * 32);
        twoMins.setPosition(x, y);
        firstMins.setPosition(x + 21, y);
        int sx = (col * 32) - 54;
        int sy = 16 + ((row + 0.5) * 32);
        twoSecs.setPosition(sx, sy);
        firstSecs.setPosition(sx + 21, sy);

        window.draw(twoMins);
        window.draw(firstMins);
        window.draw(twoSecs);
        window.draw(firstSecs);

    }


void renderwithoutop(int col, int row, sf::RenderWindow &window, Timer &time){
    //make sure u have the right time

    //time.update_time();

    //define varivables: Minute Top, Minute Bottom, Seconds Top, Seconds Bottom
    int mintop, minbot, sectop, secbot;
    //Calculate time
    mintop = time.minutes / 10;
    minbot = time.minutes % 10;
    sectop = time.seconds / 10;
    secbot = time.seconds % 10;
    //LINE BELOW FOR TROUBLESHOOTING TIMER
    //cout<< mintop << "" << minbot << ":" << sectop << "" << secbot << endl;
    sf::Texture numbers1;
    sf::Texture numbers2;
    sf::Texture numbers3;
    sf::Texture numbers4;


    sf::Sprite twoMins;
    numbers1.loadFromFile("images/digits.png", sf::IntRect(offset_calc(mintop), 0, 21, 32));
    twoMins.setTexture(numbers1);
    sf::Sprite firstMins;
    numbers2.loadFromFile("images/digits.png", sf::IntRect(offset_calc(minbot), 0, 21, 32));
    firstMins.setTexture(numbers2);
    sf::Sprite twoSecs;
    numbers3.loadFromFile("images/digits.png", sf::IntRect(offset_calc(sectop), 0, 21, 32));
    twoSecs.setTexture(numbers3);
    sf::Sprite firstSecs;
    numbers4.loadFromFile("images/digits.png", sf::IntRect(offset_calc(secbot), 0, 21, 32));
    firstSecs.setTexture(numbers4);





    //set coordinates to use
    int x = (col * 32) - 97;
    int y = 16 + ((row + 0.5) * 32);
    twoMins.setPosition(x,y);
    firstMins.setPosition(x + 21, y);
    int sx = (col * 32) - 54;
    int sy = 16 +  ((row + 0.5) * 32 );
    twoSecs.setPosition(sx, sy);
    firstSecs.setPosition(sx + 21, sy);

    window.draw(twoMins);
    window.draw(firstMins);
    window.draw(twoSecs);
    window.draw(firstSecs);

}





class game_window{
private:
    //base tile
    sf::Sprite tilesprite;
        sf::Texture tiletexture;
    //timer
        //minutes
        //seconds

    //counter

    //faces
    sf::Sprite face_sprite;
        sf::Texture happy_face_texture;
        sf::Texture win_face_texture;
        sf::Texture lose_face_texture;


    //pause play button
    sf::Sprite playpause_button_sprite;
        sf::Texture pause_texture;
        sf::Texture play_texture;

    //debug button
    sf::Sprite debug_button_sprite;
        sf::Texture debug_texture;

    //leaderboard
    sf::Sprite leaderboard_sprite;
        sf::Texture leaderboard_texture;

    LeaderBoard gameLeaderBoard;

    sf::Texture t_hidden_tile;
    sf::Texture t_revealed_tile;
    sf::Texture t_flag;
    sf::Texture t_mine;
    sf::Sprite hidden_tile;
    sf::Sprite revealed_tile;
    sf::Sprite flag;
    sf::Sprite mine;
    sf::Texture t_one, t_two, t_three, t_four, t_five, t_six, t_seven, t_eight;
    sf::Sprite one, two, three, four, five, six, seven, eight;
    vector<sf::Sprite> numbers;

public:
    void sprite_loader(){
        //tile loading
        tiletexture.loadFromFile("images/tile_hidden.png");
        tilesprite.setTexture(tiletexture);
        cout << "ILOADED" << endl;
        //clock loading
        //timer loading
        //faces loading
        happy_face_texture.loadFromFile("images/face_happy.png");
        win_face_texture.loadFromFile("images/face_win.png");
        lose_face_texture.loadFromFile("images/face_lose.png");
        face_sprite.setTexture(happy_face_texture);

        //play pause loading
        pause_texture.loadFromFile("images/pause.png");
        play_texture.loadFromFile("images/play.png");
        playpause_button_sprite.setTexture(pause_texture);
        //debug loading
        debug_texture.loadFromFile("images/debug.png");
        debug_button_sprite.setTexture(debug_texture);
        //leaderboard laoding
        leaderboard_texture.loadFromFile("images/leaderboard.png");
        leaderboard_sprite.setTexture(leaderboard_texture);

        t_hidden_tile.loadFromFile("images/tile_hidden.png");
        hidden_tile.setTexture(t_hidden_tile);
        t_revealed_tile.loadFromFile("images/tile_revealed.png");
        revealed_tile.setTexture(t_revealed_tile);
        t_flag.loadFromFile("images/flag.png");
        flag.setTexture(t_flag);
        t_mine.loadFromFile("images/mine.png");
        mine.setTexture(t_mine);

        t_one.loadFromFile("images/number_1.png");
        one.setTexture(t_one);
        numbers.push_back(one);
        t_two.loadFromFile("images/number_2.png");
        two.setTexture(t_two);
        numbers.push_back(two);
        t_three.loadFromFile("images/number_3.png");
        three.setTexture(t_three);
        numbers.push_back(three);
        t_four.loadFromFile("images/number_4.png");
        four.setTexture(t_four);
        numbers.push_back(four);
        t_five.loadFromFile("images/number_5.png");
        five.setTexture(t_five);
        numbers.push_back(five);
        t_six.loadFromFile("images/number_6.png");
        six.setTexture(t_six);
        numbers.push_back(six);
        t_seven.loadFromFile("images/number_7.png");
        seven.setTexture(t_seven);
        numbers.push_back(seven);
        t_eight.loadFromFile("images/number_8.png");
        eight.setTexture(t_eight);
        numbers.push_back(eight);





    }




    void renderwindow(int collums, int rows, int mines, string name){
        int width = collums * 32;
        int height = 100 + (rows * 32);

        face_sprite.setPosition((((collums / 2.0) * 32 ) - 32 )    ,   (32 * (rows + 0.5)) );
        debug_button_sprite.setPosition( ((collums * 32) - 304 ), (32 * (rows + 0.5)) );
        playpause_button_sprite.setPosition( (  (collums * 32) - 240)  , (32 *(rows + 0.5)) );
        leaderboard_sprite.setPosition((  (collums * 32) - 176) , (32 *(rows + 0.5)) );


        sf::RenderWindow game_window(sf::VideoMode(width, height), "Game Window", sf::Style::Close);
        Board gameboard;
        Timer testTimer;
        int wincount = 0;
        bool wincondition = false;
        bool lose = false;
        bool paused = false;
        bool debug = false;
        bool reset = false;
        int flag_counter = mines;
        int revealed_nonmines = 0;

        vector<vector<Tile>> game_tiles = initialize(collums, rows, mines);




        //go inside the loop
        //game_window.clear(sf::Color(255, 255, 255));
        while (game_window.isOpen()) {
            //closing and clicking event


            sf::Event event;
            while (game_window.pollEvent(event)) {
                if( lose || wincondition) {
                    reset = true;
                    //wincount = 0;
                    paused = false;
                    debug = true;
                    //flag_counter = mines;
                    if (event.type == sf::Event::Closed) {
                        game_window.close();
                    } else if (event.type == sf::Event::MouseButtonPressed) {
                        // Check if the left mouse button was pressed
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            // Get the position of the mouse click
                            sf::Vector2i mousePosition = sf::Mouse::getPosition(game_window);

                            // Check if the mouse click is within the bounds of the leaderboard button
                            if (leaderboard_sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
                                gameLeaderBoard.renderwindow(collums, rows, name);
                            }
                            if (face_sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
                                face_sprite.setTexture(happy_face_texture);
                                game_tiles = initialize(collums, rows, mines);
                                testTimer.reset_time();
                                wincount = 0;
                                lose = false;
                                wincondition = false;
                                reset = false;
                                int revealed_nonmines = 0;

                            }
                        }
                    }
                }

                if (event.type == sf::Event::Closed) {
                    game_window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    // Check if the left mouse button was pressed
                    if(!reset) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            // Get the position of the mouse click
                            sf::Vector2i mousePosition = sf::Mouse::getPosition(game_window);

                            // Check if the mouse click is within the bounds of the leaderboard button
                            if (leaderboard_sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
                                // Handle the click on the leaderboard button
                                // For example, display the leaderboard or perform any other action
                                //std::cout << "Leaderboard button clicked!" << std::endl;
                                if(!paused){
                                    testTimer.pauseTimer();
                                    playpause_button_sprite.setTexture(play_texture);
                                    paused = true;
                                    cout << "Pause" << endl;
                                    gameboard.rendertiles(collums, rows, game_window);
                                    game_window.display();
                                    gameLeaderBoard.renderwindow(collums, rows, name);
                                    testTimer.unpauseTimer();
                                    playpause_button_sprite.setTexture(pause_texture);
                                    paused = false;
                                    cout << "Unpause" << endl;
                                }
                                else{
                                    gameLeaderBoard.renderwindow(collums, rows, name);
                                }
                            }
                            if (face_sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
                                game_tiles = initialize(collums, rows, mines);
                                testTimer.reset_time();
                                wincount = 0;
                                paused = false;
                                debug = false;
                                flag_counter = mines;
                                lose = false;
                                wincondition = false;
                                reset = false;
                                face_sprite.setTexture(happy_face_texture);
                            }
                            if (debug_button_sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
                                debug = !debug;
                            }
                            if (playpause_button_sprite.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
                                if (paused) {
                                    testTimer.unpauseTimer();
                                    playpause_button_sprite.setTexture(pause_texture);
                                    paused = false;
                                    cout << "Unpause" << endl;
                                } else {
                                    testTimer.pauseTimer();
                                    playpause_button_sprite.setTexture(play_texture);
                                    paused = true;
                                    cout << "Pause" << endl;
                                }
                            } else {
                                int clicked_col = mousePosition.x / 32;
                                int clicked_row = mousePosition.y / 32;
                                if (clicked_row >= 0 && clicked_row < rows && clicked_col >= 0 &&
                                    clicked_col < collums) {
                                    cout << "X:" << clicked_col << " Y:" << clicked_row << endl;
                                    if (!game_tiles[clicked_row][clicked_col].is_revealed) {
                                        revealAdjacentTiles(clicked_row, clicked_col, game_tiles, rows, collums);
                                    }
                                }
                            }

                        }
                        if (event.mouseButton.button == sf::Mouse::Right) {
                            sf::Vector2i mousePosition = sf::Mouse::getPosition(game_window);
                            int clicked_col = mousePosition.x / 32;
                            int clicked_row = mousePosition.y / 32;
                            if (clicked_row >= 0 && clicked_row < rows && clicked_col >= 0 && clicked_col < collums) {
                                cout << "X:" << clicked_col << " Y:" << clicked_row << endl;
                                if (!game_tiles[clicked_row][clicked_col].is_flagged) {
                                    game_tiles[clicked_row][clicked_col].is_flagged = true;
                                    flag_counter--;
                                    //cout << "FLAGED";
                                } else {
                                    game_tiles[clicked_row][clicked_col].is_flagged = false;
                                    flag_counter++;
                                    //cout << "FLAGED";
                                }
                            }
                        }
                    }
                }
            }
            if(wincheck(game_tiles, collums, rows, mines, flag_counter)){
                face_sprite.setTexture(win_face_texture);
                wincondition = true;
                //cout << "WIN" << endl;
                //needs to repeat ONCE
                if (wincount == 0) {
                    gameLeaderBoard.addscore(name, testTimer.getTimeString());
                    cout << "WIN" << endl;
                }
                wincount++;
            }






//

            game_window.clear(sf::Color(255, 255, 255));
            //display tiles
            int x_position = 0;
            int y_position = 0;
            int x_offset = 32;
            int y_offset = 32;
            for (int i = 0; i < rows; i++){
                for(int j = 0; j < collums; j++){
                    //ORDER
                        //Mines
                        //flags
                        //tiles
                        if(game_tiles[i][j].is_revealed){
                            //draw revealed tile

                            revealed_tile.setPosition(x_position, y_position);
                            game_window.draw(revealed_tile);
                            if(!game_tiles[i][j].neighbor_mines == 0) {
                                numbers[game_tiles[i][j].neighbor_mines - 1].setPosition(x_position, y_position);
                                game_window.draw(numbers[game_tiles[i][j].neighbor_mines - 1]);
                            }

                        } else{
                            //draw hidden tile
                            hidden_tile.setPosition(x_position, y_position);
                            game_window.draw(hidden_tile);
                        }
                    if(game_tiles[i][j].is_flagged){
                        //draw flag
                        //cout << "FLAGED";
                        flag.setPosition(x_position, y_position);
                        game_window.draw(flag);
                    }
                    if(game_tiles[i][j].is_mine && game_tiles[i][j].is_revealed){
                        //draw mine AND IS REVEALED
                        lose = true;
                        face_sprite.setTexture(lose_face_texture);
                        mine.setPosition(x_position, y_position);
                        game_window.draw(mine);
                        int x_positionk = 0;
                        int y_positionk = 0;
                        int x_offsetk = 32;
                        int y_offsetk = 32;
                        for (int ik = 0; ik < rows; ik++){
                            for(int jk = 0; jk < collums; jk++){
                                if(game_tiles[ik][jk].is_mine){
                                    mine.setPosition(x_positionk, y_positionk);
                                    game_window.draw(mine);
                                }
                                    x_positionk += x_offsetk;
                                }
                            x_positionk = 0;
                            y_positionk += y_offsetk;
                            }

                        //gameover;

                    }
                    if(game_tiles[i][j].is_mine && debug){
                        mine.setPosition(x_position, y_position);
                        game_window.draw(mine);
                    }

                    x_position += x_offset;
                }
                y_position += y_offset;
                x_position = 0;
            }




            game_window.draw(face_sprite);
            game_window.draw(debug_button_sprite);
            game_window.draw(playpause_button_sprite);
            game_window.draw(leaderboard_sprite);
            //renders tiles
            if(paused) {
                gameboard.rendertiles(collums, rows, game_window);
            }
            //renders timer
            if (!reset){
                render(collums, rows, game_window, testTimer);
            }
            else{
                renderwithoutop(collums, rows, game_window, testTimer);
            }
            rendercounter(collums, rows, game_window, flag_counter);

            game_window.display();

        }
    }


};



