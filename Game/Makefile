all:

		clear
		g++ -c Source/Main.cpp -o Output/Main.o


		g++ -c Source/Player.cpp -o Output/Player.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c Source/HelpFunctions.cpp -o Output/HelpFunctions.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

		echo "\n\n"

		g++ Output/Main.o -o Game Output/Player.o Output/HelpFunctions.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		
		./Game
