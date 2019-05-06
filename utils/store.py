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
time = datetime.datetime.now()
while True:
        now = datetime.datetime.now()
        err,data = win32file.ReadFile(handle, readsize)
        pickle.dump(now, output_file)
        pickle.dump(data, output_file)
        print(time.minute - now.minute)