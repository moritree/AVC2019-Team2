LDFLAGS =  -LE101
LIBS = -le101

runMain: runMain.o drive.o readCamera.o robot.o testCamera.o
	g++ $(LDFLAGS) $(LIBS) -o runMain_exec runMain.o drive.o readCamera.o robot.o testCamera.o

runMain.o: runMain.cpp include/robot.h include/readCamera.h include/drive.h
	g++ $(LDFLAGS) $(LIBS) -c runMain.cpp
drive.o: src/drive.cpp include/drive.h E101/libtest/E101.h
	g++ -c src/drive.cpp $(LDFLAGS) $(LIBS)
readCamera.o: src/readCamera.cpp include/readCamera.h E101/libtest/E101.h
	g++ -c src/readCamera.cpp $(LDFLAGS) $(LIBS)
robot.o: src/robot.cpp include/robot.h include/readCamera.h include/drive.h
	g++ -c src/robot.cpp
testCamera.o: src/testCamera.cpp include/testCamera.h
	g++ -c src/testCamera.cpp

clean: rm editrunMain.o drive.o readCamera.o robot.o testCamera.o
