Deployed on cloud: [https://ds-uid.herokuapp.com](https://ds-uid.herokuapp.com)  
Testing over 1000 requests on heroku is too slow, need to be done locally.

##Install 

Either use **pip3 install requirements.txt**, or   
 **docker**:  
  
  1. Docker for mac or windows can be downloaded from here: [https://hub.docker.com/?overlay=onboarding](https://hub.docker.com/?overlay=onboarding)  
for linux [https://runnable.com/docker/install-docker-on-linux](https://runnable.com/docker/install-docker-on-linux)  
An account is already made
 	* username: sepehrtheraiss   
 	* password: ShoreLineSoftware2020  
 2. Docker build -t app .
 3. docker run -p 8080:8080 app
 
##Test
[client/client.py](client/client.py) has two urls to execute, local or cloud.   
Pass 'l' for local or 'c' for cloud.  
Please do pip3 install aiohttp



##Questions
Q1. To over come large requests per unit of time (i.e. 1,000 per milisecond or 1,000,000 per second)  
We must use a load balancer, load balancing can be done by using an orchestration, with docker it would be swarm.  One node would become a forwarder and manager. Then the load is passed to workers.   
This would create a **problem**, workers are just replication and seem like one from the outside,   
meaning two workers worker1 & worker2 can produce the same time stamp, node id and sequence number.  

To over come this, each worker will be given a portion of the sequence number.  
`worker[i] = sequence-range: (i*4096/n)-(4096/n)`  
For Example if we have 4 workers, worker[0] = {sequence-range: 0-1024} 

Q2.  

