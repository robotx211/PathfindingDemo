///  @file PFAlgorithms.cpp
///  @brief PFAlgorithms class definition

#include <SDL2\SDL.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "PFAlgorithms.h"

//Additional File Includes
#include "Node.h"
#include "NodeManager.h"

//Module Contents
PFAlgorithms::PFAlgorithms()
{
	InitManager();
}

PFAlgorithms::~PFAlgorithms()
{}

void PFAlgorithms::SetStart(Node *_s)
{
	m_startNode = _s;
}
void PFAlgorithms::SetGoal(Node *_g)
{
	m_goalNode = _g;
}
void PFAlgorithms::SetCurrent(Node *_c)
{
	m_currentNode = _c;
}
void PFAlgorithms::SetNManager(NodeManager *_nManager)
{
	m_NManager = _nManager;
}

Node* PFAlgorithms::GetNodeExpansion(int _xShift, int _yShift)
{
	//returns a node pointer to the node which is _xShift in the x and _yShift in the y away from the current node
	return m_NManager->m_NodeMap.at(m_currentNode->m_posX + _xShift).at(m_currentNode->m_posY + _yShift);
}

void PFAlgorithms::InitOpen()
{
	/*
	while (m_open.begin() != m_open.end())
	{
		delete m_open.at(0);
	}
	*/

	m_open.clear();
}
void PFAlgorithms::InitClosed()
{
	/*
	while (m_closed.begin() != m_closed.end())
	{
		delete m_closed.at(0);
	}
	*/
	m_closed.clear();
}

void PFAlgorithms::InitManager()
{

	m_startNode = nullptr;
	m_goalNode = nullptr;
	m_currentNode = nullptr;
	m_NManager = nullptr;

	InitOpen();
	InitClosed();

}

bool PFAlgorithms::RunAlgorithm(SDL_Renderer *_renderer, SDL_Event *_ev)
{
	//allows the user to select which algorithm to use in finding a path
	std::cout << "Please choose what Algorithm to use (while SDL window is selected)" << std::endl;
	std::cout << "Breadth First(1), Depth First(2), Best First(3), Dijkstra's(4), A*(5),\n A* Epsilon(6)" << std::endl;

	bool breakout = false;

	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{

			if (_ev->type == SDL_KEYDOWN)
			{
				switch (_ev->key.keysym.sym)
				{
				case SDLK_1:
					//calls checkIfRnder to check if user wants to renderer path calculation
					//if false, does not render calculation
					if (CheckIfRender(_ev) == 0)
					{
						PFNBreadthFirst();
					}
					//if true, does render calculation
					else
					{
						PFNBreadthFirst(_renderer);
					}
					breakout = true;
					break;
				case SDLK_2:
					if (CheckIfRender(_ev) == 0)
					{
						PFNDepthFirst();
					}
					else
					{
						PFNDepthFirst(_renderer);
					}
					breakout = true;
					break;
				case SDLK_3:
					if (CheckIfRender(_ev) == 0)
					{
						PFNBestFirst();
					}
					else
					{
						PFNBestFirst(_renderer);
					}
					breakout = true;
					break;
				case SDLK_4:
					if (CheckIfRender(_ev) == 0)
					{
						PFNDijkstra();
					}
					else
					{
						PFNDijkstra(_renderer);
					}
					breakout = true;
					break;
				case SDLK_5:
					if (CheckIfRender(_ev) == 0)
					{
						PFNAStar();
					}
					else
					{
						PFNAStar(_renderer);
					}
					breakout = true;
					break;
				case SDLK_6:
					if (CheckIfRender(_ev) == 0)
					{
						PFNAStarEpsilon();
					}
					else
					{
						PFNAStarEpsilon(_renderer);
					}
					breakout = true;
					break;
				default:
					std::cout << "Invalid Entry, try again:" << std::endl;
					break;
				}

			}

		}
	}

	//checks if a path was found, retuns 1 if it did, return 0 and print error if it didn't
	if (m_goalNode->m_parent == nullptr)
	{
		std::cout << "Error: No path found" << std::endl;
		return 0;
	}
	else
	{
		SetPathCol();
		return 1;
	}

	
}

