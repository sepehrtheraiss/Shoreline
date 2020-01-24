Originally wanted to this in elixir, but doing it in a familiar language was a better choice for testing purposes and time.
[server.py](flask/server.py)

1. #### Please describe your solution to get_id and why it is correct i.e. guaranteed globally unique.
    
    We want to have **monotonically increasing** *Unique ID*, a naive approach would be to use time.  
    But what if two different nodes get_id returns the same time stamp ?   
    A solution would be to combine the time stamp with nodes ID, but there exists another problem.  
    What if the same node is concurrently executing the same function for two different clients ? or the function is invoked in such matter that it produces same time stamp for multiple functions ?  

    ### Final Solution 
    The Final solution would be to also have a sequence counter, therefore for each get_id invocation we can increase the sequence number.  
    This would **guarantee uniqueness**, because if the same node produces the same time stamp plus it's node ID, a sequence number is going to be different for each function call.  
    Another question would be what if two nodes produce the same time stamp and sequence number. Well, each nodes ID is mutually exclusive, therefore producing a unique ID. 

    Since we are asked to use 64-bit Unique ID, we will use the following property (Twitter Snow Flake)

    * **Time stamp** would be calculated in milliseconds using Epoch, giving us enough time and uniqueness until January 19, 2038. Since Epoch is **signed 32-bit** the most significant bit will not be used, giving us actually 31-bit to work with. Converting seconds to millisecond gives us 10 extra bit, therefore the **max size of bits** we would need to represent Epoch in millisecond is **41-bits**.  
    * **Node ID's** would be represented using **unsigned 10-bits** giving us range of 0-1023. 
    * This remains **12-bits** for **sequence number**, also unsigned int, range 0-4095. 

    ```  
    timestamp: 41-bits = time().milliseconds()
    id: 10-bits = node.ID()
    (seq: 12-bits)++
    UID: 64-bits  = (timestamp << (63-41)) | (id << (63-41-10)) | (seq)
    return UID
    ```
2. #### Please explain how your solution achieves the desired performance i.e. 100,000 or more requests per second per node.  How did you verify this?   

    * All the operations are in constant time.
    * An approximation has been made by sampling start time and end time of get_id() function in nanosecond. Refer [stats.py](flask/stats.py)

    ```
    Average get_id() invocation:  9626  nanosecond
    Approximately  103885  calls per second (1000000000/ 9626)
    ```
    * The test has been done locally, meaning our http server has been receiving requests as fast as possible.  
    Since we have shown it's true for our server to handle over 100,000 request per second in the **worst case**, then it's also true it can handle at least 100,000 requests per second remotely.
3. #### Please enumerate possible failure cases and describe how your solution correctly handles each case.  How did you verify correctness?  Some example cases:  
    1. Unique ID, Sequence and Node ID over flow 
        * For Node ID, a simple check to see if ID hasn't passed 1023, tested it manually.
        * For Sequence number, I had to scale it down to a number I could execute it concurrently, to see if the threshold has exceeded. 
        * For uniqueness, I sampled 1000 concurrent requests. Refer [client.py](flask/client.py) 

* **How do you manage uniqueness after a node crashes and restarts?  
 How do you manage uniqueness after the entire system fails and restarts?**  
 Unique IDs are monotonically increasing and independent from other nodes.
 [solution](#Final-Solution)  

 * **How do you handle software defects ?**  
  The most unnoticeable defect would be returning *unique ID* as an **integer**. 
Some protocols such as http would only accept strings, converting all given bytes to ASCII, therefore some API calls (i.e. flask) would fail.  
 Furthermore, some programming languages, such as Java Script, max safe integer value is 2^53 - 1.  
 So it is always safer to pass strings around. [[reference](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER)]
