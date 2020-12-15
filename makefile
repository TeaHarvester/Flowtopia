CXXFLAGS = -Wall -g -Iinclude -Llib
LDFLAGS = -lfreeglut -lopengl32 #-Wl,--subsystem,windows
OBJS = main.o field.o helpers.o

Flutopia: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

main.o: src/main.cpp src/field.cpp src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

field.o: src/field.cpp src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/field.cpp

helpers.o: src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/helpers.cpp

clean:
	-rm -f *.o core *.core
