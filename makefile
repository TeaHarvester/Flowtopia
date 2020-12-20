CXXFLAGS = -Wall -g -Iinclude -Llib
LDFLAGS = -lfreeglut -lopengl32 #-Wl,--subsystem,windows
OBJS = main.o graphicobject.o fluid.o field.o helpers.o

Flowtopia: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

main.o: src/main.cpp src/graphicobject.cpp src/fluid.cpp src/field.cpp src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

graphicobject.o: src/graphicobject.cpp src/fluid.cpp
	$(CXX) $(CXXFLAGS) -c src/graphicobject.cpp

fluid.o: src/fluid.cpp src/field.cpp src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/fluid.cpp

field.o: src/field.cpp src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/field.cpp

helpers.o: src/helpers.cpp
	$(CXX) $(CXXFLAGS) -c src/helpers.cpp

clean:
	-rm -f *.o core *.core
