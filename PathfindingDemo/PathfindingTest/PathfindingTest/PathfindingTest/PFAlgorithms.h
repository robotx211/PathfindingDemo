/// \file PFAlgorithm.h
/// \brief Sorting structs and PFAlgorithm class declaration
/// \author Robert Brede

#ifndef _PFALGORITHMS_H_
#define _PFALGORITHMS_H_

#include <vector>
#include <SDL2\SDL.h>

#include "Node.h"
#include "NodeManager.h"

//structs, used in sorting vectors, using std::sort
struct gWeightLess
{
	//sorts nodes by lowest g weight
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_gWeight < node2->m_gWeight);
	}
};

struct costLess
{
	//sorts nodes by lowest cost
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_cost < node2->m_cost);
	}
};

struct hWeightLess
{
	//sorts nodes by lowest h weight
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_hWeight < node2->m_hWeight);
	}
};

struct hWeightMore
{
	//sorts nodes by highest h weight (was used in worst first, removed algorithm)
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_hWeight > node2->m_hWeight);
	}
};

struct fWeightLess
{
	//sorts nodes by lowest f weight
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_fWeight < node2->m_fWeight);
	}
};

struct threatLess
{
	//sorts nodes by lowest threat level
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_threatLevel < node2->m_threatLevel);
	}
};

//debug struct
struct closedSort
{
	//sorts nodes by lowest threat level
	inline bool operator () (const Node *node1, const Node *node2)
	{
		return (node1->m_posX < node2->m_posX);
	}
};

//class used to hold pathing algorithms, open and closed vectors, pointers to the start, goal and current nodes and a pointer to the node manager
class PFAlgorithms
{
private:
protected:
public:

	PFAlgorithms();
	~PFAlgorithms();

	//node pointers, to store the start, goal and current nodes
	Node *m_startNode;
	Node *m_goalNode; 
	Node *m_currentNode;

	//NodeManager pointer, to store the node manager, so the node map can be edited and drawn
	NodeManager *m_NManager;

	//node pointer to store the potential successor nodes of the current node
	Node *m_successor;

	//vectors to store the open and closed nodes
	std::vector<Node*> m_open;
	std::vector<Node*> m_closed;

	//vectors used to store subsets of nodes, eg. for A* epsilon
	std::vector<Node*> m_subSet_A;
	std::vector<Node*> m_subSet_B;
	std::vector<Node*> m_subSet_C;

	void SetStart(Node *_s);
	void SetGoal(Node *_g);
	void SetCurrent(Node *_c);
	void SetNManager(NodeManager *_nManager);

	//returns a node pointer to the node which is _xShift in the x and _yShift in the y away from the current node
	Node* GetNodeExpansion(int _xShift, int _yShift);

	//clears open and closed vectors
	void InitOpen();
	void InitClosed();

	//resets start, goal and current node pointer, node manager pointer and inits open and closed
	void InitManager();

	//runs a function to allow the user to select which algorithm to use, and then use that algorithm to find a path. returns 1 if a path is found, and 0 if no path is found
	bool RunAlgorithm(SDL_Renderer *_renderer, SDL_Event *_ev);
	//runs a function to check if user wants to render the expansion of the algorithm, or just see the final path
	bool CheckIfRender(SDL_Event *_ev);

	//checks if a node is in a vector (eg. open or closed), returns true or false
	bool CheckIfPassed(Node *_new, std::vector<Node*> _vector);
	//find a node's position in a vector (eg. open or closed), returns the position and deletes the node, if _delete is set to true
	int FindNodeInVector(Node *_new, std::vector<Node*> *_vector, bool _delete);
	//checks if the node passed is the goal node
	bool CheckGoal(Node *_new);

	//sets the node colours of the node map during calculation
	void SetCalcCol();
	//sets the node colours of the path after it has been found
	void SetPathCol();

	//returns the distance bettwen 2 nodes, either 1.0 or 1.4142
	float FindGWeight(int _xShift,int _yShift);

	//	PATHING ALGORITHMS
	//	void ExpansionOrder()
	//	bool Expansion(), returns true if node expanded to is the goal node

	//(SDL_Renderer *_renderer = NULL) means that if no renderer is passed into the algorithm, it does not render the calculation
	void PFNBreadthFirst(SDL_Renderer *_renderer = NULL);
	bool PFNBrFNodeExpansion(int _xShift, int _yShift);

	void PFNDepthFirst(SDL_Renderer *_renderer = NULL);
	bool PFNDFNodeExpansion(int _xShift, int _yShift);

	void PFNBestFirst(SDL_Renderer *_renderer = NULL);
	bool PFNBstFNodeExpansion(int _xShift, int _yShift);

	void PFNDijkstra(SDL_Renderer *_renderer = NULL);
	bool PFNDijNodeExpansion(int _xShift, int _yShift);

	void PFNAStar(SDL_Renderer *_renderer = NULL);
	bool PFNASNodeExpansion(int _xShift, int _yShift);

	void PFNAStarEpsilon(SDL_Renderer *_renderer = NULL);
	bool PFNASENodeExpansion(int _xShift, int _yShift);
};

#endif //_PFALGORITHMS_H_
