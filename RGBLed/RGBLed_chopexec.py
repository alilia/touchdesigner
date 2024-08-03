def send_frame_data(frame_data):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns

	if len(frame_data) != panel_rows * panel_cols * 3: # 3 is for R, G and B
		raise ValueError("Frame data is not enough.")

	data = bytearray()

	data.append(int('0x' + parent().par.Framemarker, 16))

	for row in range(panel_rows):
		row_data = frame_data[row * (panel_cols * 3) : (row + 1) * (panel_cols * 3)]
		data.extend(row_data)

	op(parent().par.Globalserial).sendBytes(data)

	return

def onValueChange(channel, sampleIndex, val, prev):
	panel_rows = parent().par.Panelrows
	panel_cols = parent().par.Panelcolumns

	img_data = op('img_data')
	frame_data = []

	for row in range(panel_rows):
		for col in range(panel_cols):
			r = int(img_data[0][(row * panel_cols + col)] * 255)
			g = int(img_data[1][(row * panel_cols + col)] * 255)
			b = int(img_data[2][(row * panel_cols + col)] * 255)

			frame_data.extend([r, g, b])

	send_frame_data(frame_data)

	return
