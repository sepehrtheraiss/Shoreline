#!/bin/python3
import asyncio
from aiohttp import ClientSession
import sys
import os

TIME_MASK = 0xFFFFFFFFFFC00000
ID_MASK = 0x3FF000
SEQ_MASK = 0xfff
BAD_SEQ = 4096
N_REQ = 5000
host = os.getenv("HOST")
port = os.getenv("PORT")

if host != None and port != None:
	URL = "http://"+host+":"+port
else:
    if len(sys.argv) < 2:
        URL = "http://localhost:8080"
    else:
        if str.lower(sys.argv[1]) == 'l':
            URL = "http://localhost:8080"
        elif str.lower(sys.argv[1]) == 'c':
            URL = "https://ds-uid.herokuapp.com"
        else:
            URL = "http://localhost:8080"


uid_table = {}


# checks for uniqueness and
# over flow seq number
async def get(url):
    async with ClientSession() as session:
        async with session.get(url) as response:
            response = await response.read()
            print(response)
            uid = int(response)
            print("timestamp: ", (uid & TIME_MASK) >> 22, "nodeID: ", (uid & ID_MASK) >> 12, "Sequence: ", (uid & SEQ_MASK))
            assert not(uid & SEQ_MASK == BAD_SEQ), "Hit bad sequence number"

            if uid_table.get(uid) != None:
                uid_table[uid] += 1
                print("Dup: ",uid)
                assert False, "Duplicate uid"
            else:
                uid_table[uid] = 0


loop = asyncio.get_event_loop()

for i in range(N_REQ):
	loop.run_until_complete(get(URL))
