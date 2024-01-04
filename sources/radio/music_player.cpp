#include "music_player.hpp"

#define MINIMP3_IMPLEMENTATION

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
    int index = 0;
    for (auto i = directory_iterator(p); i != directory_iterator(); i++)
    {

        if (!is_directory(i->path()))
        {
            m_filename_list.emplace_back(i->path().filename().string());
            m_filename_list[index] = directoryname + m_filename_list[index];
        }
        ++index;
    }

    for (int i = 0; i < m_filename_list.size(); ++i)
    {
        std::string s_fn = m_filename_list[i];

        if (is_mp3(s_fn))
        {
            convertMp3ToWav(s_fn);
        }

        addFile(s_fn);
    }
}

void
MusicPlayer::addFile(const std::string& filename)
{
    std::cout << "ConstructorMusic start\n";

    MusicPtr music_ptr = std::make_shared<sf::Music>();

    music_ptr->openFromFile(filename);

    sf::RenderWindow window;

    std::cout << "ConstructorMusic finish\n";

    std::cout << "openFromFile start\n";

    audiofile_list.emplace_back(music_ptr);

    std::cout << "openFromFile finish\n";
}

void
MusicPlayer::play()
{

    for (int i = 0; i < audiofile_list.size(); ++i)
    {
        MusicPtr file = audiofile_list[i];
        file->play();

        std::string com;
        while (file->getStatus() == sf::SoundSource::Status::Playing ||
               file->getStatus() == sf::SoundSource::Status::Paused)
        {
            std::getline(std::cin, com);
            if (com == "next")
            {
                file->stop();
                if (i == audiofile_list.size() - 1) i = 0;
            }
            if (com == "back")
            {
                file->stop();
                if (i == 0) i = audiofile_list.size() - 1;
                else i -= 2;
            }
            else if (com == "pause")
            {
                file->pause();
            }
            else if (com == "playing")
            {
                file->play();
            }
            else if (com == "finish")
            {
                file->stop();
                return;
            }
            else
            {
                std::cout << "the command was not recognized\n";
            }
        }
    }
}

void
MusicPlayer::convertMp3ToWav(std::string& filename)
{

    std::cout << "ConvertMp3ToWav start\n";

    std::string filename2 = filename;

    filename2[filename.size() - 3] = 'w';
    filename2[filename.size() - 2] = 'a';
    filename2[filename.size() - 1] = 'v';

    std::string command;
    command = "ffmpeg -i \"" + filename + '\"' + " -acodec pcm_u8 -ar 17000 ";

    command = command + '\"' + filename2 + '\"';

    system(command.c_str());

    std::remove(filename.c_str());

    filename = filename2;

    std::cout << "ConvertMp3ToWav finish\n";
}
