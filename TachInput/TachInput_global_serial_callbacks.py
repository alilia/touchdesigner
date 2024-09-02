class TachInputHandler:
	# Define states as class attributes
	WAITING_FOR_DATA = 0
	RECEIVING_DATA = 1

	def __init__(self, tach_input_op):
		self.tach_input_op = tach_input_op
		self.tach_input_state = TachInputHandler.WAITING_FOR_DATA

	def on_receive(self, message):
		val = int(message)

		if self.tach_input_state == TachInputHandler.WAITING_FOR_DATA:
			if val == int('0x' + self.tach_input_op.par.Rpmmarker, 16):
				self.tach_input_state = TachInputHandler.RECEIVING_DATA
		elif self.tach_input_state == TachInputHandler.RECEIVING_DATA:
			self.tach_input_op.par.Rpm = val
			self.tach_input_state = TachInputHandler.WAITING_FOR_DATA

tach_input_handler = TachInputHandler( op('TachInput_20240803_1') )

def onReceive(dat, rowIndex, message, byteData):
	tach_input_handler.on_receive(message)
	return
