import numpy

def mv_avg_std(matrix,window):

    res=numpy.zeros((matrix.shape[0]-window+1,matrix.shape[1]))#puede salir si mejora tiempos
    res[0]=numpy.mean(matrix[:window,:],axis=0)
    
    suma_actual=numpy.sum(matrix[:window,:],axis=0)

    for i in range(1,matrix.shape[0]-window+1):
        suma_actual=suma_actual-matrix[i-1,:]+matrix[i+window-1,:]
        res[i]=suma_actual/window
    
    return numpy.std(res,axis=0)/numpy.mean(res,axis=0)


def std_filter(raw_m,shotPorChunk=1000,window_time=20,window_z=5):
    n=int(raw_m.shape[0]/shotPorChunk)
    wf=numpy.zeros((n,raw_m.shape[1]))
    for i in range(n):
        esteChunk=raw_m[i*shotPorChunk:(i+1)*shotPorChunk,:] 
        esteChunkSTD=mv_avg_std(esteChunk,window_time)
        wf[i,:]=esteChunkSTD
    wf=wf.T
    wf=mv_avg_std(wf,window_z)
    wf=wf.T
    return wf