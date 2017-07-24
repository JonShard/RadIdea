all:

		clear
		g++ -c Source/Main.cpp -o Output/Main.o


		g++ -c Source/Player.cpp -o Output/Player.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c Source/HelpFunctions.cpp -o Output/HelpFunctions.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c Source/ParticleSystem.cpp -o Output/ParticleSystem.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c Source/Projectile.cpp -o Output/Projectile.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c Source/Mine.cpp -o Output/Mine.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		g++ -c Source/StateMachine.cpp -o Output/StateMachine.o	-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system


		echo "\n\n"

		g++ Output/Main.o -o Game Output/Player.o Output/HelpFunctions.o Output/ParticleSystem.o Output/Projectile.o Output/Mine.o Output/StateMachine.o -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
		
		./Game
