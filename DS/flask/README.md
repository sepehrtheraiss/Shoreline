Deployed on cloud: [https://ds-uid.herokuapp.com](https://ds-uid.herokuapp.com)  
Testing over 1000 requests on heroku is too slow, need to be done locally.

## Install & Test

Either use **pip3 install**, or   **docker**:  
  
### Docker
  
  1. Docker for mac or windows can be downloaded from here: [https://hub.docker.com/?overlay=onboarding](https://hub.docker.com/?overlay=onboarding)  
for linux [https://runnable.com/docker/install-docker-on-linux](https://runnable.com/docker/install-docker-on-linux)  
An account is already made
 	* username: sepehrtheraiss   
 	* password: ShoreLineSoftware2020 
 2.  cd flask
 3. docker build -t app .
 4. docker run --net=mynet --ip=192.168.1.2 app
 5. cd client
 6. docker build -t client .
 7. docker run --net=mynet --ip=192.168.1.3 client
 
### PIP
1. pip3 install -r requirements.txt
2. export ID=1
3. python3 app/app.py  
4. [client/client.py](client/client.py) has two urls to execute, local or cloud.   
Pass 'l' for local or 'c' for cloud.(default no args is local)  




##Questions
Q1. To over come large requests per unit of time (i.e. 1,000 per milisecond or 1,000,000 per second)  
We must use a load balancer, load balancing can be done by using an orchestration, with docker it would be swarm.  One node would become a forwarder and manager. Then the load is passed to workers.   
This would create a **problem**, workers are just replication and seem like one from the outside,   
meaning two workers worker1 & worker2 can produce the same time stamp, node id and sequence number.  

To over come this, each worker will be given a portion of the sequence number.  
`worker[i] = sequence-range: (i*4096/n)-(4096/n)`  
For Example if we have 4 workers, worker[0] = {sequence-range: 0-1024} 

Q2. What if a node goes down and comes back up in such matter that it will give the same time stamp and sequence number ?  
	We would need to save our sequence number to a file, and read it when the node restarts, but doing so every time is very costly.  
	To avoid this, we can set a threshold and when it's reached, we will write to the file.   
	
When the sequence number file is read
	upon restart, we need to immidiately update the seq number to the next threshold and write it to the file in case the node crashes before
	reaching the next threshold. 
	This way we will pass over the possible duplicate seq numbers.  
	
But this could also cause another problem.
	Since every time we read from the file we advance to the next seq number, if amount of time it takes to overflow is under 1 millisecond, 
	we risk having the same seq number with the same time stamp.   
	 
Based on our previous calculation of average amount of time it takes to finish the invokation of get_id function is 9626 nanosecond.
	9626ns to ms is 0.009626.   
	
*let t = 0.009626 ms,  r = sequence number range and l = threshold leap*, as long as the following property holds true, we will not get a 
	duplicate timestamp and sequence number.  
	
	t * (r/l) >= 1ms  
	
we want the over follow to happen after a milisecond has passed, so our sequence number wont be a number that it was during the same time stamp.
	

