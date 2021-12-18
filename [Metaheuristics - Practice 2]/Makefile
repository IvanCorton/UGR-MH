BIN=bin
OBJ=obj
INC=include
SRC=src
DAT=datos

#----------- Lista de dependencias ------------#

all: $(BIN)/ejecutable $(BIN)/ejecutable2 $(BIN)/ejecutable3 $(BIN)/ejecutable4 $(BIN)/ejecutable5 $(BIN)/ejecutable6 $(BIN)/ejecutable7

$(BIN)/ejecutable : $(OBJ)/ProblemaMDPAGG-uniforme.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable $(OBJ)/ProblemaMDPAGG-uniforme.o $(OBJ)/random.o 

$(BIN)/ejecutable2 : $(OBJ)/ProblemaMDPAGG-position.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable2 $(OBJ)/ProblemaMDPAGG-position.o $(OBJ)/random.o

$(BIN)/ejecutable3 : $(OBJ)/ProblemaMDPAGE-uniforme.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable3 $(OBJ)/ProblemaMDPAGE-uniforme.o $(OBJ)/random.o

$(BIN)/ejecutable4 : $(OBJ)/ProblemaMDPAGE-position.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable4 $(OBJ)/ProblemaMDPAGE-position.o $(OBJ)/random.o

$(BIN)/ejecutable5 : $(OBJ)/ProblemaMDPAM-1.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable5 $(OBJ)/ProblemaMDPAM-1.o $(OBJ)/random.o

$(BIN)/ejecutable6 : $(OBJ)/ProblemaMDPAM-2.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable6 $(OBJ)/ProblemaMDPAM-2.o $(OBJ)/random.o

$(BIN)/ejecutable7 : $(OBJ)/ProblemaMDPAM-3.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable7 $(OBJ)/ProblemaMDPAM-3.o $(OBJ)/random.o

#--------------------------------------------------#


obj/ProblemaMDPAM-1.o: $(SRC)/ProblemaMDPAM-1.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAM-1.cpp -o $(OBJ)/ProblemaMDPAM-1.o -I$(INC)


obj/ProblemaMDPAM-2.o: $(SRC)/ProblemaMDPAM-2.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAM-2.cpp -o $(OBJ)/ProblemaMDPAM-2.o -I$(INC)

obj/ProblemaMDPAM-3.o: $(SRC)/ProblemaMDPAM-3.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAM-3.cpp -o $(OBJ)/ProblemaMDPAM-3.o -I$(INC)






obj/ProblemaMDPAGE-position.o: $(SRC)/ProblemaMDPAGE-position.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAGE-position.cpp -o $(OBJ)/ProblemaMDPAGE-position.o -I$(INC)

obj/ProblemaMDPAGE-uniforme.o: $(SRC)/ProblemaMDPAGE-uniforme.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAGE-uniforme.cpp -o $(OBJ)/ProblemaMDPAGE-uniforme.o -I$(INC)


obj/ProblemaMDPAGG-uniforme.o: $(SRC)/ProblemaMDPAGG-uniforme.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAGG-uniforme.cpp -o $(OBJ)/ProblemaMDPAGG-uniforme.o -I$(INC)

obj/ProblemaMDPAGG-position.o: $(SRC)/ProblemaMDPAGG-position.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPAGG-position.cpp -o $(OBJ)/ProblemaMDPAGG-position.o -I$(INC)


#--------------------------------------------------#


obj/random.o: $(SRC)/random.cpp $(INC)/random.h
	g++ -c $(SRC)/random.cpp -o $(OBJ)/random.o -I$(INC)

#------------ Limpieza -------------#

clean: mrproper
	echo 'Limpiando...'
	rm obj/*.o
	

mrproper: 
	rm BIN/*
