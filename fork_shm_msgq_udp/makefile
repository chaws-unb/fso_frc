# Sender
SENDER = sender
SENDER_SRC = sender.c msg_sender.c udp_client.c
SENDER_DEPS = msg.h udp.h
SENDER_INC = -I.

# Receiver
RECEIVER = receiver
RECEIVER_SRC = receiver.c msg_receiver.c udp_server.c
RECEIVER_DEPS = msg.h udp.h
RECEIVER_INC = -I.

# General settings
CC = gcc
CFLAGS = -lpthread $(DEBUGMODE)

all: receiver sender

receiver: $(RECEIVER_SRC) $(RECEIVER_DEPS)
	$(CC) -o $(RECEIVER) $(RECEIVER_SRC) $(RECEIVER_INC) $(CFLAGS)

sender: $(SENDER_SRC) $(SENDER_DEPS)
	$(CC) -o $(SENDER) $(SENDER_SRC) $(SENDER_INC) $(CFLAGS)

udp: udp_server.c udp_client.c udp.h msg.h
	$(CC) -o udp_server udp_server.c -I. $(CFLAGS)
	$(CC) -o udp_client udp_client.c -I. $(CFLAGS)

clean:
	rm $(RECEIVER) $(SENDER)
