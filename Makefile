SRC          = log.cpp  
SRC_TEST     = log_test.cpp
EXE          = liblog.a 
EXE_TEST     = ./log_test
UNAME        = $(shell uname)

LIBS = 
#LIBS_TEST = -L. 
LIBS_TEST = -L. -llog

ifeq ($(UNAME), Darwin)

CPPFLAGS = -Wall -I. -g

endif

all: $(EXE)

$(EXE): $(SRC)

	c++ -g -c $(SRC) -o log.o $(CPPFLAGS) $(LIBS) 
	ar rcs $(EXE) log.o 
	-c++ $(SRC_TEST) $(CPPFLAGS) $(LIBS_TEST) -o $(EXE_TEST)

clean:
	-rm -rf $(EXE) 
	-rm -rf $(EXE_TEST) 
	-rm -rf *.o 

test:
	$(EXE_TEST) $(EXE) funky
	$(EXE_TEST) $(EXE) $(EXE_TEST)
