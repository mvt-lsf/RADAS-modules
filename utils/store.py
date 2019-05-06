import win32pipe, win32file
import datetime
import sys
import pickle

handle = win32file.CreateFile("\\\\.\\pipe\\ch0_raw_pipe",
                            win32file.GENERIC_READ,
                            0,
                            None,
                            win32file.OPEN_EXISTING,
                            0,
                            None)
output_file = open(sys.argv[1],"wb")
readsize = 4096

while True:
        err,data = win32file.ReadFile(handle, readsize)
        pickle.dump(datetime.datetime.now(), output_file)
        pickle.dump(data, output_file)
