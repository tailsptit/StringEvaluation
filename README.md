# StringEvaluation

## Introduction
- Program is designed for evaluating value of an expression, using client-server model.
- Client sends a string expression to server using TCP protocol (such as netcat). 
- Server accepts & evaluates the string expression and returns the value to client  
- TCP Socket Server using C++14
- Support 10K connection clients

## Environment
- linux
- gcc 9.3.0
- cmake 3.16.3
- make 4.2.1

## Build
#### Get source code
    git clone https://github.com/tailsptit/StringEvaluation.git
#### Build project
##### Open terminal, go to StringEvaluation path, execute commands
    cmake .
    make

## Run
#### Generate data only (not start server)
###### Generate data and print result to terminal, run command
    ./StringEvaluation -run-mode 1 -num-expression A -max-num-operand B 
                
        -num-expression flag: number of expressions will be generated
        -max-num-operand flag: maximum of operands in each expression
        
###### Generate data and save to file, run command
    ./StringEvaluation -run-mode 1 -num-expression A -max-num-operand B -file FILE 
        
        -num-expression flag: number of expressions will be generated
        -max-num-operand flag: maximum of operands in each expression
        -file flag: file path
###### Example
    ./StringEvaluation -run-mode 1 -num-expression 1 -max-num-operand 10
    ./StringEvaluation -run-mode 1 -num-expression 4 -max-num-operand 20 -file "/home/tails/Documents/Project/C++/test0.txt" 

#### Start server only (not gen data)
###### From StringEvaluation project path, run command
    ./StringEvaluation -run-mode 2 -port PORT -pool-size POOSIZE
            
            - flag -port: server tcp port
            - flag --pool-size: number of threads      
###### Example
    ./StringEvaluation -run-mode 2 -port 8081 -pool-size 5
#### Generating data & start server
###### From StringEvaluation project path, run command
    ./StringEvaluation -run-mode 3 -port PORT -pool-size POOSIZE -num-expression A -max-num-operand B -file FILE  
            
            - flag -port: server tcp port
            - flag --pool-size: number of threads  
            - flag -file: file path
            - flag -num-expression: number of expressions will be generated
            - flag -max-num-operand: maximum of operands in each expression
###### Example
    ./StringEvaluation -run-mode 3 -port 8081 -pool-size 5 -num-expression 2 -max-num-operand 20
    ./StringEvaluation -run-mode 3 -port 8081 -pool-size 5 -num-expression 3 -max-num-operand 20 -file "home/tails/Documents/Project/C++/test0.txt" 

#### Run client
###### Send data by typing, run command
    nc localhost PORT
        
        - PORT: TCP server port
###### Send data from file, run command
    nc localhost PORT < FILE
        
        - PORT: TCP server port
        - FILE: Data file
               
###### Example
    nc localhost 8081
    
    nc localhost 8081 < "/home/tails/Documents/Project/C++/test0.txt"
 
## Technical

#### ThreadPool - Basic thread pooling functionality.
See a tiny example in main.cpp.

##### Create a thread pool:
    ThreadPool pool(size);

##### Submit a task to thread pool:
    pool.addTask(new CallBack(func, args...));

##### Stop the thread pool and wait for all tasks to finish:
    pool.stop();
    pool.awaitTermination();

#### EventManager - An event-driven thread pool.
EventManager is an IO event-driven thread pool. The difference with a regular thread pool is that it listens to IO descriptors with epolling in a seperate thread. The epolling thread would submit IO-ready tasks to its internal thread pool. EventManager utilizes epoll(4) and supports non-blocking IO. 

##### Create an EventManager:
    EventManager event_manager(thread_pool_size);

##### Submit a task without IO monitoring same as a regular FixedThreadPool:
    event_manager.addTask(new CallBack(func, args...));

##### Submit a task waiting for IO to be readable:
    event_manager.addTaskWaitingReadable(fd, new CallBack(&func, args...);

##### Submit a task waiting for IO to be writable:
    event_manager.AddTaskWaitingWritable(fd, new CallBack(&func, args...);

##### Remove a awaiting task from event manager:
    event_manager.removeAwaitingTask(fd);

An tiny echo server example is provided which uses EventManager for multi-task handling. Server side is purely non-block socket programmed and uses state machine to maintain status of each connection. A simple client sends concurrent requests for load testing.
    