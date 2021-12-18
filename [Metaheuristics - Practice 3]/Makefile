BIN=bin
OBJ=obj
INC=include
SRC=src
DAT=datos

#----------- Lista de dependencias ------------#

all: $(BIN)/ejecutable $(BIN)/ejecutable2 $(BIN)/ejecutable3 $(BIN)/ejecutable4 

$(BIN)/ejecutable : $(OBJ)/ProblemaMDPSA.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable $(OBJ)/ProblemaMDPSA.o $(OBJ)/random.o 

$(BIN)/ejecutable2 : $(OBJ)/ProblemaMDPBMB.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable2 $(OBJ)/ProblemaMDPBMB.o $(OBJ)/random.o

$(BIN)/ejecutable3 : $(OBJ)/ProblemaMDPILS.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable3 $(OBJ)/ProblemaMDPILS.o $(OBJ)/random.o

$(BIN)/ejecutable4 : $(OBJ)/ProblemaMDPILS-SA.o $(OBJ)/random.o 
	g++ -o $(BIN)/ejecutable4 $(OBJ)/ProblemaMDPILS-SA.o $(OBJ)/random.o


#--------------------------------------------------#


obj/ProblemaMDPSA.o: $(SRC)/ProblemaMDPSA.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPSA.cpp -o $(OBJ)/ProblemaMDPSA.o -I$(INC)

obj/ProblemaMDPBMB.o: $(SRC)/ProblemaMDPBMB.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPBMB.cpp -o $(OBJ)/ProblemaMDPBMB.o -I$(INC)

obj/ProblemaMDPILS.o: $(SRC)/ProblemaMDPILS.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPILS.cpp -o $(OBJ)/ProblemaMDPILS.o -I$(INC)

obj/ProblemaMDPILS-SA.o: $(SRC)/ProblemaMDPILS-SA.cpp $(INC)/random.h 
	g++ -c $(SRC)/ProblemaMDPILS-SA.cpp -o $(OBJ)/ProblemaMDPILS-SA.o -I$(INC)


#--------------------------------------------------#


obj/random.o: $(SRC)/random.cpp $(INC)/random.h
	g++ -c $(SRC)/random.cpp -o $(OBJ)/random.o -I$(INC)


#------------ Limpieza -------------#

clean: mrproper
	echo 'Limpiando...'
	rm obj/*.o
	

mrproper: 
	rm bin/*
