# Ducky
A quack quacky UDP cache server 🦆

### Protocol

Clients of ducky communicate with the server through UDP.
Ducky listens on port 20017 for incoming messages; since ducky doesn't support TCP connection, clients simply open a UDP socket 
with the given port and send the commands within a datagram.
Ducky focuses on velocity and bandwidth saving, reducing the latency overhead of a classic TCP connection.

Data sent in ducky protocol (both requests and respondes) is in ASCII.
Each message corresponds to a command that a client sends to the server or a response from the server;  
a message is made up of the name of the command, optional command parameters, and the structured data that clients want to store or retrieve from ducky.
A message is always terminated by the sequence of \r\n characters that determines where the data blocks end.
Each response from the server has a status code that specifies the result of the command:
200 for a successful operation, 500 for an errored operation. 
One client/server session has the following lifecycle:  

1. The server listens on port 20017.
2. A client opens a connection and sends a UDP datagram to the server.
3. When the server receives the datagram tries to parse it into a known command.
4. If the command is not recognized the server responds with a proper status code/error message.
5. If the command is recognized as a GET the server tries to return the request data to the client.
    - If the operation is successful the server returns the data along with success status code. 
    - Otherwise, it responds with a proper status code/error message.
6. If the command is recognized as a SET the server tries to persist the data into the memory.
    - If the GET operation is successful the server returns a response with a success status code,
    - Otherwise, it responds with a proper status code/error message.


#### Keys and memory structure

Data stored by ducky is uniquely identified with the help of a key.  
A key is a string which uniquely identifies the data for clients
that wants to store and retrieve it.
The maximum length limit of a key is 100 characters.  
The maximum size for data to be stored is 1MB.   
Internally ducky uses a hashmap data structure to store the data.  

#### Commands

Ducky supports only two commands, SET to store some unstructured data identified by a key and GET to retrieve some data corresponding
to a specific key.
The semantics is the following:  

##### SET

```
SET key data
```

##### GET

```
GET key
```

#### Response and status codes

Each ducky response is made up of a status code and an optional payload.  
Status codes are divided into two families:
The 2xx status codes indicate that the request has succeeded.
The 5xx status codes indicate that the server encountered an error.  

##### 200 (OK)
Is sent after a successful GET operation.

```
200 OK data
```

##### 201 (Created)
Is sent after a successful SET operation.

```
201 CREATED
```

##### 500 (Generic error)
Is sent after the server encounters a generic, not known, error.

```
500 ERR_GENERIC_ERROR
```

##### 501 (Cannot recv)
The server cannot receive the data from the client.

```
501 ERR_CANNOT_RECV
```

##### 502 (Command not recognize)
The server is not able to parse or recognize that command received by the client.

```
502 ERR_COMMAND_NOT_RECOGNIZED 
```

##### 503 (Max data size)
The data attached to the SET command is greater than the maximum allowed data size (1MB).

```
503 ERR_MAX_DATA_SIZE 
```

##### 504 (Key length)
The specified key of the GET or SET command is greater than the maximum allowed length (100 characters).

```
504 ERR_KEY_LENGTH
```

##### 505 (No key)
The GET or SET commands are recognized but the key is missing.

```
505 ERR_NO_KEY 
```

##### 506 (No data)
The SET commands haven't attached data.

```
506 ERR_NO_DATA
```
