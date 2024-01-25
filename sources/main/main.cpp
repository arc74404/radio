#include "crow.h"
// #include "crow_all.h"

#include "radio/music_player.hpp"

int
main()
{
    crow::SimpleApp app;

    // define your endpoint at the root directory

    CROW_ROUTE(app, "/<string>")
    ([](std::string name) { //
        sf::Music music;
        music.openFromFile("C:/Users/arsbo/source/repos/radio/bin/templates/"
                           "CWT_-_Derby_Day_74735148.wav");
        music.play();

        auto file = crow::mustache::load_text("t.html");

        return file; //
    });

    
    

    app.port(18080).multithreaded().run();
}