from flask import Flask
import re, os, socket, time

def log(t):
    with open("log.txt" , 'a') as file:
        file.write(str(t)+'\n')

app = Flask(__name__)
counter = 0
@app.route('/')
def get_nodeid():
    # milisecond is still too fast
    # most sequential calls returns the same time stamp
    # use nanosecond to analysis
    start = time.time_ns()
    global counter
    t = int(round(time.time() * 1000)) << (63-41)
    id = int(os.environ.get('id', None)) << (63-41-10)
    if(counter == 4096):
        counter = 0
    seq = counter
    counter += 1
    uuid = t | id | seq
    end = time.time_ns()
    log(end-start)
    return str(uuid)
