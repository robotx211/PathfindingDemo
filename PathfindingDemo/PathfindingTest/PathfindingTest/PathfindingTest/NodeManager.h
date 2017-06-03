/// \file NodeManager.h
/// \brief NodeManager class declaration
/// \author Robert Brede

#ifndef _NODEMANAGER_H_
#define _NODEMANAGER_H_

#include <SDL2\SDL_ttf.h>

#include <vector>

class Node;

//class to store node map and function pertaining to the node map
class NodeManager
{
private:
protected:
public:

	NodeManager();
	~NodeManager();

	//stores the font used to draw weights
	//not used in final build
	TTF_Font *m_font;

	//2D vector to store the node map
	std::vector<std::vector<Node*>> m_NodeMap;

	//adds all the nodes into the nodemap, called once
	void InitNMap();
	//resets all values of nodes in the node map
	void ClearNMap();

	//draws all nodes in the node map on the screen
	void DrawNMap(SDL_Renderer *_renderer, bool _render);

	//removes all the pathing from the node map (not used, was designed for multiple agents)
	void RemovePathing();

	//draws weights of all nodes (not used in final build)
	void DrawWeights(SDL_Renderer *_renderer);

};

#endif //_NODEMANAGER_H_
