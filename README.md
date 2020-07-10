# Ducky
A ducky UDP cache server 🦆

### Protocol

Clients of ducky communicate with the server through UDP.
Ducky listens on port 20017 for incoming messages; since ducky doesn't support TCP connection, clients simply open a UDP socket 
with the given port and send the commands within a datagram.
Ducky focuses on velocity and bandwidth saving, reducing the latency overhead common to a TCP connection.

Data sent in ducky protocol is in ASCII.
Each message corresponds to a command that a client sends to the server or a response from the server;  
a message is made up of the name of the command, optional command parameters, and the structured data that clients want to store or retrieve from ducky.
A message is always terminated by the sequence of \r\n characters that determines where the data blocks end.
Each response from the server has a status code that specifies the result of the command:
200 for a successful operation, 500 for an errored operation. 


### Keys and memory structure

Data stored by ducky is uniquely identified with the help of a key.  
A key is a string which uniquely identifies the data for clients
that wants to store and retrieve it.
The maximum length limit of a key is 100 characters.
Internally ducky uses a hashmap data structure to store the data.  

### Commands

Ducky supports only two commands, SET to store some unstructured data identified by a key and GET to retrieve some data corresponding
to a specific key.
The semantics is the following:  

#### SET

```
SET [KEY] [TTL] data
```

#### GET

```
GET [key]
```