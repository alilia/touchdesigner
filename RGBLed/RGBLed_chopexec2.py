def syncLookupTable():
	data = bytearray()

	data.append(int('0x' + parent().par.Objectmarker, 16))
	data.append(int('0x' + parent().par.Lookuptablemarker, 16))

	for i in range(256):
		data.append(int(op('lookup_data')[0][i]))

	parent().par.Globalserial.eval().sendBytes(data)

	return

def onOnToOff(channel, sampleIndex, val, prev):
	syncLookupTable()
	return
