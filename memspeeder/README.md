# Memspeeder - Covert Channel Communication

Memspeeder is a utility for covert channel communication within the same computer. It encodes information on memory interface speed, and should work for example between virtual machines as long as same memory bus (or chip) is shared.

## Compatibility

Worked *twice* on my machine. First on thursday afternoon and then on the same evening.


## Mode of operation

The transmitter encodes the data by loading the memory bus by performin memcpy for a full second. Transmitting 1 equals to high bus load, which equals to low speed. Before transmitting the user data, 0xab (0b10101011) is sent which helps the receiver to catch up. This is called SOT (Start of Transmission). Bytes are sent one at the time; theres one start bit, eight data bits and one stop bits. So this resembls 8N1 asynchronous data.

The receiver measures the memory interface speed when the seconds are half. It has a really ugly state machine that attempts to keep track what's happening. It also prints out the current memory speed and how it interprets the bus.


## Demo Video

This demo shows communication between two virtual machines using memory bus speed as the medium.
<video src='memspeeder-vm-demo.mp4' width=360/>


## Usage

Build memspeeder:

```
gcc -O0 -o tx transmitter.c
gcc -O0 -o rx receiver.c
```

Receive data with:
```
./rx
```

Transmit data with:
```
./tx kissa
```


## Sample run
```
user@host:~/memspeeder$ ./tx kissa
bit: start
bit: 1
bit: 0
bit: 1
bit: 0
bit: 1
bit: 0
bit: 1
bit: 1
bit: stop
bit: start
bit: 0
bit: 1
bit: 1
bit: 0
bit: 1
bit: 0
bit: 1
bit: 1
bit: stop
bit: start
bit: 0
bit: 1
bit: 1
bit: 0
bit: 1
bit: 0
bit: 0
bit: 1
bit: stop
bit: start
bit: 0
bit: 1
bit: 1
bit: 1
bit: 0
bit: 0
bit: 1
bit: 1
bit: stop
bit: start
bit: 0
bit: 1
bit: 1
bit: 1
bit: 0
bit: 0
bit: 1
bit: 1
bit: stop
bit: start
bit: 0
bit: 1
bit: 1
bit: 0
bit: 0
bit: 0
bit: 0
bit: 1
bit: stop
```

```
user@host:~/memspeeder$ ./rx
5171 MB/s             message: 
6230 MB/s             message: 
6094 MB/s             message: 
6191 MB/s             message: 
6115 MB/s             message: 
5935 MB/s             message: 
4509 MB/s             message: 
6133 MB/s             message: 
3612 MB/s  is sot?    message: 
5875 MB/s  no sot     message: 
3329 MB/s  is sot?    message: 
6062 MB/s  no sot     message: 
3113 MB/s  is sot?    message: 
3559 MB/s  got sot    message: 
6047 MB/s             message: 
5845 MB/s             message: 
3688 MB/s  got start  message: 
6005 MB/s  rx bit 0   message: 
3077 MB/s  rx bit 1   message: 
3461 MB/s  rx bit 1   message: 
5390 MB/s  rx bit 0   message: 
3443 MB/s  rx bit 1   message: 
6112 MB/s  rx bit 0   message: 
2964 MB/s  rx bit 1   message: 
3409 MB/s  rx bit 1   message: k
6036 MB/s             message: k
5925 MB/s             message: k
3532 MB/s  got start  message: k
6170 MB/s  rx bit 0   message: k
2973 MB/s  rx bit 1   message: k
3489 MB/s  rx bit 1   message: k
6016 MB/s  rx bit 0   message: k
3547 MB/s  rx bit 1   message: k
5891 MB/s  rx bit 0   message: k
6144 MB/s  rx bit 0   message: k
3666 MB/s  rx bit 1   message: ki
5950 MB/s             message: ki
6104 MB/s             message: ki
3649 MB/s  got start  message: ki
5894 MB/s  rx bit 0   message: ki
2991 MB/s  rx bit 1   message: ki
3020 MB/s  rx bit 1   message: ki
3452 MB/s  rx bit 1   message: ki
6068 MB/s  rx bit 0   message: ki
5966 MB/s  rx bit 0   message: ki
2966 MB/s  rx bit 1   message: ki
3397 MB/s  rx bit 1   message: kis
5916 MB/s             message: kis
5822 MB/s             message: kis
3348 MB/s  got start  message: kis
5416 MB/s  rx bit 0   message: kis
2966 MB/s  rx bit 1   message: kis
3072 MB/s  rx bit 1   message: kis
3447 MB/s  rx bit 1   message: kis
6101 MB/s  rx bit 0   message: kis
5840 MB/s  rx bit 0   message: kis
3063 MB/s  rx bit 1   message: kis
3454 MB/s  rx bit 1   message: kiss
5884 MB/s             message: kiss
5966 MB/s             message: kiss
3350 MB/s  got start  message: kiss
5891 MB/s  rx bit 0   message: kiss
2985 MB/s  rx bit 1   message: kiss
3589 MB/s  rx bit 1   message: kiss
5872 MB/s  rx bit 0   message: kiss
6026 MB/s  rx bit 0   message: kiss
6158 MB/s  rx bit 0   message: kiss
5950 MB/s  rx bit 0   message: kiss
3522 MB/s  rx bit 1   message: kissa
6030 MB/s             message: kissa
5580 MB/s             message: kissa
6128 MB/s             message: kissa
6275 MB/s             message: kissa
6156 MB/s  
timeout.
k thx bye.
```
