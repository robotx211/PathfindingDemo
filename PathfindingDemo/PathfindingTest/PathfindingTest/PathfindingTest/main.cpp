///  @file main.cpp
///  @brief Starting point for the program, initialises all objects needed for the program to use

#include <iostream>
#include <SDL2\SDL.h>
#include <chrono>
#include <vector>

//Additional File Includes
#include "Node.h"
#include "NodeManager.h"
#include "PFAlgorithms.h"
#include "MovingChar.h"
#include "GridSetup.h"

#undef main

//Module Contents
void main(void)
{
	//initialise SDL, create window, renderer and event manager
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;

	SDL_Event ev;

	window = SDL_CreateWindow("PathfindingTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, NULL);
	renderer = SDL_CreateRenderer(window, -1, NULL);

	bool breakout;
	bool repeat = true;

	//create instance of the NodeManager, PFAlgorithms, GridSetup and MovingChar
	NodeManager nManager;
	PFAlgorithms PFManager;
	GridSetup gridSetupManager(&nManager);

	MovingChar testChar(renderer);

	//while user has not selected to exit
	while (repeat == true)
	{
		system("cls");
		
		//draw node map
		SDL_RenderClear(renderer);
		nManager.DrawNMap(renderer, true);

		//run grid setup
		gridSetupManager.Setup(renderer, &ev);

		//set start and goal nodes and NodeManager of the PFManager
		PFManager.SetStart(gridSetupManager.m_startNode);
		PFManager.SetGoal(gridSetupManager.m_goalNode);
		PFManager.SetNManager(&nManager);

		//if the PFManager found a path to the goal, draw it and then send the agent along it
		if (PFManager.RunAlgorithm(renderer, &ev) != 0)
		{
			nManager.DrawNMap(renderer, true);

			testChar.m_genPath(PFManager.m_startNode, PFManager.m_goalNode);

			breakout = false;

			system("cls");
			std::cout << "Press (1) to try again, press (2) to quit" << std::endl;

			//once path is given to the aganet, let it move along the path, unitl it reaches the goal
			//the user can then choose to set another grid and repath, or exit
			while (breakout == false)
			{
				SDL_RenderClear(renderer);
				while (SDL_PollEvent(&ev))
				{
					if (ev.type == SDL_KEYDOWN)
					{

						switch (ev.key.keysym.sym)
						{
						case SDLK_1:
							breakout = true;
							break;
						case SDLK_2:
							breakout = true;
							repeat = false;
							break;
						default:
							break;
						}

					}
				}

				testChar.Update();
				nManager.DrawNMap(renderer, false);
				testChar.Draw(renderer);

				SDL_RenderPresent(renderer);

				//nManager.DrawWeights(renderer);

				SDL_Delay(1000 / 120);
			}
		}
		//if the PFManager cannot find a path to the goal node, error is output (in PFManager) and then program waits for user decision, to set another grid and repath, or exit
		else
		{
			std::cout << "Press (1) to try again, press (2) to quit" << std::endl;
			breakout = false;

			while (breakout == false)
			{
				while (SDL_PollEvent(&ev))
				{
					if (ev.type == SDL_KEYDOWN)
					{
						switch (ev.key.keysym.sym)
						{
						case SDLK_1:
							breakout = true;
							break;
						case SDLK_2:
							breakout = true;
							repeat = false;
							break;
						default:
							break;
						}
					}
				}
			}		
		}
		//resets the node manager and PFManager, so they can be used again
		nManager.ClearNMap();
		PFManager.InitManager();
	}
	
	//destroy rendere and window when user exits
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

}