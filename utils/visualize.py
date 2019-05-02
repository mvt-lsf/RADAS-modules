import win32pipe, win32file
import numpy
import matplotlib.pyplot
#import bottleneck

handle = win32file.CreateFile("\\\\.\\pipe\\ch0_raw_pipe",
                            win32file.GENERIC_READ,
                            0,
                            None,
                            win32file.OPEN_EXISTING,
                            0,
                            None)
print(handle)
readsize = 3000 * 100 * 8	#toma un chunk
samples = 100					#cantidad de chunks que queremos
while True:
	print(readsize)
	print(samples)
	err,data = win32file.ReadFile(handle, readsize)
	data = numpy.frombuffer(data, dtype=numpy.float64)
	data.shape = (3000, -1)
	buffer = numpy.ma.average(data, axis=1)
#	for i in range(samples-1):
#		err,data = win32file.ReadFile(handle, readsize)
#		data = numpy.frombuffer(data, dtype=numpy.float64)
#		data.shape = (-1, 3000)
#		buffer = numpy.append(buffer,numpy.ma.average(data, axis=0))
#		print(buffer.shape)
#	print(buffer.shape)
#	buffer.shape = (samples, -1)
	matplotlib.pyplot.plot(buffer)
#	matplotlib.pyplot.plot(data)
#	matplotlib.pyplot.show()
#	matplotlib.pyplot.figure()
#	matplotlib.pyplot.imshow(buffer, aspect='auto', cmap='jet')  # plot con el filtro
	matplotlib.pyplot.show()