#!/bin/bash
var1=100000

for ((i=0; i<=100000; i=i+1))
do
  echo -en "GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: Go-http-client/1.1\r\nAccept-Encoding: gzip\r\n\r\n" | nc localhost 8080
  (( var1++ ))
done