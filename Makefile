main:
	g++ -pthread -I ../boost_1_71_0/ -o ./compiledProgramms/server_main_programm ./src/serverSide/server_main.cpp ./src/serverSide/server/madn_server.cpp ./src/serverSide/communicator/communicator.cpp ./src/game/madn_game.cpp
	g++ -pthread -I ../boost_1_71_0/ -o ./compiledProgramms/client_main_programm ./src/clientSide/client/madn_client.cpp