bool PFAlgorithms::CheckIfRender(SDL_Event *_ev)
{

	//takes user input on whether they want to have the calculations renderer, or just the final path (not rendering calculations is much faster)
	system("cls");
	std::cout << "Show expansions (1) or just calculate path (2) (while SDL window is selected)?" << std::endl;

	bool returnValue = 1;
	bool breakout = false;

	while (breakout == false)
	{
		while (SDL_PollEvent(_ev) != 0)
		{

			if (_ev->type == SDL_KEYDOWN)
			{
				switch (_ev->key.keysym.sym)
				{
				case SDLK_1:
					returnValue = 1;
					breakout = true;
					break;
				case SDLK_2:
					returnValue = 0;
					breakout = true;
					break;
				default:
					std::cout << "Invalid Entry, try again:" << std::endl;
					break;
				}

			}

		}
	}

	return returnValue;
}

bool PFAlgorithms::CheckIfPassed(Node *_new, std::vector<Node*> _vector)
{
	//if node is in passed vector, return true, else return false
	for (int i = 0; i < _vector.size(); i++)
	{
		if (_vector.at(i) == _new)
		{
			return true;
		}
	}
	return false;
}

int PFAlgorithms::FindNodeInVector(Node *_new, std::vector<Node*> *_vector, bool _delete)
{
	//if node is in passed vector, return it's positon, else throw exception
	//if _delete is true, deletes the node from the vector
	for (int i = 0; i < _vector->size(); i++)
	{
		if (_vector->at(i) == _new)
		{
			if (_delete == true)
			{
				_vector->erase(_vector->begin() + i);
			}
			return i;
		}
	}

	throw std::exception();
}

bool PFAlgorithms::CheckGoal(Node *_new)
{
	//if passed node is goal node, return true, else return false
	if (_new == m_goalNode)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PFAlgorithms::SetCalcCol()
{
	//sets node colours during calculation
	//sets all nodes in open to have a light blue border
	for (unsigned int i = 0; i < m_open.size(); i++)
	{
		m_open.at(i)->m_borderColour = { 97,255,249};
	}
	//sets all nodes in closed to have a dark blue border
	for (unsigned int i = 0; i < m_closed.size(); i++)
	{
		m_closed.at(i)->m_borderColour = { 37,121,191};
	}
	//sets all nodes in the a* epsilon subset to have a purple border
	for (unsigned int i = 0; i < m_subSet_A.size(); i++)
	{
		m_subSet_A.at(i)->m_borderColour = { 148,0,255 };
	}

	//sets the start node to have a green main colour
	m_startNode->m_mainColour = { 0,255,0};
	//sets the goal node to have a yellow main colour
	m_goalNode->m_mainColour = { 255,255,0};
	//if there is a current node, set it to have an orange border colour (it's main colour is also set to orange in the node draw call)
	if (m_currentNode != nullptr)
	{
		m_currentNode->m_borderColour = { 255,146,0 };
	}
	
}

void PFAlgorithms::SetPathCol()
{
	//sets all node colours after path has been found
	//sets the goal to be yellow
	m_goalNode->m_mainColour = { 255,255,0,0 };
	Node *current = m_goalNode->m_parent;

	//sets all nodes in the path to be magenta
	while (current != m_startNode)
	{
		current->m_mainColour = { 255,0,255,0 };

		current = current->m_parent;
	}

	//sets the last node (start node) to be green
	current->m_mainColour = { 0,255,0,0 };

}

float PFAlgorithms::FindGWeight(int _xShift, int _yShift) //returns the g weight of moving between nodes
{
	//returns distance between nodes, based on x and y shift
	//if it's diagonal, return 1.4142
	if (_xShift + _yShift == 0 || _xShift + _yShift == 2 || _xShift + _yShift == -2)
	{
		return 1.4142f;
	}
	//else return 1.0
	else
	{
		return 1.0f;
	}
}

void PFAlgorithms::PFNBreadthFirst(SDL_Renderer *_renderer)
{
	//breadth first path finding

	bool goalFound = false;

	//checks if node map is a valid size
	if (m_NManager->m_NodeMap.size() == 0 || m_NManager->m_NodeMap.at(0).size() == 0)
	{
		std::cout << "Invalid nMap size" << std::endl;
		return;
	}

	//sets bottom and right limits of the grid, so it cannot expand outside the grid
	int maxX = (m_NManager->m_NodeMap.size()) - 1;
	int maxY = (m_NManager->m_NodeMap.at(0).size()) - 1;

	//resets the current node
	m_currentNode = nullptr;

	//pushes start node to open, to begin algorithm
	m_open.push_back(m_startNode);

	while (m_open.size() != 0 && goalFound == false)
	{
		//sets current node to the first node in open
		SetCurrent(m_open.at(0));

		//if renderer is passed into the algorithm, draws the grid
		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

		//loops thrrough -1 to 1 in x and 7, to expand nodes
		/* Order of expansion
			123
			4 5
			678
		*/
		//this is used as the algorithm loop for all the algorithms
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				//if potential successor is out of bounds or is the current node, don't expand
				if (!((m_currentNode->m_posX + x) < 0 || (m_currentNode->m_posX + x) > maxX ||
					(m_currentNode->m_posY + y) < 0 || (m_currentNode->m_posY + y) > maxY ||
					(x == 0 && y == 0)))
				{
					//run node expansion, if the successor node is the goal node, it returns true
					if (PFNBrFNodeExpansion(x, y) == true)
					{
						goalFound = true;
					}

					if (_renderer != NULL)
					{
						SetCalcCol();
						m_NManager->DrawNMap(_renderer, true);
					}
				}
			}
		}

		m_closed.push_back(m_currentNode); //push node that has been processed to closed
		m_open.erase(m_open.begin()); //erase first element (one that has just been processed)

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

	}
}
bool PFAlgorithms::PFNBrFNodeExpansion(int _xShift, int _yShift)
{
	//if successor is not in open and not in closed
	if (CheckIfPassed(GetNodeExpansion(_xShift, _yShift), m_open) == false &&
		CheckIfPassed(GetNodeExpansion(_xShift, _yShift), m_closed) == false)
	{
		//if successor is not an object
		if (GetNodeExpansion(_xShift, _yShift)->m_isObject == false)
		{
			//set successor parent to be the current node
			GetNodeExpansion(_xShift, _yShift)->SetParent(m_currentNode);
			//is successor is the goal node, return true, without pushing goal to open
			if (CheckGoal(GetNodeExpansion(_xShift, _yShift)))
			{
				std::cout << "Goal Found" << std::endl;
				return true;
			}
			//else push succesor to open
			else
			{
				m_open.push_back(GetNodeExpansion(_xShift, _yShift));
			}
		}
	}
	//if goal is not found, return false
	return false;
}

