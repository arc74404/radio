#include "music_player.hpp"

#define MINIMP3_IMPLEMENTATION

#include "minimp3/minimp3.h"


MusicPlayer::MusicPlayer()
{
}

MusicPlayer::MusicPlayer(const std::string& filename, bool is_mp3)
{
    setFile(filename, is_mp3);
}

MusicPlayer::~MusicPlayer()
{
}

void
MusicPlayer::setFile(const std::string& filename, bool is_mp3)
{
    if (is_mp3)
    {
        convertMp3ToWav(filename);
    }

    m_audio_file.openFromFile(filename);
}

void
MusicPlayer::play()
{
}

void
MusicPlayer::convertMp3ToWav(const std::string& filename)
{
}

void
MusicPlayer::wavWrite_int16(char* filename, int16_t* buffer, int sampleRate,
                            uint32_t totalSampleCount, int channels)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        printf("File open failed.\n");
        return;
    }
    // Fixed written buffer length
    totalSampleCount *= sizeof(int16_t) * channels;
    int nbit             = 16;
    int FORMAT_PCM       = 1;
    int nbyte            = nbit / 8;
    char text[4]         = {'R', 'I', 'F', 'F'};
    uint32_t long_number = 36 + totalSampleCount;
    fwrite(text, 1, 4, fp);
    fwrite(&long_number, 4, 1, fp);
    text[0] = 'W';
    text[1] = 'A';
    text[2] = 'V';
    text[3] = 'E';
    fwrite(text, 1, 4, fp);
    text[0] = 'f';
    text[1] = 'm';
    text[2] = 't';
    text[3] = ' ';
    fwrite(text, 1, 4, fp);

    long_number = 16;
    fwrite(&long_number, 4, 1, fp);
    int16_t short_number = FORMAT_PCM; // Default audio format
    fwrite(&short_number, 2, 1, fp);
    short_number = channels; // Number of audio channels
    fwrite(&short_number, 2, 1, fp);
    long_number = sampleRate; // sampling rate
    fwrite(&long_number, 4, 1, fp);
    long_number = sampleRate * nbyte; // Bit rate
    fwrite(&long_number, 4, 1, fp);
    short_number = nbyte; // Block alignment
    fwrite(&short_number, 2, 1, fp);
    short_number = nbit; // Sampling accuracy
    fwrite(&short_number, 2, 1, fp);
    char data[4] = {'d', 'a', 't', 'a'};
    fwrite(data, 1, 4, fp);
    long_number = totalSampleCount;
    fwrite(&long_number, 4, 1, fp);
    fwrite(buffer, totalSampleCount, 1, fp);
    fclose(fp);
}

char*
MusicPlayer::getFileBuffer(const char* fname, int* size)
{
    FILE* fd = fopen(fname, "rb");
    if (fd == 0) return 0;
    struct stat st;
    char* file_buf = 0;
    if (fstat(fileno(fd), &st) < 0) goto doexit;
    file_buf = (char*)malloc(st.st_size + 1);
    if (file_buf != NULL)
    {
        if (fread(file_buf, st.st_size, 1, fd) < 1)
        {
            fclose(fd);
            return 0;
        }
        file_buf[st.st_size] = 0;
    }

    if (size) *size = st.st_size;
doexit:
    fclose(fd);
    return file_buf;
}
int16_t*
MusicPlayer::DecodeMp3ToBuffer(char* filename, uint32_t* sampleRate,
                               uint32_t* totalSampleCount,
                               unsigned int* channels)
{
    int music_size    = 0;
    int alloc_samples = 1024 * 1024, num_samples = 0;
    int16_t* music_buf = (int16_t*)malloc(alloc_samples * 2 * 2);
    unsigned char* file_buf =
        (unsigned char*)getFileBuffer(filename, &music_size);
    if (file_buf != NULL)
    {
        unsigned char* buf = file_buf;
        mp3dec_frame_info_t info;
        mp3dec_t dec;

        mp3dec_init(&dec);
        for (;;)
        {
            int16_t frame_buf[2 * 1152];
            int samples =
                mp3dec_decode_frame(&dec, buf, music_size, frame_buf, &info);
            if (alloc_samples < (num_samples + samples))
            {
                alloc_samples *= 2;
                int16_t* tmp = (int16_t*)realloc(music_buf, alloc_samples * 2 *
                                                                info.channels);
                if (tmp) music_buf = tmp;
            }
            if (music_buf)
                memcpy(music_buf + num_samples * info.channels, frame_buf,
                       samples * info.channels * 2);
            num_samples += samples;
            if (info.frame_bytes <= 0 || music_size <= (info.frame_bytes + 4))
                break;
            buf += info.frame_bytes;
            music_size -= info.frame_bytes;
        }
        if (alloc_samples > num_samples)
        {
            int16_t* tmp =
                (int16_t*)realloc(music_buf, num_samples * 2 * info.channels);
            if (tmp) music_buf = tmp;
        }

        if (sampleRate) *sampleRate = info.hz;
        if (channels) *channels = info.channels;
        if (num_samples) *totalSampleCount = num_samples;

        free(file_buf);
        return music_buf;
    }
    if (music_buf) free(music_buf);
    return 0;
}

void
MusicPlayer::splitpath(const char* path, char* drv, char* dir, char* name,
                       char* ext)
{
}
