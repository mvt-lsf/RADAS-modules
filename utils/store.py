import win32pipe, win32file
import datetime
import os
import sys
import pickle

handle = win32file.CreateFile("\\\\.\\pipe\\ch0_raw_pipe",
                            win32file.GENERIC_READ,
                            0,
                            None,
                            win32file.OPEN_EXISTING,
                            0,
                            None)
output_file = open(sys.argv[1] + "/data","wb")
readsize = 4096
time = datetime.datetime.now()
while True:
        now = datetime.datetime.now()
        err,data = win32file.ReadFile(handle, readsize)
        pickle.dump(now, output_file)
        pickle.dump(data, output_file)
        if (now.minute - time.minute) <= 5 :
                output_file.close()
                os.remove(sys.argv[1] + "tmp_file")
                os.rename(sys.argv[1] + "/data", sys.argv[1] + "tmp_file")
                output_file = open(sys.argv[1] + "/data","wb")