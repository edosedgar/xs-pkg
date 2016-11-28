#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt

def fft(signal, N):
    if N == 2:
        return [signal[1] + signal[0], signal[1] * np.exp(-1j*(np.pi)) + signal[0]]
    even = fft(signal[::2], N/2)
    odd = fft(signal[1::2], N/2)
    factor = np.exp(-2j * np.pi * np.arange(N) / N)
    return np.asarray(np.concatenate([even + factor[:N/2] * odd,
                      even + factor[N/2:] * odd]), dtype=np.complex128)
def revfft(signal, N):
    if N == 2:
        return [signal[1] + signal[0], signal[1] * np.exp(1j*(np.pi)) + signal[0]]
    even = fft(signal[::2], N/2)
    odd = fft(signal[1::2], N/2)
    factor = np.exp(2j * np.pi * np.arange(N) / N)
    return np.asarray(np.concatenate([even + factor[:N/2] * odd,
                      even + factor[N/2:] * odd]), dtype=np.complex128)


in_ = [np.sin(2*np.pi*n/16)*(np.random.sample()) for n in range(128)] + [0 for n in range(4096-128)]
out_ = fft(in_, len(in_))
plt.plot([i/len(in_) for i in range(4096)], np.abs(out_)/len(in_))
plt.show()
