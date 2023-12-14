#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include <SFML/Audio.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

using boost::filesystem::directory_iterator;
using boost::filesystem::is_directory;
using boost::filesystem::path;

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

    void convertMp3ToWav(char* filename);

    void wavWrite_int16(char* filename, int16_t* buffer, int sampleRate,
                        uint32_t totalSampleCount, int channels = 1);

    char* getFileBuffer(const char* fname, int* size);

    int16_t* DecodeMp3ToBuffer(char* filename, uint32_t* sampleRate,
                               uint32_t* totalSampleCount,
                               unsigned int* channels);

    void splitpath(const char* path, char* drv, char* dir, char* name,
                   char* ext);

    std::vector<std::string> m_filename_list;

    std::vector<sf::Music> m_audio_file;

    float m_volume;

    float m_time;
};

#endif // MUSIC_PLAYER_HPP