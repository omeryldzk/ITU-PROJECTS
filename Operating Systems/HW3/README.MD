# Concurrent Webserver ⭐

This project was part of the course BLG312E : Operating Systems offered at ITU in 2023-24 by Prof. Kemal Bıçakçı. 

A multi-threaded HTTP server implementing First in Fist out (FIFO), Recent File First (RFF) and Shortest File Name First (SFNF) scheduling policies on the server. 


## Feature Checklist ✅
```
✅ Concurrent Web Server Architecture
✅ First in First out (FIFO) Scheduling Policy
✅ Shortest File First (SFF) Scheduling Policy
✅ Shortest File Name First (SFNF) Scheduling Policy
✅ Multi-Request Client
```

## Concurrent Architecture 🔥

The basic idealogy behind a concurrent server is the producer-consumer problem where each request from client behaves as a new job which is being produced in the form of a file desciptor. The worker threads are the consumers that pick the job from the data structure (buffer)  implemented according to the scheduling policy. The below image depicts the overall producer-consumer model on the server side - 

![alt text](./screenshots/producer_consumer_basic.png "Producer Consumer")

This is further elaborated in these pictures -

![alt text](./screenshots/Concurrency_Architecture.png "Architecture")

![alt text](./screenshots/Scheduling_Policy_API.png "Architecture")

Here, we can see that a client makes an HTTP request to the server (parent thread), this main thread on receive this request puts the request in the buffer data structure and goes immediately to listen for more request, hence the main thread **does not block** other coming requests. Also, at the time of launching the server, it creates worker threads which pick up the request from this buffer data structure where each request is expressed as a file descripter (integer) for that client. These worker threads upon picking up request start serving the requested file or the spin cgi program.
c
## Client 📲 

The client is multi-request that can send a number of requests concurrently to the server by creating a new thread for every request and sending a connection request to the server. Currently the client requests "HTML" and ".cgi" files stored at the server.

## Server 💻

The server is multithreaded that supports different scheduling policies such as FIFO, SFF and SFNF. 

The scheduling policy API implemented in the server is as shown below: 

![alt text](./screenshots/scheduling_APIs.png "Scheduling Policy")

![alt text](./screenshots/Scheduler_struct.png "Scheduler")

### Base Model 🚲

Base Model has been taken from: [OSTEP Webserver](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-webserver). The base webserver can only server a single client at a given time. It is a **non-concurrent server** and hence **blocks** other requests while it is serving the current one. 

_Note - This base model was provided to us as a starter template for this project._

### FIFO Model 🚁 

When a worker thread wakes, it handles the first request (i.e., the oldest request) in the buffer. Note that the HTTP requests will not necessarily finish in FIFO order; the order in which the requests complete will depend upon how the OS schedules the active threads.


![alt text](./screenshots/Queue_FIFO.png "Queue for FIFO")

### SFF Model 🚀

When a worker thread wakes, it handles the request for the smallest file. This policy approximates Shortest Job First to the extent that the size of the file is a good prediction of how long it takes to service that request. Requests for static and dynamic content may be intermixed, depending upon the sizes of those files. Note that this algorithm can lead to the starvation of requests for large files. 

![alt text](./screenshots/Heap_SFF.png "Heap for SFF")

### RFF Model ✈️

When a worker thread wakes, it handles the request for the file with the last modified file. This policy is kind of silly, but that is life.

### Running the Webserver ▶️

Your C program should be invoked as follows:
```
prompt> server [portnum] [threads] [buffers] [schedalg(extra)]
```
The command line arguments to your web server are to be interpreted as follows.
- **portnum**: the port number that the web server should listen on; the basic web server already handles this argument.
- **threads**: the number of worker threads that should be created within the web server. Must be a positive integer.
- **buffers**: the number of request connections that can be accepted at one time. Must be a positive integer. Note that it is not an error for more or less threads to be created than buffers.
- **schedalg**: the scheduling algorithm to be performed. Must be one of FIFO, RFF, or SFF.
For example, if you run your program as:
```
prompt> server 5003 8 16 RFF
```
then your web server will listen to port 5003, create 8 worker threads for handling http requests, allocate 16 buffers for connections that are currently in progress (or waiting), and use SFF scheduling for arriving requests.


### Running the client ⏩

* Note that the client is a multi-request client. Therefore, there is provision to send multiple files with the same client. 

Use the following command to run the client: 

```
prompt> ./wclient <host> <port> <filename(s)>
```
The command line arguments to your client are to be interpreted as follows.

- **host**: IP of the server. By default  `localhost/127.0.0.1`
- **port**: The port on which the server is listening. Default: 10000
- **filenames(s)**: Can provide paths to multiple files seperated by spaces. 

For example, you could run your program as:
```
prompt> ./wclient 127.0.0.1 8003 ./test/1.html ./test/2.html 

```
The above command will run a client that will request 2 files concurrently from the server listening on the port 8003. 


* In the case of concurrent server with a thread pool of size 20, the throughput for requests reach around 10 request/sec with the average response time being around 9.88 request/sec. This configuration has the maximum throughout of 10 as it should be ideally, since every request gets scheduled as soon as it arrives.
