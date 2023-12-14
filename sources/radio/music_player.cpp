#include "music_player.hpp"

#define MINIMP3_IMPLEMENTATION

#include "minimp3/minimp3.h"

MusicPlayer::MusicPlayer()
{
}

MusicPlayer::MusicPlayer(const std::string& directoryname)
{
    setDirectory(directoryname);
}

MusicPlayer::~MusicPlayer()
{
}

bool
MusicPlayer::is_mp3(const std::string& filename)
{
    if (filename.substr(filename.find_last_of(".") + 1) == "mp3")
    {
        return true;
    }
    return false;
}

void
MusicPlayer::setDirectory(const std::string& directoryname)
{
    path p(directoryname);
    for (auto i = directory_iterator(p); i != directory_iterator(); i++)
    {
        if (!is_directory(i->path()))
        {
            m_filename_list.emplace_back(i->path().filename().string());
        }
        else continue;
    }

    for (int i = 0; i < m_filename_list.size(); ++i)
    {
        std::string fn = directoryname + m_filename_list[i];

        char* f2 = new char(fn.size() + 1);
        strcpy(f2, fn.c_str());

        f2 = "C:/Users/arsbo/source/repos/radio/music/$R7ZU3KX.mp3";

        std::cout << f2 << '\n';

        if (is_mp3(m_filename_list[i]))
        {
            convertMp3ToWav(f2);
            m_filename_list[i][m_filename_list[i].size() - 3] = 'w';
            m_filename_list[i][m_filename_list[i].size() - 1] = 'a';
            m_filename_list[i][m_filename_list[i].size() - 2] = 'v';
        }

        addFile(directoryname + m_filename_list[i]);

        f2 = nullptr;
        // delete f2;
    }
}

void
MusicPlayer::addFile(const std::string& filename)
{
    // sf::Music m;
    // m.openFromFile(filename);
    // m_audio_file.emplace_back(m);
}

void
MusicPlayer::play()
{
}

void
MusicPlayer::convertMp3ToWav(char* filename)
{

    std::cout << "ConvertMp3ToWav start\n";

    // Total audio samples
    uint32_t totalSampleCount = 0;
    // Audio sampling rate
    uint32_t sampleRate = 0;
    // Number of channels
    unsigned int channels = 0;
    int16_t* wavBuffer    = NULL;

    char* in_file = filename;

    wavBuffer =
        DecodeMp3ToBuffer(in_file, &sampleRate, &totalSampleCount, &channels);

    // Save results

    char drive[3];
    char dir[256];
    char fname[256];
    char ext[256];
    char out_file[1024];
    splitpath(in_file, drive, dir, fname, ext);
    sprintf(out_file, "%s%s%s.wav", drive, dir, fname);
    wavWrite_int16(out_file, wavBuffer, sampleRate, totalSampleCount, channels);

    if (wavBuffer)
    {
        std::cout << "free st\n";
        free(wavBuffer);
        std::cout << "free f\n";
    }

    std::cout << "ConvertMp3ToWav finish\n";
}

void
MusicPlayer::wavWrite_int16(char* filename, int16_t* buffer, int sampleRate,
                            uint32_t totalSampleCount, int channels)
{

    std::cout << "wavWrite_int16 start\n";

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
    std::cout << "wavWrite_int16 finish\n";
}

char*
MusicPlayer::getFileBuffer(const char* fname, int* size)
{
    std::cout << "getFileBuffer start\n";
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
    std::cout << "getFileBuffer finish\n";
    return file_buf;
}

int16_t*
MusicPlayer::DecodeMp3ToBuffer(char* filename, uint32_t* sampleRate,
                               uint32_t* totalSampleCount,
                               unsigned int* channels)
{
    std::cout << "DecodeMp3ToBuffer start\n";
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
        std::cout << "DecodeMp3ToBuffer finish\n";
        return music_buf;
    }
    if (music_buf) free(music_buf);

    std::cout << "DecodeMp3ToBuffer finish\n";
    return 0;
}

void
MusicPlayer::splitpath(const char* path, char* drv, char* dir, char* name,
                       char* ext)
{
    std::cout << "splitpath start\n";
    const char* end;
    const char* p;
    const char* s;
    if (path[0] && path[1] == ':')
    {
        if (drv)
        {
            *drv++ = *path++;
            *drv++ = *path++;
            *drv   = '\0';
        }
    }
    else if (drv) *drv = '\0';
    for (end = path; *end && *end != ':';)
        end++;
    for (p = end; p > path && *--p != '\\' && *p != '/';)
        if (*p == '.')
        {
            end = p;
            break;
        }
    if (ext)
        for (s = end; (*ext = *s++);)
            ext++;
    for (p = end; p > path;)
        if (*--p == '\\' || *p == '/')
        {
            p++;
            break;
        }
    if (name)
    {
        for (s = p; s < end;)
            *name++ = *s++;
        *name = '\0';
    }
    if (dir)
    {
        for (s = path; s < p;)
            *dir++ = *s++;
        *dir = '\0';
    }
    std::cout << "splitpath finish\n";
}
