1. ##### Please describe your solution to get_id and why it is correct i.e. guaranteed globally unique.
    
    We want to have monotonically increasing UUID, a naive approach would be to use time.  
    But what if two different nodes get_id returns the same time stamp ?   
    A solution would be to combine the time stamp with nodes ID, but there exsist another problem.  
    What if the same node is concurrently executing the same function for two different clients ? or the function is invoked in such matter that it produces same time stamp for multiple functions ?  

    # Final Solution 
    The Final solution would be to have a sequence counter, therefore for each get_id invocation we can increase the sequence number.  
    This Would **guarentee uniqueness**, because if the same node produces the same time stamp plus it's node ID, a sequence number is going to be different for each function call.  
    Another question would be what if two nodes produce the same time stamp and sequence number, well each nodes ID is mutually exclusive, therefore producing a unqiue ID. 

    Since we are asked to use 64-bit UUID we will use the following property (Twitter Snow Flake)

    * **Time stamp** would be calculated in mili seconds using Epoch, giving us enough time and uniqueness until January 19, 2038. Since Epoch is **signed 32-bit** the most significant bit will not be used, giving us actually 31-bit to work with. Converting seconds to mili second gives us 10 extra bit, therefore the **max size of bits** we would need to represent Epoch in mili second is **41-bits**.  
    * **Node ID's** would be represented using **unsigned 10-bits** giving us range of 0-1023. 
    * This remains **12-bits** for **sequence number**, also unsigned.

    ```  
    timestamp: 41-bits = time().miliseconds()
    id: 23-bits = node.ID()
    (seq: 12-bits)++
    UID: 64-bits  = (timestamp << (63-41)) | (id << (63-41-10)) | (seq)
    return UID
    ```
2. Please explain how your solution achieves the desired performance i.e. 100,000 or more requests per second per node.  How did you verify this?   

    All the operations are in constant time.    

How do you manage uniqueness after a node crashes and restarts?  
How do you manage uniqueness after the entire system fails and restarts?  
[solution](#Final-Solution)