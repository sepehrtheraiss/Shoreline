#!/bin/python3
with open('log.txt', 'r') as f:
    s = 0
    count = 0
    for line in f:
        s += int(line.strip())
        count += 1
    print("Average get_id() invokation: ",round(s/count), " nano second")
    oneSec_to_ns = 1000000000
    print("Approximately ", round(oneSec_to_ns/ round(s/count))," calls per second")
