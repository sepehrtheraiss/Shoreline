program takes in a file with list of cidr and port to block
1. save the cidr mask
2. calculate the net portion of it
3. map port to int net address.
ex: 192.168.1.1 comes in
using the stored cidr we need to mask it and see if it matches 
an entry in our [port] => list<net>
