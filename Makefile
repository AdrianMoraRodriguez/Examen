# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática

CXX = g++						        
CXXFLAGS = -std=c++2a -g -w -pedantic
LDFLAGS = # The linker options (if any)		

.PHONY: Examen clean
	

Examen : Preguntas.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


clean:
	@ rm -f *.o Examen 

# The $@ and $< are called automatic variables. The variable $@ represents the name 
# of the target and $< represents the first prerequisite required to create the output file.