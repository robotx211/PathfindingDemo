///  @file Node.cpp
///  @brief Node class definition

#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "Node.h"

//Module Contents
void Node::SetParent(Node *_parent)
{
	//sets the parent node to the passed node pointer	
	m_parent = _parent;

	//Code under this was used for debugging loops of parent nodes

	/*
	Node *current = this;
	std::vector<Node*> process;
	m_parent = _parent;

	
	while (current->m_parent != nullptr)
	{
		for (int i = 0; i < process.size(); i++)
		{
			if (process.at(i) == current)
			{
				throw std::exception();
			}
		}
		process.push_back(current);
		current = current->m_parent;
	*/
}


void Node::SetHWeight(Node *_goal)
{

	//sets the h weight of the node to be the manhattan distance to the goal node
	//eg: this node = (2,3), goal = (6,6), h weight = 4 + 3 = 7
	m_hWeight = (std::abs(this->m_posX - _goal->m_posX) + std::abs(this->m_posY - _goal->m_posY));

}

void Node::SetPathDistance()
{
	//sets the path distance to be equal to the distance from start to the parent node + the distance from the parent to this node

	if (m_parent != nullptr)
	{
		//if manhattan distance from parent to this node = 1 (up, down, left, or right expansion), +1.0f to path distance
		if (std::abs(m_posX - m_parent->m_posX) + std::abs(m_posY - m_parent->m_posY) == 1)
		{
			m_pathDistance = 1.0f + m_parent->m_pathDistance;
		}
		//if manhattan distance from parent to this node = 2 (diagonal expansion), +1.4142f to path distance
		else if (std::abs(m_posX - m_parent->m_posX) + std::abs(m_posY - m_parent->m_posY) == 2)
		{
			m_pathDistance = 1.4142f + m_parent->m_pathDistance;
		}

	}
}

void Node::SetFWeight()
{
	m_fWeight = m_hWeight + m_gWeight;
}

void Node::SetGWeight()
{
	//sets g weight to equal total path distance and total path weight cost

	SetPathDistance();
	SetTotalPathCost();

	m_gWeight = m_pathDistance + m_totalCost;
}

void Node::SetCost(float _cost)
{
	//sets cost of node to be what the user input, then sets the main colour to be a greyscale based on that weight

	m_cost = _cost;
	m_mainColour = { (Uint8)floor(255.0f - (_cost * 8.0f)), (Uint8)floor(255.0f - (_cost * 8.0f)), (Uint8)floor(255.0f - (_cost * 8.0f)) };
}

void Node::SetTotalPathCost()
{
	//if this node has a parent, sets the total cost to equal the parents totl cost + it's total cost

	if (m_parent != nullptr)
	{
		m_totalCost = m_parent->m_totalCost + m_cost;
	}
	//else it's total cost equals it's cost
	else
	{
		m_totalCost = m_cost;
	}
	
}

void Node::SetThreatLevel(float _threat)
{
	//sets threat level of node to be what the user input, then sets the border colour to be a redscale based on that value. 
	//If threat level = 0.0, border colour is set to black, to avoid confusion when drawing

	m_threatLevel = _threat;
	if (_threat == 0.0f)
	{
		m_borderColour = { 0,0,0 };
	}
	else
	{
		m_borderColour = { 255, (Uint8)floor(255.0f - (_threat * 10.0f)), (Uint8)floor(255.0f - (_threat * 10.0f)), 0};	
	}
}


void Node::ToggleObject()
{
	//toggles whether a node is an object, and sets it's colour to be white if false, and black if true

	if (m_isObject == true)
	{
		m_isObject = false;
		m_mainColour = { 255,255,255,0 };
	}
	else if (m_isObject == false)
	{
		m_isObject = true;
		m_mainColour = { 0,0,0,0 };
	}
}

void Node::ToggleStart()
{
	//toggles whether a node is the start node, and sets it's colour to be white if false, and green if true
	if (m_isStart == true)
	{
		m_isStart = false;
		m_mainColour = { 255,255,255,0 };
	}

	else if (m_isStart == false)
	{
		m_isStart = true;
		m_mainColour = { 0,255,0,0 };
	}
}

void Node::ToggleGoal()
{
	//toggles whether a node is the goal node, and sets it's colour to be white if false, and yellow if true
	if (m_isGoal == true)
	{
		m_isGoal = false;
		m_mainColour = { 255,255,255,0 };
	}

	else if (m_isGoal == false)
	{
		m_isGoal = true;
		m_mainColour = { 255,255,0,0 };
	}
}

