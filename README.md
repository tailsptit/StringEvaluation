# StringEvaluation
- Program is designed for evaluating value of an expression

## Introduction
- Server receive string expression from client, evaluate expression 
and return to client.
- Using netcat tool as client to send string expression to server  
- Using C++14, TCP Socket Server
- Support 10K clients connection

## Environment
- linux
- gcc 9.3.0

## Build
 From current project
 - mkdir build
         - cd build
         - cmake ..
         - make  
## To get source code from  GitHub
  - https://github.com/tailsptit/StringEvaluation.git
  
## Run
 ## To gen data
 ABC
 ### To run StringEvaluation using the generated data
 ABC

## Test 
#### Create a customer using CREATE API using curl command
   - Open terminal 
   nc -w 5 localhost port < file
   
   - Example
   nc -w 5 localhost 8081 < /home/tails/Documents/Project/C++/test0.txt

 
## Technical