void PFAlgorithms::PFNDepthFirst(SDL_Renderer *_renderer)
{
	//basically the same as breadth first, expansion is different
	bool goalFound = false;

	if (m_NManager->m_NodeMap.size() == 0 || m_NManager->m_NodeMap.at(0).size() == 0)
	{
		std::cout << "Invalid nMap size" << std::endl;
		return;
	}

	int maxX = (m_NManager->m_NodeMap.size()) - 1;
	int maxY = (m_NManager->m_NodeMap.at(0).size()) - 1;

	m_currentNode = nullptr;

	m_open.push_back(m_startNode);

	while (m_open.size() != 0 && goalFound == false)
	{

		SetCurrent(m_open.at(0));

		m_open.erase(m_open.begin()); //erase first element (one that has just been processed)

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if (!((m_currentNode->m_posX + x) < 0 || (m_currentNode->m_posX + x) > maxX ||
					(m_currentNode->m_posY + y) < 0 || (m_currentNode->m_posY + y) > maxY ||
					(x == 0 && y == 0)))
				{
					if (PFNDFNodeExpansion(x, y) == true)
					{
						goalFound = true;
					}

					if (_renderer != NULL)
					{
						SetCalcCol();
						m_NManager->DrawNMap(_renderer, true);
					}
				}
			}
		}

		m_closed.push_back(m_currentNode); //push node that has been processed to closed


		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

	}
}
bool PFAlgorithms::PFNDFNodeExpansion(int _xShift, int _yShift)
{
	//basically the same as breadth first, but successor is pushed to the start of open

	if (CheckIfPassed(GetNodeExpansion(_xShift, _yShift), m_open) == false &&
		CheckIfPassed(GetNodeExpansion(_xShift, _yShift), m_closed) == false)
	{
		if (GetNodeExpansion(_xShift, _yShift)->m_isObject == false)
		{
			GetNodeExpansion(_xShift, _yShift)->SetParent(m_currentNode);
			if (CheckGoal(GetNodeExpansion(_xShift, _yShift))) //allows algorithm to exit without waiting until _g is processed
			{
				std::cout << "Goal Found" << std::endl;
				return true;
			}
			else
			{
				m_open.insert(m_open.begin(), GetNodeExpansion(_xShift, _yShift));
			}
		}
	}

	return false;
}

