import win32pipe, win32file
import resource
import sys

handle = win32file.CreateFile("\\\\.\\pipe\\ch0_raw_pipe",
                            win32file.GENERIC_READ,
                            0,
                            None,
                            win32file.OPEN_EXISTING,
                            0,
                            None)
output_file = open(sys.argv[1],'w')
readsize = resource.getpagesize()

while True:
        err,data = win32file.ReadFile(handle, readsize)
        file.write(data, file=output_file)
