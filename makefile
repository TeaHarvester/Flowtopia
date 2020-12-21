CXXFLAGS = -Wall -g -Iinclude -Llib
LDFLAGS = -lfreeglut -lopengl32 #-Wl,--subsystem,windows
OBJS = main.o graphicobject.o fluid.o field.o vector3.o quaternion.o

Flowtopia: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

main.o: src/main.cpp src/graphicobject.cpp src/fluid.cpp src/field.cpp src/vector3.cpp src/quaternion.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

graphicobject.o: src/graphicobject.cpp src/fluid.cpp src/vector3.cpp src/quaternion.cpp
	$(CXX) $(CXXFLAGS) -c src/graphicobject.cpp

fluid.o: src/fluid.cpp src/field.cpp src/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/fluid.cpp

field.o: src/field.cpp src/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/field.cpp

vector3.o: src/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/vector3.cpp

quaternion.o: src/quaternion.cpp src/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/quaternion.cpp

clean:
	-rm -f *.o core *.core
