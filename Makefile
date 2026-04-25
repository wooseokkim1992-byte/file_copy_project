
DIR_PATH := .
FILE_SEARCH := file_search
all : $(DIR_PATH)/$(FILE_SEARCH).o $(DIR_PATH)/main

$(DIR_PATH)/main.o: $(DIR_PATH)/main.c
	gcc -c $^ -o $@

$(DIR_PATH)/$(FILE_SEARCH).o : $(DIR_PATH)/$(FILE_SEARCH).c
	gcc -c $< -o $@

$(DIR_PATH)/main : $(DIR_PATH)/main.o $(DIR_PATH)/$(FILE_SEARCH).o
	gcc $^ -o $@

clean :
	rm -f $(DIR_PATH)/*.o
	rm -f main