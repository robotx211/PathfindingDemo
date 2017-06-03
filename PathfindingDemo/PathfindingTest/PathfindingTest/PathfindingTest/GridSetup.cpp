///  @file GridSetup.cpp
///  @brief GridSetup class definition

#include <SDL2\SDL.h>
#include <vector>
#include <iostream>
#include <math.h>

#include "GridSetup.h"

//Additional File Includes
#include "Node.h"
#include "NodeManager.h"

//Module Contents
GridSetup::GridSetup(NodeManager *_NManager)
{
	m_NManager = _NManager;
}

GridSetup::~GridSetup()
{
	//deletes the start, goal and temp node pointers when the destructor is called
	delete(m_startNode);
	delete(m_goalNode);
	delete(m_tempNode);
}

void GridSetup::Setup(SDL_Renderer *_renderer, SDL_Event *_ev)
{
	//reset variables used to moniter setup completion, weights, threat levels and node pointers
	m_setupComplete = false;

	m_currentWeight = 0.0f;
	m_currentThreatLevel = 0.0f;

	m_startNode = NULL;
	m_goalNode = NULL;
	m_tempNode = new Node();

	//sets up a boolean to break out of the event handling
	bool breakout;

	//while setup has not been completed
	while (m_setupComplete == false)
	{
		//basic instructions, printed in the console, show user input options
		//the user presses a number button, corresponding to an option, and then can edit the node map
		std::cout << "Please choose what nodes to add, and enter 0 when finished (while SDL window is selected)" << std::endl;
		std::cout << "Start(1), Goal(2), Object(3), Weighted Node(4), Threat(5), Help(9)" << std::endl;
		std::cout << "Press backspace when setting multiple nodes (eg. Objects, Weighted Nodes etc.)" << std::endl;
		std::cout << "(After entering data in this console, please tab back into the SDL window)" << std::endl;
		std::cout << "(Note: If you add multiple values to the same node, it's colour will change..." << std::endl;
		std::cout << " ...but the values will remain)" << std::endl;
		breakout = false;

		//loops until a valid input is input by the user
		while (breakout == false)
		{
			//while user input is detected
			while (SDL_PollEvent(_ev) != 0)
			{
				//if user input is a keyboard keypress
				if (_ev->type == SDL_KEYDOWN)
				{
					//switch statement to run setup function corresponding to user input
					switch (_ev->key.keysym.sym)
					{
						case SDLK_1:
						{
							SetStart(_renderer, _ev);
							breakout = true;
							break;
						}
						case SDLK_2:
						{
							SetGoal(_renderer, _ev);
							breakout = true;
							break;
						}
						case SDLK_3:
						{
							SetObjects(_renderer, _ev);
							breakout = true;
							break;
						}
						case SDLK_4:
						{
							SetWeights(_renderer, _ev);
							breakout = true;
							break;
						}
						case SDLK_5:
						{
							SetThreat(_renderer, _ev);
							breakout = true;
							break;
						}
						case SDLK_9:
						{
							ShowHelp();
							breakout = true;
							break;
						}
						case SDLK_0:
						{
							//if user enters finished without a start and goal node set, return an error
							if (m_startNode == nullptr || m_goalNode == nullptr)
							{
								std::cout << "Please set a start and goal node" << std::endl;
							}
							else
							{
								breakout = true;
								m_setupComplete = true;
							}

							break;
						}
						default:
						{
							//if user enters invalid keyboard input
							system("cls");
							std::cout << "Invalid Entry, try again:" << std::endl;
							break;
						}
					}
				}
			}
		}
	}
}

