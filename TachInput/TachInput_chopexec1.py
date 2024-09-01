def onValueChange(channel, sampleIndex, val, prev):
	data = bytearray()
	data.append(int('0x' + parent().par.Basespeedmarker, 16))
	data.extend([int(val)])

	parent().par.Globalserial.eval().sendBytes(data)

	return
