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

#### Run
###### From StringEvaluation project path, run command
    ./StringEvaluation -port PORT -pool-size SIZE
            
            -port: server tcp port. Default PORT = 8081
            -pool-size: number of threads. Default SIZE = 7       
###### Example
    ./StringEvaluation -port 8081 -pool-size 5
    
#### Generate data
###### From StringEvaluation project path, run command
    ./StringEvaluation -gen-data GEN -num-expression NUM_EXPS -num-operand NUM_OPS -min-operand MIN_OPS -max-operand MAX_OPS -allow-bracket BRACKET -operator OPS -file FILE  
        
    Parameter is optional
        -gen-data: generating data. Default GEN=true
        -num-expression: number of expressions will be generated. Default NUM_EXPS = 1
        -num-operand: numer of operands in expression. Default NUM_OPS = 10
        -min-operand: minimum of operand. Default MIN_OPS = 0. Should be positive
        -max-operand: maximum of operand. Default MAX_OPS = 1000. Should be greater than MIN_OPS 
        -allow-bracket: allow bracket in expression or not. Default BRACKET = true
        -operator: allow operators in expression, it is subset of [+, -, *, /]. Default OPS = +-*/
        -file: absolute filename
###### Example
    ./StringEvaluation -gen-data true -num-expression 1 -num-operand 20 -min-operand 0 -max-operand 2000 -allow-bracket true -operator +-
    ./StringEvaluation -gen-data true -num-expression 1 -num-operand 20 -min-operand 0 -max-operand 2000 -allow-bracket true -operator +- -file "/home/tails/Documents/Project/C++/test0.txt"  

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

#### ThreadPool
##### Create a thread pool:
    ThreadPool threadPool(size);

##### Submit a task to the thread pool
    pool.addTask(new CallBack(func, args...));

##### Stop the thread pool
    pool.stop();
    pool.awaitTermination();

#### EventManager
    EventManager is an IO event-driven thread pool, support non-blocking IO 
    The difference with ThreadPool is that it listens to IO descriptors with epolling in a seperate thread. 
    The epolling thread would submit IO-ready tasks to its internal ThreadPool. 

##### Create an EventManager:
    EventManager eventManager(threadPoolSize);

##### Submit a task without IO monitoring same as a regular FixedThreadPool:
    eventManager.addTask(new CallBack(func, args...));

##### Submit a task waiting for IO to be readable:
    eventManager.addTaskWaitingReadable(fd, new CallBack(&func, args...);

##### Submit a task waiting for IO to be writable:
    eventManager.addTaskWaitingWritable(fd, new CallBack(&func, args...);

##### Remove a awaiting task from event manager:
    eventManager.removeAwaitingTask(fd);

#### TCP Server
    An TCP server using EventManager for multi-task handling. 
    Server is non-block socket and use state machine to maintain status of each connection
    