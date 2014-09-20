
# Sender
SENDER = sender
SENDER_SRC = sender.c msg_sender.c udp_client.c
SENDER_DEPS = msg.h udp.h
SENDER_INC = -I.

# Receiver
RECEIVER = sender
RECEIVER_SRC = receiver.c msg_receiver.c udp_server.c
RECEIVER_DEPS = msg.h udp.h
RECEIVER_INC = -I.

# General settings
CC = gcc
CFLAGS = 

all: receiver sender

receiver:
	$(CC) -o $(RECEIVER) $(RECEIVER_SRC) $(RECEIVER_INC) $(CFLAGS)

sender:
	$(CC) -o $(SENDER) $(SENDER_SRC) $(SENDER_INC) $(CFLAGS)

