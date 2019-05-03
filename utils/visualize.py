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
readsize = 3000 * 50 * 8	#toma un chunk
#samples = 1					#cantidad de chunks que queremos
while True:
	print(readsize)
	err,data = win32file.ReadFile(handle, readsize)
	buffer = data
#	for i in range(samples-1):
#		err,data = win32file.ReadFile(handle, readsize)
#		buffer += data
	buffer = numpy.frombuffer(buffer, dtype=numpy.float64)
	print(buffer.shape)
	matplotlib.pyplot.plot(buffer)
	matplotlib.pyplot.show()