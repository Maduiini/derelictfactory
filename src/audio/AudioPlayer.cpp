
#include "AudioPlayer.h"

#include "../Log.h"

#include <bass.h>

namespace der
{

    AudioPlayer::AudioPlayer()
        : m_initialized(false)
    { }

    AudioPlayer::~AudioPlayer()
    {
        if (m_initialized)
            BASS_Free();
    }

    bool AudioPlayer::init()
    {
        const int device = -1; // Default sound device
        const int freq = 44100; // Sample rate (Hz)
        if (BASS_Init(device, freq, 0, 0, NULL))
            m_initialized = true;

        return m_initialized;
    }

    bool AudioPlayer::is_initialized() const
    {
        return m_initialized;
    }

    void AudioPlayer::play_music(const char * const filepath, bool loop, float volume)
    {
        if (!m_initialized)
            return;

        HSTREAM channel = BASS_StreamCreateFile(FALSE, filepath, 0, 0, 0);
        BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);
        if (loop)
            BASS_ChannelFlags(channel, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);

        if (!BASS_ChannelPlay(channel, FALSE))
            log::error("Failed to play music: %", filepath);
    }

    void AudioPlayer::set_global_volume(float volume)
    {
        if (!m_initialized)
            return;

        BASS_SetVolume(volume);
    }

} // der
