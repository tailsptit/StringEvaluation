# StringEvaluation
- Program is designed for evaluating value of an expression, using client-server model.
- Client sends string expression using TCP. 
- Server accepts & evaluates expression and return value to client
  

## Introduction
- Using netcat tool as client to send string expression to server  
- Using C++14, TCP Socket Server
- Support 10K clients connection

## Environment
- linux
- gcc 9.3.0

## Build
#### Get source code
- git clone https://github.com/tailsptit/StringEvaluation.git
#### Build project
From StringEvaluation path, execute commands
- cmake .
- make  

## Run
#### Generate data only (not start server)
###### From StringEvaluation project path, run command
    -./StringEvaluation -run-mode 1 -num-expression A -max-num-operand B -file FILE 
        
        - flag -file: file path
        - flag -num-expression: number of expressions will be generated
        - flag -max-num-operand: maximum of operands in each expression
###### Example
    ./StringEvaluation -run-mode 1 -num-expression 3 -max-num-operand 20 -file "home/tails/Documents/Project/C++/test0.txt" 

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
    ./StringEvaluation -run-mode 3 -port 8081 -pool-size 5 -num-expression 3 -max-num-operand 20 -file "home/tails/Documents/Project/C++/test0.txt" 

#### Run client
###### Open terminal and run command
    nc -w TIME localhost PORT < FILE
        
        - flag -w: number of seconds, netcat close
        - flag PORT: TCP server port
        - flag FILE: Data file
###### Example
    nc -w 5 localhost 8081 < "/home/tails/Documents/Project/C++/test0.txt"
 
## Technical
