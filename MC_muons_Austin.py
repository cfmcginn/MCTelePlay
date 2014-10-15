import random
import numpy as np
import math
from scipy.integrate import quad
import matplotlib.pyplot as plt

#geometry,
a1 = 10.0
b1=10.0
a2=10.0
b2=10.0
L= np.linspace(0.1,100,101).tolist()

lengths_to_try = len(L) 
dr = np.zeros(lengths_to_try)

max_angle_from_corner = np.zeros(lengths_to_try)
for k in range(lengths_to_try):
    max_angle_from_corner[k] = math.atan((((a1/2.0+a2/2.0)**2+(b1/2.0+b2/2.0)**2)**(1/2.0))/L[k])


#exponent of cos(theta) that is used for the muon intensity I=I_0*cos(theta)**angular_power
angular_power = 1.95

#MC parameters
total_evts = 10000

#MC
for k in range(lengths_to_try):    
    print L[k]
    total_accepted = 0

    for n in range(total_evts):
        #if n%1000 == 0:
        #    print n
    
        x = a1*random.random()-a1/2.0
        y = b1*random.random()-b1/2.0
        phi = 2*math.pi*random.random()
        theta = math.acos((random.random())**(1./(2.0+angular_power)))
    
        if (x+L[k]*math.tan(theta)*math.cos(phi) < a2/2.0) & (x+L[k]*math.tan(theta)*math.cos(phi)>=-a2/2.0) & (x+L[k]*math.tan(theta)*math.sin(phi) < b2/2.0) & (x+L[k]*math.tan(theta)*math.sin(phi) >=-b2/2.0):
            total_accepted = total_accepted+1

    fraction_accepted = float(total_accepted)/float(total_evts)
    print 'fraction_accepted after hitting top panel: ',fraction_accepted


    # gathering power is the fraction accepted times the total gathering power of the top panel
    #0.253165 comes from the integral(cos(theta)^2.95,d(cos(theta)),0,1), which is the top panel's gathering panel for an incoming intensity
    # that is proportional to cos(theta)^1.95

    def integrand(x):
        return x**(1+angular_power)
    top_power = quad(integrand,0,1)[0]

    gathering_power = 2*math.pi*a1*b1*top_power
    print 'gathering power of top panel: ', gathering_power, ' cm^2*sr'


    #absolute verticle muon flux: 0.007/cm^2/s/sr
    rate = 0.007
    #converted to muons/min
    detection_rate = gathering_power*fraction_accepted*rate*60.0
    dr[k] = detection_rate
    print 'detection rate: ', detection_rate, ' muons/min'

    #electrons/min assumes 1% muons decay
    print 'electron rate: ', detection_rate/100.0, 'electrons/min'

#plots
plt.subplot(2,1,1)
plt.plot(L,dr)
plt.ylabel('muons/min')
plt.xlabel('distance between 2 10x10 detectors (cm)')
plt.subplot(2,1,2)
plt.plot(max_angle_from_corner,dr)
plt.ylabel('muons/min')
plt.xlabel('max angle a muon can have from verticle')
plt.tight_layout()
plt.show()