void GridSetup::SetStart(SDL_Renderer *_renderer, SDL_Event *_ev)
{

	//allows the user to place a start node in the node map, by clicking on the corresponding grid position on the screen
	//the user can also press backspace to return to the setup menu

	system("cls");
	std::cout << "Set the start node (in SDL Window)" << std::endl;
	std::cout << "Press backspace to return to setup menu" << std::endl;

	bool breakout = false;

	//loops until a the user places a start node
	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{
			if (_ev->type == SDL_MOUSEBUTTONDOWN)
			{
				if (_ev->button.button == SDL_BUTTON_LEFT)
				{
					//if user attempts to place the start node on the goal node, output error
					if (m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size)) == m_goalNode)
					{
						std::cout << "Error: Start Node cannot be the same as the Goal Node!" << std::endl;
					}
					//if user attempt to place the start node on an object node, outpur error
					if (m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size))->m_isObject == true)
					{
						std::cout << "Error: Start Node cannot be placed on an Object Node!" << std::endl;
					}
					else
					{
						//if no start node has been placed before, create a new start node to place
						if (m_startNode == NULL)
						{
							m_startNode = new Node();
						}
						//else set the current start node to no longer be a start node
						else
						{
							m_startNode->ToggleStart();
						}

						//sets the start node to point to the selected node in the node map, and call ToggleStart() on it
						m_startNode = m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size));
						m_startNode->ToggleStart(); //resets node to be start with no other attributes
						//re-draws the grid with the updated node values
						m_NManager->DrawNMap(_renderer, true);

						//breakout to true to user return to setup menu
						breakout = true;
					}
					
				}
			}
			//if user presses backspace, return to setup menu
			if (_ev->type == SDL_KEYDOWN)
			{
				if (_ev->key.keysym.sym == SDLK_BACKSPACE)
				{

					breakout = true;
				}
			}
		}
	
	
	}
	//clears the console, and tells the user the start node was set
	system("cls");
	std::cout << "Start Node Set!" << std::endl;
	std::cout << std::endl;
	
}

void GridSetup::SetGoal(SDL_Renderer *_renderer, SDL_Event *_ev)
{
	//allows the user to place a goal node in the node map, by clicking on the corresponding grid position on the screen
	//works in the same way as SetStart();

	system("cls");
	std::cout << "Set the goal node (in SDL Window)" << std::endl;
	std::cout << "Press backspace to return to setup menu" << std::endl;
	bool breakout = false;
	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{
			if (_ev->type == SDL_MOUSEBUTTONDOWN)
			{
				if (_ev->button.button == SDL_BUTTON_LEFT)
				{
					//if user attempts to place the goal node on the start node, output error
					if (m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size)) == m_startNode)
					{
						std::cout << "Error: Goal Node cannot be the same as the Start Node!" << std::endl;
					}
					//if user attempt to place the goal node on an object node, outpur error
					if (m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size))->m_isObject == true)
					{
						std::cout << "Error: Goal Node cannot be placed on an Object Node!" << std::endl;
					}
					else
					{
						if (m_goalNode == NULL)
						{
							m_goalNode = new Node();
						}

						else
						{
							m_goalNode->ToggleGoal();
						}

						//sets the goal node to point to the selected node in the node map, and call ToggleGoal() on it
						m_goalNode = m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size));
						m_goalNode->ToggleGoal(); //resets node to be goal with no other attributes
						m_NManager->DrawNMap(_renderer, true);

						breakout = true;
					}
					
				}
			}
			if (_ev->type == SDL_KEYDOWN)
			{
				if (_ev->key.keysym.sym == SDLK_BACKSPACE)
				{
					breakout = true;
				}
			}
		}
	}
	system("cls");
	std::cout << "Goal Node Set!" << std::endl;
	std::cout << std::endl;

	
}

void GridSetup::SetObjects(SDL_Renderer *_renderer, SDL_Event *_ev)
{
	//allows the user to place object nodes in the node map, by clicking on the corresponding grid position on the screen
	//the user can press backspace to return to the setup menu at any time

	system("cls");

	std::cout << "Add object nodes (in SDL Window)" << std::endl;
	std::cout << "Press backspace to return to setup menu" << std::endl;
	bool breakout = false;
	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{
			if (_ev->type == SDL_MOUSEBUTTONDOWN)
			{
				//if user attempts to place an object node on the start or goal node, output error
				if (m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size)) == m_startNode || m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size)) == m_goalNode)
				{
					std::cout << "Error: Cannot set the start or goal nodes to objects!" << std::endl;
				}
				//else, sets the node selected to an object
				else
				{
					m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size))->ToggleObject();
					m_NManager->DrawNMap(_renderer, true);
				}
			}

			//only returns to the setup menu when backspace is pressed
			if (_ev->type == SDL_KEYDOWN)
			{
				if (_ev->key.keysym.sym == SDLK_BACKSPACE)
				{
					breakout = true;
				}
			}
		}

	}

	system("cls");
}

