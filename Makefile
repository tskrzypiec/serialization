#CXXFLAGS =	-O2 -g -std=c++11 -Wall -I/cygdrive/c/cygwin64/usr/local/include/ -fmessage-length=0

CXXFLAGS =	-O2 -g -std=c++11 -Wall -I/usr/local/include/ -fmessage-length=0



OBJS =		serialization.o

#LIBS = 		/cygdrive/c/cygwin64/usr/local/lib/libboost_serialization.a

LIBS = /usr/local/lib/libboost_serialization.a

TARGET =	serialization.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
