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
readsize = 3000 * 100 * 2	#toma un chunk
print(readsize)

while True:
	err,data = win32file.ReadFile(handle, readsize)
	buffer = numpy.frombuffer(data, dtype=numpy.int16)
	matplotlib.pyplot.plot(buffer)
	matplotlib.pyplot.show()