void PFAlgorithms::PFNBestFirst(SDL_Renderer *_renderer)
{
	//basically the same as breadth first, but the open vector is sorted by each nodes h value, so the lowest h valued nodes are first
	//the current node is then selected from the front of open, so it is always the node with the lowest h value in open

	bool goalFound = false;

	//sort object used to sort open, see PFAlgorithms header
	hWeightLess sortObject;

	if (m_NManager->m_NodeMap.size() == 0 || m_NManager->m_NodeMap.at(0).size() == 0)
	{
		std::cout << "Invalid nMap size" << std::endl;
		return;
	}

	int maxX = (m_NManager->m_NodeMap.size()) - 1;
	int maxY = (m_NManager->m_NodeMap.at(0).size()) - 1;

	m_currentNode = nullptr;

	//sets the start node h value
	m_startNode->SetHWeight(m_goalNode);
	m_open.push_back(m_startNode);

	while (m_open.size() != 0 && goalFound == false)
	{


		SetCurrent(m_open.at(0));

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if (!((m_currentNode->m_posX + x) < 0 || (m_currentNode->m_posX + x) > maxX ||
					(m_currentNode->m_posY + y) < 0 || (m_currentNode->m_posY + y) > maxY ||
					(x == 0 && y == 0)))
				{
					if (PFNBstFNodeExpansion(x, y) == true)
					{
						goalFound = true;
						break;
					}

					if (_renderer != NULL)
					{
						SetCalcCol();
						m_NManager->DrawNMap(_renderer, true);
					}
				}
			}

			if (goalFound == true)
			{
				break;
			}
		}

		m_closed.push_back(m_currentNode); //push node that has been processed to closed
		m_open.erase(m_open.begin()); //erase first element (one that has just been processed)

		//sorts the open list by the lowest h value, using std::sort
		std::sort(m_open.begin(), m_open.end(), sortObject);

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

	}
}
bool PFAlgorithms::PFNBstFNodeExpansion(int _xShift, int _yShift)
{
	//same as breadth first, but sets the h weight of the successor node

	if (CheckIfPassed(GetNodeExpansion(_xShift, _yShift), m_open) == false &&
		CheckIfPassed(GetNodeExpansion(_xShift, _yShift), m_closed) == false)
	{
		if (GetNodeExpansion(_xShift, _yShift)->m_isObject == false)
		{
			GetNodeExpansion(_xShift, _yShift)->SetParent(m_currentNode);
			if (CheckGoal(GetNodeExpansion(_xShift, _yShift))) //allows algorithm to exit without waiting until _g is processed
			{
				std::cout << "Goal Found" << std::endl;
				return true;
			}
			else
			{
				GetNodeExpansion(_xShift, _yShift)->SetHWeight(m_goalNode);
				m_open.push_back(GetNodeExpansion(_xShift, _yShift));
			}
		}
	}

	return false;
}

