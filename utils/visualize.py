import win32pipe, win32file
import numpy
import matplotlib.pyplot


handle = win32file.CreateFile("\\\\.\\pipe\\ch0_raw_pipe",
                            win32file.GENERIC_READ,
                            0,
                            None,
                            win32file.OPEN_EXISTING,
                            0,
                            None)
print(handle)
while True:
	err,buffer = win32file.ReadFile(handle, 200000*2)
	buffer = numpy.frombuffer(buffer, dtype=numpy.int16)
	matplotlib.pyplot.plot(buffer)
	matplotlib.pyplot.show()