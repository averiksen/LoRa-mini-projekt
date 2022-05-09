import serial
import matplotlib.pyplot as plt
import numpy

plt.ion()
fig = plt.figure()

i = 0
x = list()
y = list()

ser = serial.Serial('COM6', 115200)
ser.close()
ser.open()
while True:
    data = ser.readline()
    data = data[10:18]
    data_hex = int(data, 16)
    level = data_hex & 0xFF
    time = data_hex & 0xFFFFFF00
    print(time, level)

    fig = plt.figure(figsize=(4+i*0.15, 4))
    x.append(i)
    y.append(level)
    plt.scatter(x,y)
    i += 1
    plt.show()
    #plt.pause(0.0001)