void PFAlgorithms::PFNDijkstra(SDL_Renderer *_renderer)
{
	//basically the same as best first, but the open vector is sorted by each nodes g value, so the lowest g valued nodes are first
	//the current node is then selected from the front of open, so it is always the node with the lowest g value in open
	bool goalFound = false;

	//sort object used to sort the open, see PFAlgorithms header
	gWeightLess sortObject;

	if (m_NManager->m_NodeMap.size() == 0 || m_NManager->m_NodeMap.at(0).size() == 0)
	{
		std::cout << "Invalid nMap size" << std::endl;
		return;
	}

	int maxX = (m_NManager->m_NodeMap.size()) - 1;
	int maxY = (m_NManager->m_NodeMap.at(0).size()) - 1;

	m_currentNode = nullptr;

	//set g weight of start node (needed is the start node has a cost value)
	m_startNode->SetGWeight();
	m_open.push_back(m_startNode);

	while (m_open.size() != 0 && goalFound == false)
	{

		SetCurrent(m_open.at(0));

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if (!((m_currentNode->m_posX + x) < 0 || (m_currentNode->m_posX + x) > maxX ||
					(m_currentNode->m_posY + y) < 0 || (m_currentNode->m_posY + y) > maxY ||
					(x == 0 && y == 0)))
				{
					if (PFNDijNodeExpansion(x, y) == true)
					{
						goalFound = true;
						break;
					}

					if (_renderer != NULL)
					{
						SetCalcCol();
						m_NManager->DrawNMap(_renderer, true);
					}
				}
			}

			if (goalFound == true)
			{
				break;
			}
		}

		m_closed.push_back(m_currentNode); //push node that has been processed to closed
		m_open.erase(m_open.begin()); //erase first element (one that has just been processed)

		//sorts the open list by the lowest g value, using std::sort
		std::sort(m_open.begin(), m_open.end(), sortObject);

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

	}
}
bool PFAlgorithms::PFNDijNodeExpansion(int _xShift, int _yShift)
{
	//Dijkstra expansion has improvement, checks if the successor is in open or closed
	//if in neither, and not the goal node, set it's parent to be the current node, calculate the g weight of the node and push it to open
	//if in open, check if the g weight it could have would be lower than the g weight it currently has, set it's parent to be the current node and calculate the g weight of the node
	//if in closed, check if the g weight it could have would be lower than the g weight it currently has, set it's parent to be the current node ,calculate the g weight of the node, push it to open and remove it from closed

	//used to simplify the code
	m_successor = GetNodeExpansion(_xShift, _yShift);

	if (m_successor->m_isObject == false) //if is an object
	{
		if (CheckIfPassed(m_successor, m_open) == false &&
			CheckIfPassed(m_successor, m_closed) == false) //if not in open or closed
		{

			m_successor->SetParent(m_currentNode);
			if (CheckGoal(m_successor)) //allows algorithm to exit without waiting until goal is processed
			{
				std::cout << "Goal Found" << std::endl;
				return true;
			}
			else
			{
				m_successor->SetGWeight();
				m_open.push_back(m_successor);
			}

		}
		else if (CheckIfPassed(m_successor, m_open) == true &&
			CheckIfPassed(m_successor, m_closed) == false) //if in open, but not closed
		{
			//total potential g weight = distance between current and successor, cost of successor and g weight of current
			if (FindGWeight(_xShift, _yShift) + m_successor->m_cost + m_currentNode->m_gWeight < m_successor->m_gWeight)
			{
				m_successor->SetParent(m_currentNode);
				m_successor->SetGWeight();
			}

		}
		else if (CheckIfPassed(m_successor, m_open) == false &&
			CheckIfPassed(m_successor, m_closed) == true) //if in closed, but not open
		{

			if (FindGWeight(_xShift, _yShift) + m_successor->m_cost + m_currentNode->m_gWeight < m_successor->m_gWeight)
			{
				m_successor->SetParent(m_currentNode);
				m_successor->SetGWeight();
				m_open.push_back(m_successor);
				//m_closed.erase(m_closed.begin() + FindNodeInVector(m_successor, m_closed));
				FindNodeInVector(m_successor, &m_closed, true);
			}

		}
	}

	return false;
}

