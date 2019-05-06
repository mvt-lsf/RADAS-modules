# -*- coding: utf-8 -*-
"""
Created on Fri Apr 12 15:19:41 2019

@author: Fotonica
"""
    
import numpy as np
import multiprocessing as mp
import win32file
import time
import sys

def windows_pipe(pipe_name):
	return win32file.CreateFile("\\\\.\\pipe\\"+pipe_name,
	                          win32file.GENERIC_READ | win32file.GENERIC_WRITE,
	                          0, None,
	                          win32file.OPEN_EXISTING,
	                          0, None)


def filler(pipe_name,data_size,data_q):
    
    pipe_handler=windows_pipe(pipe_name)

    while True:
        start_time=time.time()
        error,data=win32file.ReadFile(pipe_handler,data_size)
        data_q.put(np.frombuffer(data,dtype=np.int16))
        elapsed_time=(time.time()-start_time)
        #print(elapsed_time)
        #if elapsed_time>0:
            #print ('Lectura pipe: ', data_size/2/1024./1024./elapsed_time, ' MB/s')
        
def filler_py(data_q,bins,shots_per_chunk):
    
    while True:
        data_q.put(np.random.randint(2**14,size=(shots_per_chunk,bins),dtype=np.int16))
        time.sleep(0.5)


def reader(data_q,bins,shots_per_chunk,window):
    tmp_filtr = np.zeros((shots_per_chunk-window+1, bins))
    while True:
        if not(data_q.empty()):#not reliable
            #print (filt(data_q.get(),bins))
            st=time.time()
            mat=data_q.get().reshape(-1,bins)
            mv_avg_std(mat,window,tmp_filtr)
            #print (mat.shape)
            print ("Tiempo para filtrar un chunk de ",bins," puntos y ", shots_per_chunk," shots >", time.time()-st)
            print ('Chunks pendientes: ',data_q.qsize())#not reliable
        else:
            time.sleep(0.1)#??

def filt(data,bins):
    return np.sum(np.diag(data.reshape(-1,bins)))

def mv_avg(matrix, window,res):
    res[0] = np.mean(matrix[:window, :], axis=0)

    suma_actual = np.sum(matrix[:window, :], axis=0)

    for i in range(1, matrix.shape[0]-window+1):
        suma_actual = suma_actual-matrix[i-1, :]+matrix[i+window-1, :]
        res[i] = suma_actual/window
    return res


def mv_avg_std(matrix, window,res):
    res = mv_avg(matrix, window,res)
    return np.std(res, axis=0)/np.mean(res, axis=0)

        
        
if __name__=='__main__':

        
    data_q=mp.Queue()
  
    if len(sys.argv)>1:
        bins=int(sys.argv[1])
        shots_per_chunk=int(sys.argv[2])
        single_data_size=2 #bytes
        data_size=bins*shots_per_chunk*single_data_size
        pipe_name='ch0_raw_pipe'
        filler_p=mp.Process(target=filler,args=[pipe_name,data_size,data_q])
    else:
        bins=22500
        shots_per_chunk=1000
        filler_p=mp.Process(target=filler_py,args=[data_q,bins,shots_per_chunk])
  
    window_time=20

    reader_p=mp.Process(target=reader,args=[data_q,bins,shots_per_chunk,window_time])
    
    filler_p.start()
    reader_p.start()

    filler_p.join()
    reader_p.join()
