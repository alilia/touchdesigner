def pack_6bit_values(values):
	packed_data = bytearray()
	asd = []

	# Assuming 'values' is a list of integers, each with a max value of 63 (6-bit)
	for i in range(0, len(values), 4):
		# Get the next 4 values, or pad with 0 if fewer than 4 remain
		v0 = values[i] if i < len(values) else 0
		v1 = values[i+1] if i+1 < len(values) else 0
		v2 = values[i+2] if i+2 < len(values) else 0
		v3 = values[i+3] if i+3 < len(values) else 0

		print("chunks", v0, v1, v2, v3)

		# Pack 4 6-bit values into 3 bytes
		byte1 = (v0 << 2) | (v1 >> 4)
		byte2 = ((v1 & 0xF) << 4) | (v2 >> 2)
		byte3 = ((v2 & 0x3) << 6) | v3

		asd.append(byte1)
		asd.append(byte2)
		asd.append(byte3)

		print("packed: ", byte1, byte2, byte3)
		print()

		packed_data.extend([byte1, byte2, byte3])

	print(asd)

	return packed_data

def send_frame_data(frame_data):
	print(frame_data)
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns

	if len(frame_data) != panel_rows * panel_cols * 3: # 3 is for R, G and B
		raise ValueError("Frame data is not enough.")

	data = bytearray()

	data.append(int('0x' + parent().par.Framemarker, 16))
	data += pack_6bit_values(frame_data)

	print("sent: ", op(parent().par.Globalserial).sendBytes(data))

	return

def onValueChange(channel, sampleIndex, val, prev):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns

	img_data = op('img_data')
	frame_data = []

	for row in range(panel_rows):
		for col in range(panel_cols):
			r = int(img_data[0][(row * panel_cols + col)] * 63)
			g = int(img_data[1][(row * panel_cols + col)] * 63)
			b = int(img_data[2][(row * panel_cols + col)] * 63)

			print("r, g, b: ", r, g, b)

			frame_data.extend([r, g, b])

	send_frame_data(frame_data)

	return
