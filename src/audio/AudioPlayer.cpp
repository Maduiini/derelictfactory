
#include "AudioPlayer.h"

#include "../Log.h"

#include <bass.h>

namespace der
{

    AudioPlayer::AudioPlayer()
        : m_initialized(false)
        , m_music_stream(0)
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
        else
            m_music_stream = channel;
    }

    void AudioPlayer::stop_music()
    {
        if (m_music_stream)
            BASS_ChannelStop(m_music_stream);
    }

    void AudioPlayer::set_global_volume(float volume)
    {
        if (!m_initialized)
            return;

        BASS_SetVolume(volume);
    }

    void AudioPlayer::set_music_volume(float volume)
    {
        if (m_music_stream)
            BASS_ChannelSetAttribute(m_music_stream, BASS_ATTRIB_VOL, volume);
    }

} // der
