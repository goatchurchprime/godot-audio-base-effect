/**************************************************************************/
/*  audio_effect_opus_chunked.h                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef AUDIO_EFFECT_BASE_H
#define AUDIO_EFFECT_BASE_H

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>
#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/audio_stream_playback_resampled.hpp>

namespace godot {

class AudioEffectBase;

class AudioEffectBaseInstance : public AudioEffectInstance {
    GDCLASS(AudioEffectBaseInstance, AudioEffectInstance);
    friend class AudioEffectBase;
    Ref<AudioEffectBase> base;

protected:
    static void _bind_methods() {;};

public:
    virtual void _process(const void *src_buffer, AudioFrame *p_dst_frames, int p_frame_count) override; 
    virtual bool _process_silence() const override { return true; }

    ~AudioEffectBaseInstance();
};

class AudioEffectBase : public AudioEffect {
    GDCLASS(AudioEffectBase, AudioEffect)
    friend class AudioEffectBaseInstance;

    int audiosamplerate = 44100;

    int instanceinstantiations = 0;
    void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);

protected:
    static void _bind_methods();
    
public:
    virtual Ref<AudioEffectInstance> _instantiate() override;

    void set_audiosamplerate(int laudiosamplerate) { audiosamplerate = laudiosamplerate; };
    int get_audiosamplerate() { return audiosamplerate; };

    AudioEffectBase();
    ~AudioEffectBase();
};

}

#endif // AUDIO_EFFECT_BASE_H
