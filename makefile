CXXFLAGS = -Wall -g -Iinclude -Llib
LDFLAGS = -lfreeglut -lopengl32 #-Wl,--subsystem,windows
OBJS = main.o graphicobject.o fluid.o field.o rigidbody.o vector3.o quaternion.o

Flowtopia: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

main.o: src/main.cpp src/graphicobject.cpp src/fluid.cpp src/field.cpp src/helpers/vector3.cpp src/helpers/quaternion.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

graphicobject.o: src/graphicobject.cpp src/fluid.cpp src/helpers/vector3.cpp src/helpers/quaternion.cpp
	$(CXX) $(CXXFLAGS) -c src/graphicobject.cpp

fluid.o: src/fluid.cpp src/field.cpp src/rigidbody.cpp src/helpers/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/fluid.cpp

field.o: src/field.cpp src/helpers/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/field.cpp

rigidbody.o: src/rigidbody.cpp src/field.cpp
	$(CXX) $(CXXFLAGS) -c src/rigidbody.cpp

vector3.o: src/helpers/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/helpers/vector3.cpp

quaternion.o: src/helpers/quaternion.cpp src/helpers/vector3.cpp
	$(CXX) $(CXXFLAGS) -c src/helpers/quaternion.cpp

clean:
	-rm -f *.o core *.core
