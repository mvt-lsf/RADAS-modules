import win32pipe
import win32file
import datetime
import os
import sys
import pickle


def new_store_file(path, filename):
    datapath = os.path.join(path, filename)
    tmppath = os.path.join(path, "tmp_file")
    os.rename(datapath, tmppath)
    file = open(path, "wb")
    return file


def store_data(file, data, time):
    pickle.dump(time, file)
    pickle.dump(data, file)


def rmv_old_tmp_file(path):
    tmppath = os.path.join(path, "tmp_file")
    try:
        os.remove(tmppath)
    except OSError:
        pass


class input_pipe:
    def __init__(self, pipename):
        self.pipe = win32file.CreateFile("\\\\.\\pipe\\" + pipename,
                                 win32file.GENERIC_READ,
                                 0,
                                 None,
                                 win32file.OPEN_EXISTING,
                                 0,
                                 None)

    def read(self, readsize):
        return win32file.ReadFile(self.pipe, readsize)


class duplex_out_pipe:
    def __init__(self, pipename, in_buff_siz, out_buff_siz):
        self.pipe = win32pipe.CreateNamedPipe("\\\\.\\pipe\\" + pipename,
                                    win32pipe.PIPE_ACCESS_DUPLEX,
                                    win32pipe.PIPE_TYPE_BYTE,  # | win32pipe.PIPE_WAIT,
                                    4,
                                    out_buff_siz,
                                    in_buff_siz,
                                    win32pipe.NMPWAIT_USE_DEFAULT_WAIT,
                                    None)

    def read(self, readsize):
        try:
            err, data = win32file.ReadFile(self.pipe, readsize)
        except:
            pass
        return data

    def write(self, data):
        win32file.WriteFile(self.pipe, data)

    def set_non_blocking(self):
        win32pipe.SetNamedPipeHandleState(
        self.pipe, win32pipe.PIPE_TYPE_BYTE | win32pipe.PIPE_NOWAIT, None, None)


# inputpipe = win32file.CreateFile("\\\\.\\pipe\\ch0_raw_pipe",
#                                 win32file.GENERIC_READ,
#                                 0,
#                                 None,
#                                 win32file.OPEN_EXISTING,
#                                 0,
#                                 None)
#outpipe=win32pipe.CreateNamedPipe("\\\\.\\pipe\\ch0_store",
#                                    win32pipe.PIPE_ACCESS_DUPLEX,
#                                    win32pipe.PIPE_TYPE_BYTE,  # | win32pipe.PIPE_WAIT,
#                                    4,
#                                    4096,
#                                    4096,
#                                    win32pipe.NMPWAIT_USE_DEFAULT_WAIT,
#                                    None)

outpipe = duplex_out_pipe("ch0_store", 4096, 4096)
outpipe.set_non_blocking()
inputpipe = input_pipe("ch0_raw_pipe")
outfile = open(sys.argv[1] + "data", "wb")

readsize = 4096
time = datetime.datetime.now()
while True:
    now = datetime.datetime.now()
    err, data = inputpipe.read(readsize)
    outpipe.write(data)
    store_data(outfile, data, now)
    if (now.minute - time.minute) <= 5:
        outfile.close()
        rmv_old_tmp_file(sys.argv[1])
        outfile = new_store_file(sys.argv[1], "data")
