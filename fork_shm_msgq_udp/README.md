This is a simple sender/receiver program implemented with threads, message queue and udp  

The process is preety simple. First, compile it with `make all` and you'll have both sender and receiver programs.  

This is how the program behaves:  
*	./receiver
	*	Starts a parent function which creates a new message queue with the key "msg_receiver.c"
	*	The main program now waits for a new message from the queue
	*	A child thread is initialized, it connects to the created message queue and then waits for a udp package to arrive
*	./sender
	*	Starts a parent function which creates a new message queue with the key "msg_sender.c"
	*	The main program now waits for user input
	*	Meanwhile a child thread is initialized, it connect to parent's message queue
	*	As the user inputs something from stdin, a string is sent to the queue
	*	The child thread now reads the string from the queue and send it through udp via: 127.0.0.1:9002
*	./receiver
	*	The child thread now reads a string from udp and add it to the message queue
	*	The main flow now can continue, once the message is poped from the queue and printed to the screen

Still don't know why, there's a segmentation fault when ./receiver stops. Gotta figure this out