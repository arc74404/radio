#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include <boost/filesystem.hpp>

#include <iostream>

#include <string>

#include<shared_mutex>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "crow.h"

using boost::filesystem::directory_iterator;
using boost::filesystem::is_directory;
using boost::filesystem::path;

typedef std::shared_ptr<sf::Music> MusicPtr;

class MusicPlayer
{
public:
    MusicPlayer();

    MusicPlayer(const std::string& directoryname);

    ~MusicPlayer();

    void play();

    void setDirectory(const std::string& directoryname);

private:
    bool is_mp3(const std::string& filename);

    void addFile(const std::string& filename);

    void convertMp3ToWav(std::string& filename);

    std::vector<std::string> m_filename_list;

    std::vector<MusicPtr> audiofile_list;

    float m_volume;

    float m_time;
};

#endif // MUSIC_PLAYER_HPP