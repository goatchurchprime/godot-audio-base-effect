extends Control

var dv : AudioEffectDattorroVerb = null
var dvi : AudioEffectDattorroVerbInstance = null

func _ready():
	var idx = AudioServer.get_bus_index($AudioStreamPlayer.bus)
	dv = AudioServer.get_bus_effect(idx, 0)
	dvi = AudioServer.get_bus_effect_instance(idx, 0)
	prints(dv, dvi)
	%HSliderReplayLoop.value = 1.1
	%HSliderDryWet.value_changed.connect(func (v): update_dv())
	%HSliderPreDelay.value_changed.connect(func (v): update_dv())
	%HSliderPreFilter.value_changed.connect(func (v): update_dv())
	%HSliderInputDiffusion1.value_changed.connect(func (v): update_dv())
	%HSliderInputDiffusion2.value_changed.connect(func (v): update_dv())
	%HSliderDecayDiffusion.value_changed.connect(func (v): update_dv())
	%HSliderDamping.value_changed.connect(func (v): update_dv())
	update_dv()

func update_dv():
	dv.dry_wet = %HSliderDryWet.value
	dv.pre_delay = %HSliderPreDelay.value
	dv.pre_filter = %HSliderPreFilter.value
	dv.input_diffusion1 = %HSliderInputDiffusion1.value
	dv.input_diffusion2 = %HSliderInputDiffusion2.value
	dv.decay_diffusion = %HSliderDecayDiffusion.value
	dv.decay = %HSliderDecay.value
	dv.damping = %HSliderDamping.value

func _on_h_slider_replay_loop_value_changed(value):
	$AudioStreamPlayer/Timer.wait_time = value

func _on_h_slider_pitch_value_changed(value):
	$AudioStreamPlayer.pitch_scale = value

func _on_timer_timeout():
	$AudioStreamPlayer.play()

func _on_stereo_button_toggled(toggled_on):
	dv.stereo = toggled_on
