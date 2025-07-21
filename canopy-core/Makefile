libcanopy.a : canopy_engine.o
	ar rcs out/libcanopy.a out/canopy_engine.o
	rm -f out/canopy_engine.o

canopy_engine.o: src/canopy_engine.cc src/canopy_engine.h
	 g++ -c src/canopy_engine.cc -o out/canopy_engine.o

clean :
	rm -f out/libcanopy.a out/canopy_engine.o
