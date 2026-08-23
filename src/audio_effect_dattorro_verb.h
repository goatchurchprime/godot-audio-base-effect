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

#ifndef AUDIO_EFFECT_DATTORRO_VERB_H
#define AUDIO_EFFECT_DATTORRO_VERB_H

#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

extern "C" {
#include "dattorro-verb/verb.h"
}

#define DECLARE_PROPERTY_DEFAULT(TYPE, NAME, DEFAULT) \
                                                      \
public:                                               \
	void set_##NAME(const TYPE p_##NAME);             \
	TYPE get_##NAME() const;                          \
                                                      \
private:                                              \
	TYPE NAME = DEFAULT;

#define CREATE_VAR_BINDINGS(CLASS, TYPE, NAME)                                    \
	ClassDB::bind_method(D_METHOD("get_" #NAME), &CLASS::get_##NAME);             \
	ClassDB::bind_method(D_METHOD("set_" #NAME, "p_" #NAME), &CLASS::set_##NAME); \
	ADD_PROPERTY(PropertyInfo(Variant::TYPE, #NAME), "set_" #NAME, "get_" #NAME);



namespace godot {

class AudioEffectDattorroVerbInstance;

// the wrapper for the effect
class AudioEffectDattorroVerb : public AudioEffect {
	GDCLASS(AudioEffectDattorroVerb, AudioEffect)


	double dry_wet = 0.21;
	double pre_delay = 0.1;
	double pre_filter = 0.85;
	double damping = 0.95;
	double input_diffusion1 = 0.75;

	void set_dry_wet(float ldry_wet) { dry_wet = ldry_wet; };
	float get_dry_wet() const { return dry_wet; };
	void set_pre_delay(float lpre_delay) { pre_delay = lpre_delay; };
	float get_pre_delay() const { return pre_delay; };
	void set_pre_filter(float lpre_filter) { pre_filter = lpre_filter; };
	float get_pre_filter() const { return pre_filter; };
	void set_damping(float ldamping) { damping = ldamping; };
	float get_damping() const { return damping; };
	void set_input_diffusion1(float linput_diffusion1) { input_diffusion1 = linput_diffusion1; };
	float get_input_diffusion1() const { return input_diffusion1; };
    

	DECLARE_PROPERTY_DEFAULT(double, input_diffusion2, 0.0625)
	DECLARE_PROPERTY_DEFAULT(double, decay_diffusion, 0.7)
	DECLARE_PROPERTY_DEFAULT(double, decay, 0.75)

protected:
	static void _bind_methods();

public:
	AudioEffectDattorroVerb() = default;
    friend class AudioEffectDattorroVerbInstance;

	Ref<AudioEffectDattorroVerbInstance> instance;

	virtual Ref<AudioEffectInstance> _instantiate() override;
};

// effect instance
class AudioEffectDattorroVerbInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectDattorroVerbInstance, AudioEffectInstance)
    friend class AudioEffectDattorroVerb;

private:
	struct sDattorroVerb *reverb_left;
	struct sDattorroVerb *reverb_right;
	Ref<AudioEffectDattorroVerb> base;

public:
	AudioEffectDattorroVerbInstance();
	~AudioEffectDattorroVerbInstance() override;

	void reset();

	virtual void _process(const void *src_buffer, AudioFrame *dst_buffer, int frame_count) override;

	double dry_wet = 0.2;
	double pre_delay = 0.1;
	double pre_filter = 0.85;
	double damping = 0.95;
	double input_diffusion1 = 0.75;

	DECLARE_PROPERTY_DEFAULT(double, input_diffusion2, 0.0625)
	DECLARE_PROPERTY_DEFAULT(double, decay_diffusion, 0.7)
	DECLARE_PROPERTY_DEFAULT(double, decay, 0.75)

protected:
	static void _bind_methods();
};


}


#endif // AUDIO_EFFECT_DATTORRO_VERB_H
