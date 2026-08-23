extends Control

var dv : AudioEffectDattorroVerb = null
var dvi : AudioEffectDattorroVerbInstance = null

func _ready():
	var idx = AudioServer.get_bus_index($AudioStreamPlayer.bus)
	dv = AudioServer.get_bus_effect(idx, 0)
	dvi = AudioServer.get_bus_effect_instance(idx, 0)
	prints(dv, dvi)
	%HSliderReplayLoop.value = 1.1
	%HSliderDryWet.value = 0.5

func _on_h_slider_replay_loop_value_changed(value):
	$AudioStreamPlayer/Timer.wait_time = value

func _on_timer_timeout():
	$AudioStreamPlayer.play()

func _on_h_slider_dry_wet_value_changed(value):
	dv.dry_wet = value

	print("update ", value)
