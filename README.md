# Reservation System

Multi-process synchronization exercise modeled around a reservation system 

## Build

Clone the repository

```bash
cd reservation-system
mkdir build
cd build
cmake ..
make
cd ..
```

At this point, there is a new folder created in the root of the project, `bin` that contains the compiled executables. You can run them directly and run the test bash script.

## Example Ouptut

```bash
~/reservation-system $ ./bin/reservation_server &
~/reservation-system $ ./run
>> Placing reservation for Mike                                                 
>> Placing reservation for Alice
The demand for Mike was Accepted
>> Placing reservation for Nico
The demand for Alice was No Reply
>> Placing reservation for Bob
The demand for Nico was Accepted
>> Placing reservation for Oscar
The demand for Bob was Accepted
>> Placing reservation for Charlie
The demand for Oscar was Accepted
>> Placing reservation for Paul
The demand for Charlie was Accepted
>> Placing reservation for Dylan
The demand for Paul was Accepted
>> Placing reservation for Quentin
The demand for Dylan was Accepted
>> Placing reservation for Emilie
The demand for Quentin was Accepted
>> Placing reservation for Romeo
The demand for Emilie was Accepted
>> Placing reservation for Frank
The demand for Romeo was Accepted
>> Placing reservation for Serge
The demand for Frank was Denied
>> Placing reservation for Garry
The demand for Serge was Denied
>> Placing reservation for Tyler
The demand for Garry was Denied
>> Placing reservation for Hugo
The demand for Tyler was Denied
>> Placing reservation for Tabatha
The demand for Hugo was Denied
>> Placing reservation for Ingrid
The demand for Tabatha was Denied
>> Placing reservation for Victor
The demand for Ingrid was Denied
>> Placing reservation for Juliet
The demand for Victor was Denied
>> Placing reservation for William
The demand for Juliet was Denied
>> Placing reservation for Kate
The demand for William was Denied
>> Placing reservation for Xavier
The demand for Kate was Denied
>> Placing reservation for Luke
The demand for Xavier was Denied
>> Placing reservation for Yasmin
The demand for Luke was Denied

~/reservation-system $ ./bin/reservation_client
>> status	
-------------------------
| Section | Table | Name 
-------------------------
| A       | 100   | Mike
| A       | 101   | Nico
| A       | 102   | Bob
| A       | 103   | Oscar
| A       | 104   | Charlie
| A       | 105   | Paul
| A       | 106   | Dylan
| A       | 107   | Quentin
| A       | 108   | Emilie
| A       | 109   | Romeo
| B       | 200   | 
| B       | 201   | 
| B       | 202   | 
| B       | 203   | 
| B       | 204   | 
| B       | 205   | 
| B       | 206   | 
| B       | 207   | 
| B       | 208   | 
| B       | 209   | 
-------------------------
>> 
```
