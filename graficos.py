from matplotlib import pyplot as plt
m1 = open("outT4P300E100.csv", "r")
m2 = open("outT4P4930E100.csv", "r")
m3 = open("outT4P9930E100.csv", "r")
m4 = open("outT6P30E100.csv", "r")
m5 = open("outT6P4950030E100.csv", "r")
m6 = open("outT6P9863000E100.csv", "r")
m7 = open("outT8P10E100.csv", "r")
m8 = open("outT8P40000000E100.csv", "r")
m9 = open("outT8P90000000E100.csv", "r")


def read_and_load_graph(file, title):
    serial = 0
    parallel = 0
    exp_l = []
    serial_l = []
    parallel_l = []
    exp = 0
    for line in file:
        arr = line.split(",")
        exp = int(arr[1])
        exp_l.append(exp)
        serial_l.append(float(arr[2])*1000)
        parallel_l.append(float(arr[3])*1000)
        serial += float(arr[2])*1000
        parallel += float(arr[3])*1000
    serial = serial/100
    parallel = parallel/100
    plt.ylabel('T (10^T valores en el experimento)')
    plt.xlabel('Time (ms)')
    plt.axis([-0.25, 2, 0, 9])
    plt.plot(serial_l, exp_l, "r^--", label="Serial Time")
    plt.plot(parallel_l, exp_l, "bs-", label="Parallel Time")
    plt.title(title)
    plt.legend()
    plt.show()


read_and_load_graph(m1, "outT4P300E100.csv")
read_and_load_graph(m2, "outT4P4930E100.csv")
read_and_load_graph(m3, "outT4P9930E100.csv")
read_and_load_graph(m4, "outT6P30E100.csv")
read_and_load_graph(m5, "outT6P4950030E100.csv")
read_and_load_graph(m6, "outT6P9863000E100.csv")
read_and_load_graph(m7, "outT8P10E100.csv")
read_and_load_graph(m8, "outT8P40000000E100.csv")
read_and_load_graph(m9, "outT8P90000000E100.csv")

m1.close()
m2.close()
m3.close()
m4.close()
m5.close()
m6.close()
m7.close()
m8.close()
m9.close()
