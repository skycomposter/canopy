libcanopy.a : canopy_engine.o input.o utils.o
	ar rcs out/libcanopy.a out/canopy_engine.o out/input.o out/utils.o
	rm -f out/canopy_engine.o out/input.o out/utils.o

canopy_engine.o: src/canopy_engine.cc src/canopy_engine.h src/input.h \
				 src/input_event.h src/util/primitives.h src/util/utils.h
	g++ -c src/canopy_engine.cc -o out/canopy_engine.o

input.o: src/input.cc src/input.h src/input_codes.h src/input_event.h \
		 src/util/primitives.h
	g++ -c src/input.cc -o out/input.o

utils.o: src/util/utils.cc src/util/utils.h
	g++ -c src/util/utils.cc -o out/utils.o

clean :
	rm -f out/libcanopy.a out/canopy_engine.o out/input.o
