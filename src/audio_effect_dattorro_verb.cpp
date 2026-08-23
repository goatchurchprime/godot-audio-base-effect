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

#include <cstdio>

#include "audio_effect_dattorro_verb.h"

#include "dattorro-verb/verb.h"

using namespace godot;

AudioEffectDattorroVerbInstance::AudioEffectDattorroVerbInstance() {
	reverb_left = DattorroVerb_create();
	reverb_right = DattorroVerb_create();
}



AudioEffectDattorroVerbInstance::~AudioEffectDattorroVerbInstance() {
	if (reverb_left) {
		DattorroVerb_delete(reverb_left);
		reverb_left = nullptr; // Set to nullptr after deletion (defensive programming)
	}
	if (reverb_right) {
		DattorroVerb_delete(reverb_right);
		reverb_right = nullptr; // Set to nullptr after deletion (defensive programming)
	}
}

double AudioEffectDattorroVerbInstance::get_input_diffusion2() const { return input_diffusion2; }
void AudioEffectDattorroVerbInstance::set_input_diffusion2(const double p_input_diffusion2) {
	input_diffusion2 = p_input_diffusion2;
	DattorroVerb_setInputDiffusion2(reverb_left, input_diffusion2);
	DattorroVerb_setInputDiffusion2(reverb_right, input_diffusion2);
}
double AudioEffectDattorroVerbInstance::get_decay_diffusion() const { return decay_diffusion; }
void AudioEffectDattorroVerbInstance::set_decay_diffusion(const double p_decay_diffusion) {
	decay_diffusion = p_decay_diffusion;
	DattorroVerb_setDecayDiffusion(reverb_left, decay_diffusion);
	DattorroVerb_setDecayDiffusion(reverb_right, decay_diffusion);
}
double AudioEffectDattorroVerbInstance::get_decay() const { return decay; }
void AudioEffectDattorroVerbInstance::set_decay(const double p_decay) {
	decay = p_decay;
	DattorroVerb_setDecay(reverb_left, decay);
	DattorroVerb_setDecay(reverb_right, decay);
}

void AudioEffectDattorroVerbInstance::reset() {
}

void AudioEffectDattorroVerbInstance::_bind_methods() {
}

void AudioEffectDattorroVerbInstance::_process(const void *src_buffer, AudioFrame *dst_buffer, int frame_count) {
	const AudioFrame *p_src_frames = static_cast<const AudioFrame *>(src_buffer);

	dry_wet = CLAMP(base->dry_wet, 0.0, 1.0);
	if (pre_delay != base->pre_delay) {
		pre_delay = base->pre_delay;
		DattorroVerb_setPreDelay(reverb_left, pre_delay);
		DattorroVerb_setPreDelay(reverb_right, pre_delay);
	}
	if (pre_filter != base->pre_filter) {
		pre_filter = base->pre_filter;
		DattorroVerb_setPreFilter(reverb_left, pre_filter);
		DattorroVerb_setPreFilter(reverb_right, pre_filter);
	}
	if (damping != base->damping) {
		damping = base->damping;
		DattorroVerb_setDamping(reverb_left, damping);
		DattorroVerb_setDamping(reverb_right, damping);
	}
	if (input_diffusion1 != base->input_diffusion1) {
		input_diffusion1 = base->input_diffusion1;
		DattorroVerb_setInputDiffusion1(reverb_left, input_diffusion1);
		DattorroVerb_setInputDiffusion1(reverb_right, input_diffusion1);
	}
	

	// should do mono actually and mix the stereo bits
	for (int i = 0; i < frame_count; i++) {
		// get input
		float input_left = p_src_frames[i].left;
		float input_right = p_src_frames[i].right;

		// process reverb
		DattorroVerb_process(reverb_left, input_left);
		DattorroVerb_process(reverb_right, input_right);

		// get output	
		float reverb_output_left = DattorroVerb_getLeft(reverb_left);
		float reverb_output_right = DattorroVerb_getLeft(reverb_right);

		// mix dry and wet signals
		float output_left = (1.0 - dry_wet) * input_left + reverb_output_left * dry_wet;
		float output_right = (1.0 - dry_wet) * input_right + reverb_output_right * dry_wet;

		// return final mix
		dst_buffer[i].left = output_left;
		dst_buffer[i].right = output_right;
	}
}

#pragma endregion

#pragma region WRAPPER

void AudioEffectDattorroVerb::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_dry_wet", "dry_wet"), &AudioEffectDattorroVerb::set_dry_wet);
    ClassDB::bind_method(D_METHOD("get_dry_wet"), &AudioEffectDattorroVerb::get_dry_wet);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dry_wet", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_dry_wet", "get_dry_wet");

    ClassDB::bind_method(D_METHOD("set_pre_delay", "pre_delay"), &AudioEffectDattorroVerb::set_pre_delay);
    ClassDB::bind_method(D_METHOD("get_pre_delay"), &AudioEffectDattorroVerb::get_pre_delay);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pre_delay", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_pre_delay", "get_pre_delay");

    ClassDB::bind_method(D_METHOD("set_damping", "damping"), &AudioEffectDattorroVerb::set_damping);
    ClassDB::bind_method(D_METHOD("get_damping"), &AudioEffectDattorroVerb::get_damping);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damping", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_damping", "get_damping");

    ClassDB::bind_method(D_METHOD("set_pre_filter", "pre_filter"), &AudioEffectDattorroVerb::set_pre_filter);
    ClassDB::bind_method(D_METHOD("get_pre_filter"), &AudioEffectDattorroVerb::get_pre_filter);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pre_filter", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_pre_filter", "get_pre_filter");

    ClassDB::bind_method(D_METHOD("set_input_diffusion1", "input_diffusion1"), &AudioEffectDattorroVerb::set_input_diffusion1);
    ClassDB::bind_method(D_METHOD("get_input_diffusion1"), &AudioEffectDattorroVerb::get_input_diffusion1);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "input_diffusion1", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_input_diffusion1", "get_input_diffusion1");

	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, input_diffusion2)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, decay)
	CREATE_VAR_BINDINGS(AudioEffectDattorroVerb, Variant::FLOAT, decay_diffusion)
}
double AudioEffectDattorroVerb::get_input_diffusion2() const { return input_diffusion2; }
void AudioEffectDattorroVerb::set_input_diffusion2(const double p_input_diffusion2) {
	input_diffusion2 = p_input_diffusion2;
	if (instance.is_valid()) {
		instance->set_input_diffusion2(input_diffusion2);
	}
}
double AudioEffectDattorroVerb::get_decay_diffusion() const { return decay_diffusion; }
void AudioEffectDattorroVerb::set_decay_diffusion(const double p_decay_diffusion) {
	decay_diffusion = p_decay_diffusion;
	if (instance.is_valid()) {
		instance->set_decay_diffusion(decay_diffusion);
	}
}
double AudioEffectDattorroVerb::get_decay() const { return decay; }
void AudioEffectDattorroVerb::set_decay(const double p_decay) {
	decay = p_decay;
	if (instance.is_valid()) {
		instance->set_decay(decay);
	}
}

//
//
//
//

Ref<AudioEffectInstance> AudioEffectDattorroVerb::_instantiate() {

    //Ref<AudioEffectBaseInstance> ins;
    //ins.instantiate();
	instance.instantiate(); // create instnce
    instance->base = Ref<AudioEffectDattorroVerb>(this);
    return instance;
}


