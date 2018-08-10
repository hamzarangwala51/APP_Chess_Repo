chessMPI: cGame.cpp cBoardState.cpp chessMPI.cpp cHash.cpp
	mpic++ -g -o chessMPI *.cpp 

pvp:
	mpirun -x TERM -hostfile hostfile chessMPI 

pva:
	mpirun -x TERM -hostfile hostfile chessMPI pva

ava:
	mpirun -x TERM -hostfile hostfile chessMPI ava

ava2: 
	mpirun -np 2 -x TERM -hostfile hostfile chessMPI ava

valgrind:
	mpirun -x TERM -hostfile hostfile valgrind ./chessMPI ava

valgrind2:
	mpirun -np 2 -x TERM -hostfile hostfile valgrind ./chessMPI ava

 
	
