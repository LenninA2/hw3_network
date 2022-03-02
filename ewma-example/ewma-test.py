import matplotlib.pyplot as plt
import numpy as np
from sys import argv

def ewma(data, weight):
    avgs = [data[0]]
    for d in data[1:]:
        avg = (1 - weight) * avgs[-1] + weight * d
        avgs.append(avg)
    
    return np.array(avgs)

sample_rtt = np.random.randint(100, 300, 1000).astype('f')
sample_rtt += np.linspace(0, 100, 1000)

alpha = 0.125
beta = 0.125
est_rtt = ewma(sample_rtt, alpha)
dev_rtt = ewma(np.abs(sample_rtt - est_rtt), beta)
timeout = est_rtt + 4*dev_rtt
plt.plot(sample_rtt, label='SampleRTT')
plt.plot(est_rtt, label='EstimatedRTT (alpha={})'.format(alpha))
plt.plot(dev_rtt, label='DevRTT (beta={})'.format(beta))
plt.plot(timeout, label='Timeout')

plt.title('TCP Timeout Demo')
plt.xlabel('Segment Number')
plt.ylabel('Time (ms)')
plt.legend(loc=0)
plt.show()
