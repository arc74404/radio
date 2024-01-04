
#include "radio/music_player.hpp"

int
main(int argc, char* argv[])
{
    MusicPlayer player;

    std::string d = "C:/Users/arsbo/source/repos/radio/music/";

    player.setDirectory(d);

    player.play();

    std::cout << "end";

    return 0;
}