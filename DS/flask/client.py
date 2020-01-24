#!/bin/python3
# https://docs.python.org/3/library/concurrent.futures.html#threadpoolexecutor-example
# ThreadPoolExecutor Example 
import concurrent.futures
import urllib.request
import time

TIME_MASK = 0xFFFFFFFFFFC00000
ID_MASK = 0x3FF000
SEQ_MASK = 0xfff
BAD_SEQ = 4096 #10

# checks for uniqueness and
# over flow seq number
poolSize = 1000 # cannot handle 4096 + 1
URLS = ["http://localhost:8080" for i in range(poolSize)]
uid_table = {}

# Retrieve a single page and report the URL and contents
def load_url(url, timeout):
    with urllib.request.urlopen(url, timeout=timeout) as conn:
        return conn.read()

# Tests uniqueness and seq over flow
# We can use a with statement to ensure threads are cleaned up promptly
with concurrent.futures.ThreadPoolExecutor(max_workers=poolSize) as executor:
    # Start the load operations and mark each future with its URL
    future_to_url = {executor.submit(load_url, url, 60): url for url in URLS}
    for future in concurrent.futures.as_completed(future_to_url):
        url = future_to_url[future]
        try:
            data = future.result()
        except Exception as exc:
            print('%r generated an exception: %s' % (url, exc))
        else:
            uid = int(data)
            print("timestamp: ", (uid & TIME_MASK) >> 22, "nodeID: ", (uid & ID_MASK) >> 12, "Sequence: ", (uid & SEQ_MASK))

            assert not(uid & SEQ_MASK == BAD_SEQ), "Hit bad sequence number"

            if uid_table.get(data) != None:
                uid_table[data] += 1
                print("Dup: ",data)
                assert False, "Duplicate uid"
            else:
                uid_table[data] = 0
            print(data)
            #print('%r page is %d bytes' % (url, len(data)))
