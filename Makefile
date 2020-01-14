main:
	g++ -pthread -I ../boost_1_71_0/ -o ./compiledProgramms/server_main_programm ./src/serverSide/server_main.cpp ./src/serverSide/server/madn_server.cpp ./src/serverSide/communicator/communicator.cpp ./src/game/madn_game.cpp ./src/game/com.cpp
	g++ -pthread -I ../boost_1_71_0/ -I ../SFML-2.5.1/include/ -o ./compiledProgramms/client_main_programm ./src/clientSide/client_main.cpp  ./src/clientSide/client/madn_client.cpp ./src/view/view.cpp ./src/clientSide/client/boardClient/madn_client_board.cpp -L ../SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system
