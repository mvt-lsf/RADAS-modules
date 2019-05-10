import config
import pipe
import datetime
import os
import sys

try:
   import cPickle as pickle
except:
   import pickle


def new_store_file(path, filename):
    datapath = os.path.join(path, filename)
    tmppath = os.path.join(path, "tmp_file")
    os.rename(datapath, tmppath)
    file = open(datapath, "wb")
    return file


def store_data(file, data, time):
    pickle.dump(time, file,pickle.HIGHEST_PROTOCOL)
    pickle.dump(data, file,pickle.HIGHEST_PROTOCOL)


def rmv_old_tmp_file(path):
    tmppath = os.path.join(path, "tmp_file")
    try:
        os.remove(tmppath)
    except OSError:
        pass


readsize = configuration["Bins"] * configuration["NShotsChk"]
outpipe = pipe.duplex_pipe("ch0_store", readsize, 4096)
outpipe.set_non_blocking()
inputpipe = pipe.input_pipe("ch0_raw_pipe")
outfile = open(sys.argv[1] + "data", "wb")
configuration = config.load("config.ini")

time = datetime.datetime.now()
while True:
    now = datetime.datetime.now()
    err, data = inputpipe.read(readsize)
    outpipe.write(data)
    store_data(outfile, data, now)
    if (now.minute - time.minute) >= 5:
        outfile.close()
        rmv_old_tmp_file(sys.argv[1])
        outfile = new_store_file(sys.argv[1], "data")
        time = datetime.datetime.now()
