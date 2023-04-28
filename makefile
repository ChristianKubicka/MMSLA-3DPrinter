
# Constants
CC = g++
CPP = ./src/CppFiles/
HPP = ./src/HppFiles/
TMP = ./src/Tmp/
EXE = printer

$(EXE): $(TMP)StepperMotor.o $(TMP)BeamBreakSensor.o $(TMP)Printer.o $(TMP)ConfigurationParser.o $(TMP)UVLamp.o $(TMP)Display.o $(CPP)PrinterRunner.cpp
	$(CC) $(CPP)PrinterRunner.cpp $(TMP)StepperMotor.o $(TMP)BeamBreakSensor.o $(TMP)Printer.o $(TMP)UVLamp.o $(TMP)Display.o $(TMP)ConfigurationParser.o -lwiringPi -o $(EXE) -I$(HPP)

$(TMP)Printer.o: $(CPP)Printer.cpp $(HPP)Printer.hpp
	mkdir -p $(TMP)
	$(CC) -c $(CPP)Printer.cpp -o $(TMP)Printer.o -I$(HPP)

$(TMP)ConfigurationParser.o: $(CPP)ConfigurationParser.cpp $(HPP)ConfigurationParser.hpp
	mkdir -p $(TMP)
	$(CC) -c $(CPP)ConfigurationParser.cpp -o $(TMP)ConfigurationParser.o -I$(HPP)

$(TMP)StepperMotor.o: $(CPP)StepperMotor.cpp $(HPP)StepperMotor.hpp
	mkdir -p $(TMP)
	$(CC) -c $(CPP)StepperMotor.cpp -o $(TMP)StepperMotor.o -I$(HPP)

$(TMP)BeamBreakSensor.o: $(CPP)BeamBreakSensor.cpp $(HPP)BeamBreakSensor.hpp
	mkdir -p $(TMP)
	$(CC) -c $(CPP)BeamBreakSensor.cpp -o $(TMP)BeamBreakSensor.o -I$(HPP)

$(TMP)UVLamp.o: $(CPP)UVLamp.cpp $(HPP)UVLamp.hpp
	mkdir -p $(TMP)
	$(CC) -c $(CPP)UVLamp.cpp -o $(TMP)UVLamp.o -I$(HPP)

$(TMP)Display.o: $(CPP)Display.cpp $(HPP)Display.hpp
	mkdir -p $(TMP)
	$(CC) -c $(CPP)Display.cpp -o $(TMP)Display.o -I$(HPP)

clean:
	-rm -r $(TMP)

rclean: clean
	-rm -r $(EXE)
