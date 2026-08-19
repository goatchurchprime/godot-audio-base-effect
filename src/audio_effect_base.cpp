/**************************************************************************/
/*  audio_effect_opus_chunked.cpp                                              */
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
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED httpTO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "audio_effect_base.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/gd_extension_manager.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void AudioEffectBase::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_audiosamplerate", "audiosamplerate"), &AudioEffectBase::set_audiosamplerate);
    ClassDB::bind_method(D_METHOD("get_audiosamplerate"), &AudioEffectBase::get_audiosamplerate);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "audiosamplerate", PROPERTY_HINT_RANGE, "20,192000,1"), "set_audiosamplerate", "get_audiosamplerate");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "audiosamplesize", PROPERTY_HINT_RANGE, "10,4000,1"), "set_audiosamplesize", "get_audiosamplesize");

}

AudioEffectBase::AudioEffectBase() {
}

AudioEffectBase::~AudioEffectBase() {
};

Ref<AudioEffectInstance> AudioEffectBase::_instantiate() {
    instanceinstantiations += 1;
    // this triggers when re-ordering effects in AudioBus
    //if (instanceinstantiations > 1)
    //    godot::UtilityFunctions::printerr("Warning: more than one AudioEffectOpusChunkedInstance instantiation");
    Ref<AudioEffectBaseInstance> ins;
    ins.instantiate();
    ins->base = Ref<AudioEffectBase>(this);
    return ins;
}

AudioEffectBaseInstance::~AudioEffectBaseInstance() {
    if (!base.is_null()) {
        base->instanceinstantiations -= 1;
        // this triggers when re-ordering effects in AudioBus
        //if (base->instanceinstantiations == 0)
        //    godot::UtilityFunctions::printerr("Warning: unexpected number of AudioEffectOpusChunkedInstance instantiations after destructor");
    }
}

void AudioEffectBaseInstance::_process(const void *src_buffer, AudioFrame *p_dst_frames, int p_frame_count) {
    base->process((const AudioFrame *)src_buffer, p_dst_frames, p_frame_count); 
}

void AudioEffectBase::process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) {
    for (int i = 0; i < p_frame_count; i++) {
        //p_dst_frames[i] = p_src_frames[i];
        p_dst_frames[i].left = p_src_frames[i].right;
        p_dst_frames[i].right = p_src_frames[i].left;
    }
}


