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
while True:
	readsize = 3000*5000
	err,data = win32file.ReadFile(handle, readsize)
	buffer = data
	#while len(data) == readsize:
	#	err,data = win32file.ReadFile(handle, readsize)
	#	buffer += data
	
	buffer = numpy.frombuffer(buffer, dtype=numpy.float64)
#	matplotlib.pyplot.plot(waterfall)
#	matplotlib.pyplot.figure()
#	matplotlib.pyplot.imshow(waterfall, aspect='auto', cmap='jet')  # plot con el filtro
	matplotlib.pyplot.plot(buffer)