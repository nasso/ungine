#ifndef AUDIOCLIP_HPP_
#define AUDIOCLIP_HPP_

#include "ige/plugin/audio/AudioBuffer.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <libnyquist/Decoders.h>

namespace ige::plugin::audio {

class AudioClip {
public:
    AudioClip(const std::string& path);
    AudioClip(const AudioClip& other) = delete;
    AudioClip(AudioClip&&);

    std::vector<float>& get_samples();
    AudioBuffer& get_audio_buffer();

    AudioClip& operator=(AudioClip&& other);

private:
    nqr::AudioData m_audio_data;
    AudioBuffer m_buffer;
    bool m_moved;

    ALenum find_sample_mode(const nqr::AudioData&);
};

}

#endif /* !AUDIOCLIP_HPP_ */
