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
#### Generate data and save to file
- ABC
#### Run server
- ABC
#### Run client
- StringEvaluation using the generated data
   nc -w 5 localhost port < file
   
- Example
   nc -w 5 localhost 8081 < /home/tails/Documents/Project/C++/test0.txt
 
## Technical
