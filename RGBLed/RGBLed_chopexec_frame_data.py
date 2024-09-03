def getPixelFormatMultiplier():
	return 3 if parent().par.Rgba else 1

def pack_values(values, bits_per_value, upper_limit):
	packed_data = bytearray()
	temp_value = 0
	bits_filled = 0

	# Ensure max 8-bit value does not exceed upper_limit
	max_allowed_value = min(upper_limit, 255)

	for value in values:
		temp_value = (temp_value << bits_per_value) | value
		bits_filled += bits_per_value

		while bits_filled >= 8:
			bits_filled -= 8
			packed_byte = (temp_value >> bits_filled) & 0xFF
			if packed_byte > max_allowed_value:
				packed_byte = max_allowed_value
			packed_data.append(packed_byte)

	if bits_filled > 0:
		packed_byte = (temp_value << (8 - bits_filled)) & 0xFF
		if packed_byte > max_allowed_value:
			packed_byte = max_allowed_value
		packed_data.append(packed_byte)

	return packed_data

def send_frame_data(frame_data):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns
	rgb_resolution = parent().par.Colordepth
	amt_of_devices = parent().par.Amtofdevices

	pixel_format_multiplier = getPixelFormatMultiplier()

	if len(frame_data) != panel_rows * panel_cols * pixel_format_multiplier:
		raise ValueError("Frame data is not enough.")

	data = bytearray()

	try:
		object_marker = int('0x' + parent().par.Objectmarker, 16)
		frame_marker = int('0x' + parent().par.Framemarker, 16)

		data.append(object_marker) # addressing arduino object
		data.append(frame_marker) # addressing arduino object functionality

	except ValueError as e:
		print(f"Error converting markers to int: {e}")
		return

	try:
		upper_limit = 255 - amt_of_devices
		data += pack_values(frame_data, rgb_resolution, upper_limit)

		sent_bytes = parent().par.Globalserial.eval().sendBytes(data)
		print(f"Data successfully sent. Length: {sent_bytes}")

	except Exception as e:
		print(f"Error sending frame data: {e}")

	return

def onValueChange(channel, sampleIndex, val, prev):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns
	rgb_resolution = parent().par.Colordepth
	pixel_format_multiplier = getPixelFormatMultiplier()

	img_data = op('img_data')
	frame_data = []

	for row in range(panel_rows):
		for col in range(panel_cols):
			max_value = (1 << rgb_resolution) - 1
			idx = (row * panel_cols + col)

			r = int(img_data[0][idx] * max_value)

			if (pixel_format_multiplier == 3):
				a = img_data[3][idx] # no need to adjust the value, since alpha channel is used for brightness

				r = int(img_data[0][idx] * max_value * a) # r = r * a would loose extra color depth
				g = int(img_data[1][idx] * max_value * a)
				b = int(img_data[2][idx] * max_value * a)

				frame_data.extend([r, g, b])
			elif (pixel_format_multiplier == 1):
				frame_data.extend([r])

	send_frame_data(frame_data)

	return
