#!/bin/python3
# https://docs.python.org/3/library/concurrent.futures.html#threadpoolexecutor-example
# ThreadPoolExecutor Example 
import concurrent.futures
import urllib.request
import time


# 1. over flow seq number
# 2. over flow id num (use docker)
# 3. time? -\_(`_`)_/-

# Sequnce over flow test
# 1 overflow, num should be 0
poolSize = 1000#4096 + 1
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
            if uid_table.get(data) != None:
                uid_table[data] += 1
                assert False, "Duplicate uid"
            else:
                uid_table[data] = 0
            #print(data)
            #print('%r page is %d bytes' % (url, len(data)))
