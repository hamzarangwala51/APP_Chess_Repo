chessMPI: cGame.cpp cBoardState.cpp chessMPI.cpp
	mpic++ -g -o chessMPI *.cpp 

pvp:
	mpirun -x TERM -hostfile hostfile chessMPI 

pva:
	mpirun -x TERM -hostfile hostfile chessMPI pva

ava:
	mpirun  -x TERM -hostfile hostfile chessMPI ava
 
	
