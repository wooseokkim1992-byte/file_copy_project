.PHONY: all clean
.SUFFIXES:

DIR_PATH := .
FILE_SEARCH := file_search
SERVER := server
CLI := client
CC := gcc
CFLAGS := -Wall 


all : $(SERVER) $(CLI) 

$(SERVER): $(SERVER).c
	$(CC) $(CFLAGS) $< -o $@

$(CLI) : $(CLI).c $(FILE_SEARCH).o 
	$(CC) $(CFLAGS) $^ -o $@

$(FILE_SEARCH).o : $(FILE_SEARCH).c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(DIR_PATH)/*.o
	rm -f $(DIR_PATH)/$(SERVER)
	rm -f $(DIR_PATH)/$(CLI)
