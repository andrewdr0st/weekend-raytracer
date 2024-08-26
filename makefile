

default: main.cpp
	g++ -o raytrace main.cpp

clean: 
	rm -f *.exe *.ppm