# Protocol

Clients of Ducky communicate with the server through `UDP`.    
Ducky listens on port `20017` for incoming messages; since Ducky doesn't support TCP connection, clients simply open a UDP socket 
with the given port and send the commands within a datagram.  
Ducky focuses on throughput and bandwidth saving, reducing the latency overhead of a classic TCP connection.  

Data sent in Ducky protocol (both requests and responses) is in `ASCII`.  
Each message corresponds to a command that a client sends to the server or a response from the server;  
a message is made up of the name of the command, optional command parameters, and the structured data that clients want to store or retrieve from Ducky.  
A message is always terminated by a `\n` character that determines where the data blocks end.  
Data stored in Ducky is identified with the help of a key.  
A key is a string that uniquely identifies the data for clients that want to store and retrieve it.  
The maximum length limit of a key is **100** characters.  
The maximum size for a single item to be stored is **1MB**. 
Each response from the server has a status code that specifies the result of the command:  
`2xx` for a successful operation, `5xx` for an errored one.  
Ducky supports only two commands, `GET` and `SET`.  
One client/server session has the following lifecycle:  

1. The server listens on port `20017`.
2. A client opens a connection and sends a `UDP` datagram to the server.
3. When the server receives the datagram tries to parse it into a known command.
4. If the command is not recognized the server responds with a proper status code/error message.
5. If the command is recognized as a `GET` the server tries to return the request data to the client.
    - If the operation is successful the server returns the data along with the success status code. 
    - Otherwise, it responds with a proper status code/error message.
6. If the command is recognized as a `SET` the server tries to persist the data into the memory.
    - If the `SET` operation is successful the server returns a response with a success status code.
    - Otherwise, it responds with a proper status code/error message.

Here there's a more  illustrative diagram:  

