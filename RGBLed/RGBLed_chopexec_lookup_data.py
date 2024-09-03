def syncLookupTable():
	data = bytearray()
	upper_limit = 255 - parent().par.Amtofdevices

	try:
		object_marker = int('0x' + parent().par.Objectmarker, 16)
		lookup_table_marker = int('0x' + parent().par.Lookuptablemarker, 16)

		data.append(object_marker)
		data.append(lookup_table_marker)

	except ValueError as e:
		print(f"Error converting markers to int: {e}")
		return

	try:
		for i in range(256):
			a = op('lookup_data')[3][i]

			data.append(int(op('lookup_data')[0][i] * upper_limit * a))
			data.append(int(op('lookup_data')[1][i] * upper_limit * a))
			data.append(int(op('lookup_data')[2][i] * upper_limit * a))

		sent_bytes = parent().par.Globalserial.eval().sendBytes(data)
		print(f"Data successfully sent. Length: {sent_bytes}")

	except Exception as e:
		print(f"Error in syncLookupTable: {e}")

	return

def onOnToOff(channel, sampleIndex, val, prev):
	syncLookupTable()
	return