void Node::DrawNode(SDL_Renderer *_renderer)
{
	//draws the node, with it's colours, size and position
	//nodes are drawn as a filled square, set to be the main colour, and an unfilled square, set to be the border colour
	DrawRect.h = m_size;
	DrawRect.w = m_size;
	DrawRect.x = m_size * m_posX;
	DrawRect.y = m_size * m_posY;

	//if the main colour is orange (so it is the current node in a pathing algorithm, set it's filled square colour to be orange (it's border colour)
	if (m_borderColour.r == 255 && m_borderColour.g == 146 && m_borderColour.b == 0)
	{
		SDL_SetRenderDrawColor(_renderer, m_borderColour.r, m_borderColour.g, m_borderColour.b, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(_renderer, m_mainColour.r, m_mainColour.g, m_mainColour.b, 255);
	}

	SDL_RenderFillRect(_renderer, &DrawRect);

	
	SDL_SetRenderDrawColor(_renderer, m_borderColour.r, m_borderColour.g, m_borderColour.b, 255);
	SDL_RenderDrawRect(_renderer, &DrawRect);
	

	
}

void Node::DrawParentLine(SDL_Renderer *_renderer)
{
	//draws a line between the centre of the this node to the centre of the parent node, on screen
	if (m_parent != nullptr)
	{
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderDrawLine(_renderer, m_posX * m_size + m_size / 2, m_posY * m_size + m_size / 2, m_parent->m_posX * m_size + m_size / 2, m_parent->m_posY * m_size + m_size / 2);
	}
}

void Node::DrawWeights(SDL_Renderer *_renderer, TTF_Font *_font)
{
	//used to write the h, g and f weights of nodes onto the grid
	//text does not scale well at low sizes, so it not included in the final build
	//only really works at a vrey high node size
	//also decreases performacne drastically

	//prints the f weight in the top left corner, h weight in bottom left corner and g weight in bottom right corner

	std::stringstream fString, gString, hString;


	DrawRect.h = 8;
	DrawRect.w = 24;

	if (m_fWeight != 0)
	{
		DrawRect.h = 16;
		if (m_fWeight <= 99)
		{
			fString << "00" << m_fWeight;
		}
		else if (m_fWeight <= 999)
		{
			fString << "0" << m_fWeight;
		}
		else
		{
			fString << m_fWeight;
		}

		m_weightsSur = TTF_RenderText_Solid(_font, fString.str().c_str(), { 0,0,0 });
		m_weightsTex = SDL_CreateTextureFromSurface(_renderer, m_weightsSur);

		DrawRect.x = m_size * m_posX + 2;
		DrawRect.y = m_size * m_posY + 2;

		SDL_RenderCopy(_renderer, m_weightsTex, NULL, &DrawRect);

		if (m_gWeight <= 99)
		{
			gString << "00" << m_gWeight;
		}
		else if (m_gWeight <= 999)
		{
			gString << "0" << m_gWeight;
		}
		else
		{
			gString << m_gWeight;
		}

		m_weightsSur = TTF_RenderText_Solid(_font, gString.str().c_str(), { 0,0,0 });
		m_weightsTex = SDL_CreateTextureFromSurface(_renderer, m_weightsSur);

		DrawRect.y = m_size * m_posY + m_size - 2 - 12;

		SDL_RenderCopy(_renderer, m_weightsTex, NULL, &DrawRect);

		if (m_hWeight <= 99)
		{
			hString << "00" << m_hWeight;
		}
		else if (m_hWeight <= 999)
		{
			hString << "0" << m_hWeight;
		}
		else
		{
			hString << m_hWeight;
		}
		m_weightsSur = TTF_RenderText_Solid(_font, hString.str().c_str(), { 0,0,0 });
		m_weightsTex = SDL_CreateTextureFromSurface(_renderer, m_weightsSur);

		DrawRect.x = m_size * m_posX + m_size - 2 - 24;

		SDL_RenderCopy(_renderer, m_weightsTex, NULL, &DrawRect);

		SDL_FreeSurface(m_weightsSur);
	}

	
}

void Node::ResetNode()
{
	//resets all node values

	m_mainColour = { 255,255,255,0 };

	m_borderColour = { 0,0,0,0 };

	m_hWeight = 0.0f;
	m_gWeight = 0.0f;
	m_fWeight = 0.0f;

	if (m_isStart == true)
	{
		ToggleStart();
	}

	if (m_isGoal == true)
	{
		ToggleGoal();
	}

	if (m_isObject == true)
	{
		ToggleObject();
	}

	m_pathDistance = 0.0f;

	SetCost(0.0f);
	m_totalCost = 0.0f;

	SetThreatLevel(0.0f);

	m_parent = nullptr;
}



