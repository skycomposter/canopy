# General compilation flags.
CXXFLAGS = -Wall -Wextra -std=c++17
# Flags that make sure all headers can be found correctly.
CPPFLAGS = -I./include -I./src

# All .cc files.
SRCS = src/canopy_engine.cc src/input/input.cc src/state/game_state.cc \
       src/util/utils.cc
# One .o file for each .cc file.
OBJS = $(SRCS:.cc=.o)
# The built library binary.
BINARY = lib/libcanopy.a

# Builds the binary and cleans up the intermediate object files.
lib: $(BINARY)
	rm -f $(OBJS)

# Builds the library and all intermediate files.
$(BINARY): $(OBJS)
	ar rcs $(BINARY) $(OBJS)

# Sets up a .o target for any .cc file in the project 
%.o: %.cc
	g++ $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Remove both binary and intermediate object files.
clean:
	rm -f $(OBJS) $(BINARY)
