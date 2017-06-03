/// \file Node.h
/// \brief Node class declaration
/// \author Robert Brede

#ifndef _NODE_H_
#define _NODE_H_

#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>

//class to store data for each node in the node map
class Node
{
private:

public:

	Node()
	{
		m_mainColour = { 0,0,0,0 };
		m_borderColour = { 0,0,0,0 };
		m_posX = 0;
		m_posY = 0;

		m_hWeight = 0;
		m_gWeight = 0.0f;

		m_cost = 0.0f;

		m_isObject = false;
		m_isStart = false;
		m_isGoal = false;

		m_size = 30;

		DrawRect.h = m_size;
		DrawRect.w = m_size;

		m_parent = nullptr;
	}

	Node(SDL_Color _mCol, SDL_Color _bCol, int _X, int _Y,bool _isObj)
	{
		m_posX = _X;
		m_posY = _Y;

		m_mainColour = _mCol;
		m_borderColour = _bCol;
		
		m_hWeight = 0;
		m_gWeight = 0.0f;

		m_cost = 0.0f;

		m_isObject = _isObj;
		m_isStart = false;
		m_isGoal = false;

		m_size = 30;

		DrawRect.h = m_size;
		DrawRect.w = m_size;

		m_parent = nullptr;
	}

	~Node()
	{
		m_parent = nullptr;
	}

	//stores main node colour and border colour, to be used in node drawing
	SDL_Color m_mainColour;
	SDL_Color m_borderColour;

	//node positions in the node map
	int m_posX;
	int m_posY;

	float m_hWeight; //heuristic weight
	float m_gWeight; //real weight
	float m_fWeight; //total (distance) weight, f(n) = h(n) + g(n)

	//stores the total distance of the path
	float m_pathDistance;

	//stores the weight cost of the node
	float m_cost;
	//stores the total wight cost of the path
	float m_totalCost;

	//stores the threat level of the node
	float m_threatLevel;

	//stores the draw size of the node
	int m_size;
	//stores the draw rectangle of the node
	SDL_Rect DrawRect;

	//stores the pointer to the parent node
	Node *m_parent = nullptr;

	bool m_isObject;
	bool m_isStart;
	bool m_isGoal;

	void SetParent(Node *_parent);

	void SetHWeight(Node *_goal);
	void SetGWeight();
	void SetFWeight();

	void SetPathDistance();
	void SetCost(float _cost);
	void SetTotalPathCost(); 
	
	void SetThreatLevel(float _threat);

	//toggles whether the node is an object, start or goal node, and sets colours accordingly
	void ToggleObject();
	void ToggleStart();
	void ToggleGoal();
	
	//draws the node on the screen
	void DrawNode(SDL_Renderer *_renderer);
	//draws a line to the parent node on the screen
	void DrawParentLine(SDL_Renderer *_renderer);

	//draws the h, g and f weights of the node onto the screen (not included in final build, fonts do not scale well enough to be used)
	void DrawWeights(SDL_Renderer *_renderer, TTF_Font *_font);

	SDL_Surface *m_weightsSur;
	SDL_Texture *m_weightsTex;

	//resets all node values to their default values
	void ResetNode();
};

#endif //_NODE_H_