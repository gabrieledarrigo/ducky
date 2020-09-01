 ![Ducky](https://user-images.githubusercontent.com/1985555/91746299-ac944a80-ebbc-11ea-808f-bfae2586955b.png)  

A quack quacky UDP cache server 🦆, developed for the Networking course of ["Sicurezza dei Sistemi e Delle Reti Informatiche"](http://sicurezzaonline.di.unimi.it/) bachelor's degree program.  

![C/C++ CI](https://github.com/gabrieledarrigo/ducky/workflows/C/C++%20CI/badge.svg?branch=master)

## Rationale

Ducky is an network cache server written in C.  
It stores unstructured data sent from a client in memory, ready to be served when a client asks for it using a unique key.  
Ducky's principal purpose is to reduce networking and computation load from a server (for example, an API, a Web application, or a database) so that a client can store data that is accessed with a high frequency, increasing the overall performance of the system.  
Ducky is loosely inspired by projects like [Memcached](https://github.com/memcached/memcached) and [Redis](https://github.com/redis/redis).  
While Ducky's approach to the "key-value" server implementation is naive, it worked well as a demonstrative and learning project on how to build a simple networking server.  
Ducky's internal implementation is documented here: [protocol](https://github.com/gabrieledarrigo/ducky/blob/master/PROTOCOL.md).  

## Dependencies

Ducky is a portable project and has only one dependency:

- [greatest](https://github.com/silentbicycle/greatest), used to write the unit tests for the project


## Build and test

Ducky is built with an out-of-source approach using [CMake](https://cmake.org/cmake/help/v3.18/manual/cmake.1.html).  
To build Ducky first clone the repository and then, inside the Ducky folder, generate the _build tree_:

```
$ cmake -B ./build
```

Now you can build the project:

```
$ cmake --build ./build
```

To run the unit tests just use the following command:

```
$ cmake --build ./build --target test
```

## Run

To start Ducky simply enter in the `build/src` folder and run the executable:

```
$ cd build/src
$ ./ducky

14:00:57 INFO  Ducky up and running, listening on port 20017
```

Now you can start to store and or retrieve data from Ducky;
just open another terminal session and use Netcat to send commands via UDP:

```
$ nc -u localhost 20017
SET key data
201 CREATED
GET key
200 data
```

## Contributors

A huge thanks go to [Roberto Carrà](https://www.linkedin.com/in/robertocarra) for his precious work on the logo.
