def pack_values(values, bits_per_value):
	packed_data = bytearray()
	temp_value = 0
	bits_filled = 0

	for value in values:
		temp_value = (temp_value << bits_per_value) | value
		bits_filled += bits_per_value

		while bits_filled >= 8:
			bits_filled -= 8
			packed_byte = (temp_value >> bits_filled) & 0xFF
			packed_data.append(packed_byte)

	if bits_filled > 0:
		packed_byte = (temp_value << (8 - bits_filled)) & 0xFF
		packed_data.append(packed_byte)

	return packed_data

def send_frame_data(frame_data):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns
	rgb_resolution = parent().par.Rgbresolution

	if len(frame_data) != panel_rows * panel_cols * 3:
		raise ValueError("Frame data is not enough.")

	data = bytearray()

	data.append(int('0x' + parent().par.Framemarker, 16))
	data += pack_values(frame_data, rgb_resolution)

	op(parent().par.Globalserial).sendBytes(data)

	return

def onValueChange(channel, sampleIndex, val, prev):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns
	rgb_resolution = parent().par.Rgbresolution

	img_data = op('img_data')
	frame_data = []

	for row in range(panel_rows):
		for col in range(panel_cols):
			max_value = (1 << rgb_resolution) - 1
			r = int(img_data[0][(row * panel_cols + col)] * max_value)
			g = int(img_data[1][(row * panel_cols + col)] * max_value)
			b = int(img_data[2][(row * panel_cols + col)] * max_value)

			frame_data.extend([r, g, b])

	send_frame_data(frame_data)

	return
