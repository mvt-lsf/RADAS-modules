import win32file
import win32pipe

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


class duplex_pipe:
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
        return err, data

    def write(self, data):
        try:
            win32file.WriteFile(self.pipe, data)
        except:
            return -1

    def set_non_blocking(self):
        win32pipe.SetNamedPipeHandleState(
        self.pipe, win32pipe.PIPE_TYPE_BYTE | win32pipe.PIPE_NOWAIT, None, None)