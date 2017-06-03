/// \file GridSetup.h
/// \brief GridSetup class declaration
/// \author Robert Brede

#ifndef _GRIDSETUP_H_
#define _GRIDSETUP_H_

#include <vector>
#include <SDL2\SDL.h>
#include <iostream>

#include "Node.h"
#include "NodeManager.h"

class GridSetup
{

private:
protected:
public:

	GridSetup(NodeManager *_NManager);
	~GridSetup();

	//set to store a pointer to the node manager, so that the setup can modify node values and draw the grid
	NodeManager *m_NManager;

	//used to store references to the user set start and goal nodes, so they can be toggled
	Node *m_startNode;
	Node *m_goalNode;

	//used to find the size of the node, so mouse input can be properly calculated, and node size still remains defined in the node objects
	Node *m_tempNode;

	//used to store user set weights and threat levels, so they can be used on multiple nodes
	float m_currentWeight;
	float m_currentThreatLevel;

	//used to check if the user has completed their setup, else the setup will repeat
	bool m_setupComplete;

	//runs a setup of the node map, setting start, goal, obstacles, weighted nodes and threat levels
	void Setup(SDL_Renderer *_renderer, SDL_Event *_ev);

	void ShowHelp();

	//used to set nodes of the corresponding name, when the user selects them in Setup()
	void SetStart(SDL_Renderer *_renderer, SDL_Event *_ev);
	void SetGoal(SDL_Renderer *_renderer, SDL_Event *_ev);
	void SetObjects(SDL_Renderer *_renderer, SDL_Event *_ev);
	void SetWeights(SDL_Renderer *_renderer, SDL_Event *_ev);

	void SetThreat(SDL_Renderer *_renderer, SDL_Event *_ev);

};

#endif //_GRIDSETUP_H_
