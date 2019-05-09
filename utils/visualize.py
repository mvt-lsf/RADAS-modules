import pipe
import numpy
import matplotlib.pyplot

datainput = pipe.input_pipe("ch0_raw_pipe")
readsize = 3000 * 100 * 2	#toma un chunk
print(readsize)

while True:
	err,data = datainput.read(readsize)
	buffer = numpy.frombuffer(data, dtype=numpy.int16)
	matplotlib.pyplot.plot(buffer)
	matplotlib.pyplot.show()