void GridSetup::SetWeights(SDL_Renderer *_renderer, SDL_Event *_ev)
{
	//allows the user to set travel weights of nodes in the node map, by clicking on the corresponding grid position on the screen
	//the user can press backspace to return to the setup menu at any time

	system("cls");

	//requires the user to enter the weight they wish to set nodes to, between 0 and 25
	//inputs below 0.0 get set to 0.0, inputs over 25.0 get set to 25.0
	std::cout << "Set weight (0 to 25) (in console)" << std::endl;
	std::cin >> m_currentWeight;

	if (m_currentWeight > 25.0f)
	{
		m_currentWeight = 25.0f;
	}
	else if (m_currentWeight < 0.0f)
	{
		m_currentWeight = 0.0f;
	}

	std::cout << std::endl;

	m_NManager->DrawNMap(_renderer, true);

	//sets the cost of selected nodes to the value set by the user
	std::cout << "Add weighted nodes (in SDL Window)" << std::endl;
	std::cout << "Press backspace to return to setup menu" << std::endl;
	bool breakout = false;
	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{
			if (_ev->type == SDL_MOUSEBUTTONDOWN)
			{

				m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size))->SetCost(m_currentWeight);
				m_NManager->DrawNMap(_renderer, true);
			}

			//only returns to the setup menu when backspace is pressed
			if (_ev->type == SDL_KEYDOWN)
			{
				if (_ev->key.keysym.sym == SDLK_BACKSPACE)
				{
					breakout = true;
				}
			}
		}

	}

	system("cls");
}

void GridSetup::SetThreat(SDL_Renderer *_renderer, SDL_Event *_ev)
{
	//allows the user to set threat levels of nodes in the node map, by clicking on the corresponding grid position on the screen
	//the user can press backspace to return to the setup menu at any time

	system("cls");
	
	//requires the user to enter the weight they wish to set nodes to, between 0 and 25
	//inputs below 0.0 get set to 0.0, inputs over 25.0 get set to 25.0
	std::cout << "Set threat level (0 to 25)  (in console)" << std::endl;
	std::cin >> m_currentThreatLevel;

	if (m_currentThreatLevel > 25.0f)
	{
		m_currentThreatLevel = 25.0f;
	}
	else if (m_currentThreatLevel < 0.0f)
	{
		m_currentThreatLevel = 0.0f;
	}
	

	std::cout << std::endl;

	m_NManager->DrawNMap(_renderer, true);

	//sets the cost of selected nodes to the value set by the user
	std::cout << "Add threat nodes (in SDL Window)" << std::endl;
	std::cout << "Press backspace to return to setup menu" << std::endl;
	std::cout << "(Note: Threat Levels are only used by A* Epsilon)" << std::endl;
	bool breakout = false;
	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{
			if (_ev->type == SDL_MOUSEBUTTONDOWN)
			{
				m_NManager->m_NodeMap.at(floor(_ev->button.x / m_tempNode->m_size)).at(floor(_ev->button.y / m_tempNode->m_size))->SetThreatLevel(m_currentThreatLevel);
				m_NManager->DrawNMap(_renderer, true);
			}

			//only returns to the setup menu when backspace is pressed
			if (_ev->type == SDL_KEYDOWN)
			{
				if (_ev->key.keysym.sym == SDLK_BACKSPACE)
				{
					breakout = true;
				}
			}
		}

	}

	system("cls");
}

void GridSetup::ShowHelp()
{
	//outputs the help section to the console

}
