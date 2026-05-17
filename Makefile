.PHONY: all clean
.SUFFIXES:

DIR_PATH := .
FILE_SEARCH := file_search
SERVER := server
CLI := client
SOCKET_FN := socket_fn
CC := gcc
CFLAGS := -Wall 


all : $(SERVER) $(CLI) 

$(FILE_SEARCH).o : $(FILE_SEARCH).c 
	$(CC) $(CFLAGS) -c $< -o $@

$(SOCKET_FN).o: $(SOCKET_FN).c 
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER): $(SERVER).c $(SOCKET_FN).o $(FILE_SEARCH).o
	$(CC) $(CFLAGS) $^ -o $@

$(CLI) : $(CLI).c $(SOCKET_FN).o $(FILE_SEARCH).o
	$(CC) $(CFLAGS) $^ -o $@


clean :
	rm -f $(DIR_PATH)/*.o
	rm -f $(DIR_PATH)/$(SERVER)
	rm -f $(DIR_PATH)/$(CLI)
