#ifndef MUSIC_PLAYER_HPP
#define MUSIC_PLAYER_HPP

#include <SFML/Audio.hpp>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>



class MusicPlayer
{
public:
    MusicPlayer();

    MusicPlayer(const std::string& filename, bool is_mp3);

    ~MusicPlayer();

    void setFile(const std::string& filename, bool is_mp3);

    void play();

private:
    void convertMp3ToWav(const std::string& filename);

    void wavWrite_int16(char* filename, int16_t* buffer, int sampleRate,
                        uint32_t totalSampleCount, int channels = 1);

    char* getFileBuffer(const char* fname, int* size);

    int16_t* DecodeMp3ToBuffer(char* filename, uint32_t* sampleRate,
                               uint32_t* totalSampleCount,
                               unsigned int* channels);

    void splitpath(const char* path, char* drv, char* dir, char* name,
                   char* ext);

    sf::Music m_audio_file;

    float m_volume;

    float m_time;
};

#endif // MUSIC_PLAYER_HPP