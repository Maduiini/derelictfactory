
#ifndef H_DER_AUDIO_PLAYER_H
#define H_DER_AUDIO_PLAYER_H

#include "../Types.h"

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

        void stop_music();

        void set_global_volume(float volume);

        void set_music_volume(float volume);

    private:
        bool m_initialized;

        uint32_t m_music_stream;

    };

} // der

#endif // H_DER_AUDIO_PLAYER_H
