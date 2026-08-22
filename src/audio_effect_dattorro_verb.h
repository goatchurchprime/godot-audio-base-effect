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

#define CREATE_GETTER_SETTER(CLASS, TYPE, NAME)     \
	TYPE CLASS::get_##NAME() const { return NAME; } \
	void CLASS::set_##NAME(const TYPE p_##NAME) { NAME = p_##NAME; }


namespace godot {

class AudioEffectBase;

// effect instance
class AudioEffectDattorroVerbInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectDattorroVerbInstance, AudioEffectInstance)

private:
	struct sDattorroVerb *reverb_left;
	struct sDattorroVerb *reverb_right;

public:
	AudioEffectDattorroVerbInstance();
	~AudioEffectDattorroVerbInstance() override;

	void reset();

	virtual void _process(const void *src_buffer, AudioFrame *dst_buffer, int frame_count) override;

	float gain = 1.0; // this gain is set by the parent object

	bool stereo = true;
	double dry_wet = 0.2;

	DECLARE_PROPERTY_DEFAULT(double, pre_delay, 0.1)
	DECLARE_PROPERTY_DEFAULT(double, pre_filter, 0.85)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion1, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion2, 0.0625)
	DECLARE_PROPERTY_DEFAULT(double, decay_diffusion, 0.7)
	DECLARE_PROPERTY_DEFAULT(double, decay, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, damping, 0.95)

protected:
	static void _bind_methods();
};

// the wrapper for the effect
class AudioEffectDattorroVerb : public AudioEffect {
	GDCLASS(AudioEffectDattorroVerb, AudioEffect)

	DECLARE_PROPERTY_DEFAULT(float, gain, 1.0)

	DECLARE_PROPERTY_DEFAULT(bool, stereo, true)

	DECLARE_PROPERTY_DEFAULT(double, dry_wet, 0.2)

	DECLARE_PROPERTY_DEFAULT(double, pre_delay, 0.1)
	DECLARE_PROPERTY_DEFAULT(double, pre_filter, 0.85)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion1, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, input_diffusion2, 0.0625)
	DECLARE_PROPERTY_DEFAULT(double, decay_diffusion, 0.7)
	DECLARE_PROPERTY_DEFAULT(double, decay, 0.75)
	DECLARE_PROPERTY_DEFAULT(double, damping, 0.95)

protected:
	static void _bind_methods();

public:
	AudioEffectDattorroVerb() = default;

	Ref<AudioEffectDattorroVerbInstance> instance;

	virtual Ref<AudioEffectInstance> _instantiate() override;
};

}


#endif // AUDIO_EFFECT_DATTORRO_VERB_H
