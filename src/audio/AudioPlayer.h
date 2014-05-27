
#ifndef H_DER_AUDIO_PLAYER_H
#define H_DER_AUDIO_PLAYER_H

namespace der
{

    class AudioPlayer
    {
    public:
        AudioPlayer();
        ~AudioPlayer();

        bool init();

        bool is_initialized() const;

        void play_music(const char * const filepath, bool loop, float volume);

        void set_global_volume(float volume);

    private:
        bool m_initialized;

    };

} // der

#endif // H_DER_AUDIO_PLAYER_H
