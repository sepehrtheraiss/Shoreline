from flask import Flask
import re, os, socket, time
import threading

#def log(t):
#    with open("log.txt" , 'a') as file:
#        file.write(str(t)+'\n')

# latest flask version http by default is multithreaded
application = Flask(__name__)

counter = 0
MAX_ID = 1023
MAX_SEQ = 4095 

#id = int(os.environ.get('ID', None)) 
id = 2
#print("Real ID ", id)
assert id <= MAX_ID, "node id > Max Node ID"
id <<= (63-41-10)
#print("shifted ID ", id)

@application.route('/')
def get_nodeid():
    # millisecond is still too fast
    # most sequential calls returns the same time stamp
    # use nanosecond for analysis
#    start = time.time_ns()
    global counter
    t = int(round(time.time() * 1000)) << (63-41)
    with threading.Lock():
        counter += 1
        if(counter > MAX_SEQ):
            counter = 0
        seq = counter
    uuid = t | id | seq
 #   end = time.time_ns()
    #log(end-start)
    return str(uuid)

if __name__ == '__main__':
    #app.run(host="0.0.0.0", port=80, debug=True)
    application.run()
