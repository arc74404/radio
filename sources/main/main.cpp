
#include "radio/music_player.hpp"

int
main(int argc, char* argv[])
{
    MusicPlayer player;

    char* d = "C:/Users/arsbo/source/repos/radio/music/";

    player.setDirectory(d);

    std::cout << "end";

    return 0;
}