void PFAlgorithms::PFNAStar(SDL_Renderer *_renderer)
{
	//basically the same as best first, but the open vector is sorted by each nodes f value, so the lowest f valued nodes are first
	//the current node is then selected from the front of open, so it is always the node with the lowest gf value in open

	bool goalFound = false;
	//sort object used to sort the open, see PFAlgorithms header
	fWeightLess sortObject;

	if (m_NManager->m_NodeMap.size() == 0 || m_NManager->m_NodeMap.at(0).size() == 0)
	{
		std::cout << "Invalid nMap size" << std::endl;
		return;
	}

	int maxX = (m_NManager->m_NodeMap.size()) - 1;
	int maxY = (m_NManager->m_NodeMap.at(0).size()) - 1;

	m_currentNode = nullptr;

	m_startNode->SetHWeight(m_goalNode);
	m_startNode->SetGWeight();
	m_startNode->SetFWeight();
	m_open.push_back(m_startNode);

	while (m_open.size() != 0 && goalFound == false)
	{


		SetCurrent(m_open.at(0));

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if (!((m_currentNode->m_posX + x) < 0 || (m_currentNode->m_posX + x) > maxX ||
					(m_currentNode->m_posY + y) < 0 || (m_currentNode->m_posY + y) > maxY ||
					(x == 0 && y == 0)))
				{
					if (PFNASNodeExpansion(x, y) == true)
					{
						goalFound = true;
						break;
					}

					if (_renderer != NULL)
					{
						SetCalcCol();
						m_NManager->DrawNMap(_renderer, true);
					}
				}
			}

			if (goalFound == true)
			{
				break;
			}
		}

		m_closed.push_back(m_currentNode); //push node that has been processed to closed
		m_open.erase(m_open.begin()); //erase first element (one that has just been processed)
		//sorts the open list by the lowest f value, using std::sort
		std::sort(m_open.begin(), m_open.end(), sortObject);

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

	}
}
bool PFAlgorithms::PFNASNodeExpansion(int _xShift, int _yShift)
{
	//A* expansion has improvement, checks if the successor is in open or closed
	//if in neither, and not the goal node, set it's parent to be the current node, calculate the h, g and f weight of the node and push it to open
	//if in open, check if the g weight it could have would be lower than the g weight it currently has, set it's parent to be the current node and calculate the h, g and f weight of the node
	//if in closed, check if the g weight it could have would be lower than the g weight it currently has, set it's parent to be the current node ,calculate the h, g and f weight of the node, push it to open and remove it from closed


	m_successor = GetNodeExpansion(_xShift, _yShift);

	if (m_successor->m_isObject == false) //if is an object
	{
		if (CheckIfPassed(m_successor, m_open) == false &&
			CheckIfPassed(m_successor, m_closed) == false) //if not in open or closed
		{

			m_successor->SetParent(m_currentNode);
			if (CheckGoal(m_successor)) //allows algorithm to exit without waiting until goal is processed
			{
				std::cout << "Goal Found" << std::endl;
				return true;
			}
			else
			{
				m_successor->SetHWeight(m_goalNode);
				m_successor->SetGWeight();
				m_successor->SetFWeight();
				m_open.push_back(m_successor);
			}

		}
		else if (CheckIfPassed(m_successor, m_open) == true &&
			CheckIfPassed(m_successor, m_closed) == false) //if in open, but not closed
		{

			if (FindGWeight(_xShift, _yShift) + m_successor->m_cost + m_currentNode->m_gWeight < m_successor->m_gWeight)
			{
				m_successor->SetParent(m_currentNode);
				m_successor->SetGWeight();
				m_successor->SetFWeight();
			}

		}
		else if (CheckIfPassed(m_successor, m_open) == false &&
			CheckIfPassed(m_successor, m_closed) == true) //if in closed, but not open
		{

			if (FindGWeight(_xShift, _yShift) + m_successor->m_cost + m_currentNode->m_gWeight < m_successor->m_gWeight)
			{
				m_successor->SetParent(m_currentNode);
				m_successor->SetGWeight();
				m_successor->SetFWeight();
				m_open.push_back(m_successor);
				//m_closed.erase(m_closed.begin() + FindNodeInVector(m_successor, &m_closed));
				FindNodeInVector(m_successor, &m_closed, true);
			}

		}
		//used if the successor is in both open and closed, used for debugging
		//prints an error to the console and stops the program
		else if (CheckIfPassed(m_successor, m_open) == true &&
			CheckIfPassed(m_successor, m_closed) == true)
		{
			system("cls");
			std::cout << "ERROR in both open and closed!!!" << std::endl;
			while (true)
			{

			}
		}
	}

	return false;
}

