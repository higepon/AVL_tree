TARGET  = ./test
SOURCES = test.cpp
CXXFLAGS = -Wall -g -coverage

OBJECTS = $(SOURCES:.cpp=.o)

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -lgcov -lgtest -lpthread -o $(TARGET)

check :all
	$(TARGET)
	@gcov test.gcda | grep avl_tree -B2 | grep '%'

.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

depend:
	$(CXX) -MM $(INCLUDE) $(CXXFLAGS) $(SOURCES) > dependencies

check-syntax:
	LANG=C $(CXX) -o nul -fsyntax-only $(CXXFLAGS) $(INCLUDE) -S ${CHK_SOURCES} && python $(MONADIR)/tool/cpplint.py ${CHK_SOURCES}

clean :
	rm -f $(OBJECTS) $(TARGET) *.gcov *.gcda *.gcno dependencies

-include dependencies