![Diagram](https://user-images.githubusercontent.com/1985555/91744013-14e12d00-ebb9-11ea-8300-7eb5b7331949.jpg)

## Network I/O

Designers of networking software can choose various strategies on how to handle client connections and input/output from/towards them.  
Notably, these strategies are:  

- Fork multiple _child processes_, one per client, to achieve concurrency and serve multiple requests.
- Spawn multiple _threads_, one per client, to achieve concurrency, and serve multiple requests.
- Use asynchronous, non-blocking I/O, using [kqueue(2)](https://man.openbsd.org/kqueue), or [epoll(4)](https://man7.org/linux/man-pages/man7/epoll.7.html).
- Use synchronous I/O multiplexing using [select(2)](https://man7.org/linux/man-pages/man2/select.2.html) vs [poll(2)](https://man7.org/linux/man-pages/man2/poll.2.html).

Even well known, battle-tested, and production-ready servers use different approaches.  
For example, [Apache HTTP Server](https://httpd.apache.org/) can handle concurrency both with forking child processes (via [prefork](https://httpd.apache.org/docs/2.4/mod/prefork.html) module) or by spawning multiple system's threads (via [worker](https://httpd.apache.org/docs/2.4/mod/worker.html) module), one per each incoming connection.  
[Apache Tomcat](https://httpd.apache.org/), the  Java Servlet web server, use a multithread approach for its HTTP Connector, using one thread per request up to the maximum configured number of threads.  
Other servers or backends use the third approach, basing their concurrency model on asynchronous, non-blocking I/O, implementing an [event loop](https://en.wikipedia.org/wiki/Event_loop) (a design pattern that waits for and dispatches events or messages) to handle requests from clients.   
Usually, an event loop is implemented using a library or a framework that abstracts away the underlying kernel system call (kqueue(2), epoll(4), event completions) and offers high-level APIs to handle events on file descriptors.  
For example, Memcached uses [libevent](https://libevent.org/), an event notification library, to implement its event loop, 
while Node.Js, the famous JavaScript runtime built on Chrome's V8 engine uses [libuv](https://github.com/libuv/libuv).  

While these solutions are far more efficient, well tested, and probably more elegant I decided to not use any framework; Ducky uses the traditional select(2) system call to be notified when a file descriptor (a client connection) is ready for reading.  
select(2) performances are poor in comparison to the other strategies we just illustrated; it works linearly, so the more file descriptors select(2) is required to handle, the slower the system gets.  
Depending on the hardware specification Ducky (and so all applications that uses select(2)) can reach few hundreds of open file descriptors before the mere waiting for their activity becomes a bottleneck.  
Nevertheless, select(2) has great portability (it's implemented almost everywhere) and served well in designing a simple memory cache server like Ducky.

## Memory structure  

Internally Ducky uses a [hash table](http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap12.htm) data structure.  
It offers an O(1) algorithmic complexity for both storing and retrieving data within a given key.  
There are several ways to implement a hash table data structure, especially on how to handle element collision.  
Ducky uses an _open-addressed_, _double-hashed_ hash table:  
instead of using a linked list for each bucket of the hash table, an _open-addressed_ implementation stores the element in the hash table itself.  
That is, each table bucket contains either an element of the dynamic set or `NULL`;  
when searching for an item with a given key, the hash table is systematically examined, until the desired item is found in one of its buckets or it is clear that it is not in the table. 
There are no lists or elements stored outside the table, as there are in chaining. 
The index that points the position in which the element needs to be inserted is computed by a double hash function with the following form:

```
h(k, i) = (h1(k) + ih2(k)) mod m
```

where _h1_ and _h2_ are the computation of a hash function that:

- Takes the string _k_ as an input.
- Converts it into a large integer number.
- Reduces the size of the integer to a fixed range, by taking its remainder mod m, where m is the number of buckets of the hash table.
- Returns the reduced integer.

When a collision happens, the collided item is placed in some other bucket in the hash table, depending on the result of the double hash function.  
You should note how the index of the new position depends on the number of _i_ collisions.  
While an open-addressed hash table can fill up its space Ducky implementation can resize itself when the load of the data structure is above 70%.

## Commands

As we said Ducky supports only two commands:  
SET to store some unstructured data identified by a key and GET to retrieve some data corresponding
to a specific key.  
The semantics is the following:  

#### SET

```
SET key data
```

#### GET

```
GET key
```

## Response and status codes

Each Ducky response is made up of a status code and an optional payload; its parsing is up to the client.  
Status codes are divided into two families:

- The 2xx status codes indicate that the request has succeeded.
- The 5xx status codes indicate that the server encountered an error.  

##### 200 (OK)
Is sent after a successful GET operation.

```
200 OK data
```

##### 201 (Created)
Is sent after a successful SET operation.

```
201 STATUS_CREATED
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

##### 502 (Cannot send)
The server cannot send the data to the client.

```
502 ERR_CANNOT_SEND
```

##### 503 (Command not recognized)
The server is not able to parse or recognize the command received by the client.

```
503 ERR_COMMAND_NOT_RECOGNIZED 
```

##### 504 (Max data size)
The data attached to the SET command is greater than the maximum allowed data size (1MB).

```
504 ERR_MAX_DATA_SIZE 
```

##### 505 (Key length)
The specified key length of the GET or SET command is greater than the maximum allowed length (100 characters).

```
505 ERR_KEY_LENGTH
```

##### 506 (No key)
The GET or SET commands are recognized by the server but the key is missing.

```
506 ERR_NO_KEY 
```

##### 507 (No data)
The SET commands haven't attached any data.

```
507 ERR_NO_DATA
```

##### 508 (Not found)
Data for the given specified key cannot be found. 

```
508 ERR_NO_DATA
```

## References

Developing Ducky was both fun and formative because it was the cue to learn better how TCP and UDP networking works at the system level, and to gain a better understanding on subjects like concurrency models, blocking and non-blocking I/O, data structures.  
It was impossible to implement Ducky without some great books, articles, and materials that I used to dig deeper into the subject:  

- I must start with the great [C10K problem article](http://www.kegel.com/c10k.html) by Dan Kegel, full of concepts and references. It's a must-read!
- The great [Poll vs Select article](https://daniel.haxx.se/docs/poll-vs-select.html) by Daniel Stenberg
- The obvious [Cormen reference to the hash table](http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap12.htm) data structure
- The super cool [C Hash Table implementation](https://github.com/jamesroutley/write-a-hash-table) by James 明良 Routley, that I used as an example to implements the internal memory cache of Ducky
- A useful book on network programming [Hands-On Network Programming with C](https://www.packtpub.com/networking-and-servers/hands-network-programming-c)
- The essential [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/), really worth a read!