void PFAlgorithms::PFNAStarEpsilon(SDL_Renderer *_renderer)
{
	//the same as A*, but takes all nodes from a sorted open list which have an f value up to 10% more than the lowest f value and put them into a subset
	//the subset is then sorted by the lowest threat value. This means that the algorithm will preference lower threat values, over the lowest f value

	//this algorithm has not been fully tested, but seems to work

	bool goalFound = false;
	fWeightLess sortObject1;
	threatLess sortObject2;

	float lowF;

	float epsilon = 0.1f; //as a percentage eg 0.0 = 0%, 0.1 = 10%, 2.5 = 250%
						  //epsilon = 0.1f;

	if (m_NManager->m_NodeMap.size() == 0 || m_NManager->m_NodeMap.at(0).size() == 0)
	{
		std::cout << "Invalid nMap size" << std::endl;
		return;
	}

	int maxX = (m_NManager->m_NodeMap.size()) - 1;
	int maxY = (m_NManager->m_NodeMap.at(0).size()) - 1;

	m_currentNode = nullptr;

	m_startNode->SetHWeight(m_goalNode);
	m_startNode->SetGWeight();
	m_startNode->SetFWeight();
	m_open.push_back(m_startNode);

	while (m_open.size() != 0 && goalFound == false)
	{

		lowF = m_open.at(0)->m_fWeight;

		for (int i = 0; i < m_open.size(); i++)//push all nodes with lowest f weight to subset
		{
			if (m_open.at(i)->m_fWeight <= lowF * (1.0f + epsilon)) //if node is less than or equal to the lowest weight * (1 + epsilon)
			{
				m_subSet_A.push_back(m_open.at(i)); //push to subset

				if (_renderer != NULL)
				{
					SetCalcCol();
					m_NManager->DrawNMap(_renderer, true);
				}
			}
		}

		std::sort(m_subSet_A.begin(), m_subSet_A.end(), sortObject2);

		SetCurrent(m_subSet_A.at(0));

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				if (!((m_currentNode->m_posX + x) < 0 || (m_currentNode->m_posX + x) > maxX ||
					(m_currentNode->m_posY + y) < 0 || (m_currentNode->m_posY + y) > maxY ||
					(x == 0 && y == 0)))
				{
					if (PFNASENodeExpansion(x, y) == true)
					{
						goalFound = true;
						break;
					}

					if (_renderer != NULL)
					{
						SetCalcCol();
						m_NManager->DrawNMap(_renderer, true);
					}
				}
			}

			if (goalFound == true)
			{
				break;
			}
		}

		m_closed.push_back(m_currentNode); //push node that has been processed to closed
		//m_open.erase(m_open.begin() + FindNodeInVector(m_currentNode, m_open)); //erase first element (one that has just been processed)
		FindNodeInVector(m_currentNode, &m_open, true);

		//sorts the open vector by the lowest f value
		std::sort(m_open.begin(), m_open.end(), sortObject1);

		//clears the a* epsilon subset, after each expansion
		m_subSet_A.clear();

		if (_renderer != NULL)
		{
			SetCalcCol();
			m_NManager->DrawNMap(_renderer, true);
		}

	}
}
bool PFAlgorithms::PFNASENodeExpansion(int _xShift, int _yShift)
{
	//same expansion as a*, as a* epsilon only affects which nodes are expanded, not how they are expanded

	m_successor = GetNodeExpansion(_xShift, _yShift);

	if (m_successor->m_isObject == false) //if is an object
	{
		if (CheckIfPassed(m_successor, m_open) == false &&
			CheckIfPassed(m_successor, m_closed) == false) //if not in open or closed
		{

			m_successor->SetParent(m_currentNode);
			if (CheckGoal(m_successor)) //allows algorithm to exit without waiting until goal is processed
			{
				std::cout << "Goal Found" << std::endl;
				return true;
			}
			else
			{
				m_successor->SetHWeight(m_goalNode);
				m_successor->SetGWeight();
				m_successor->SetFWeight();
				m_open.push_back(m_successor);
			}

		}
		else if (CheckIfPassed(m_successor, m_open) == true &&
			CheckIfPassed(m_successor, m_closed) == false) //if in open, but not closed
		{

			if (FindGWeight(_xShift, _yShift) + m_successor->m_cost + m_currentNode->m_gWeight < m_successor->m_gWeight)
			{
				m_successor->SetParent(m_currentNode);
				m_successor->SetGWeight();
				m_successor->SetFWeight();
			}

		}
		else if (CheckIfPassed(m_successor, m_open) == false &&
			CheckIfPassed(m_successor, m_closed) == true) //if in closed, but not open
		{

			if (FindGWeight(_xShift, _yShift) + m_successor->m_cost + m_currentNode->m_gWeight < m_successor->m_gWeight)
			{
				m_successor->SetParent(m_currentNode);
				m_successor->SetGWeight();
				m_successor->SetFWeight();
				m_open.push_back(m_successor);
				//m_closed.erase(m_closed.begin() + FindNodeInVector(m_successor, m_closed));
				FindNodeInVector(m_successor, &m_closed, true);
			}

		}
	}

	return false;
}



