CXX = g++ -std=c++11 -Ofast -funroll-all-loops
CXXFLAGS = -g -Wall -I.

TARGETS = tsp

SOURCES := solucion
OBJECTS := $(addsuffix .o, $(SOURCES))

#============================================================
#=====================   COMPILATION   ======================
#============================================================
.SILENT:

# all / default
#
default : $(TARGETS)

# targets rule
#
tsp : $(OBJECTS) main.o
	@echo Building target: $@
	$(CXX) $(CXXFLAGS) $^ -o $@

# objects rule
#
$(OBJ)/%.o : $(SRC)/%.cpp
	@echo Building object: $@
	$(CXX) $(CXXFLAGS) $< -c -o $@

#============================================================
#======================   CLEANNING  ========================
#============================================================

.PHONY : clean
remake : clean default

clean :
	@echo Removing content ...
	$(RM) $(TARGETS) *.o
