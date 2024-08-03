def onValueChange(channel, sampleIndex, val, prev):
	op('fan_speed').par.value0 = val
	return
