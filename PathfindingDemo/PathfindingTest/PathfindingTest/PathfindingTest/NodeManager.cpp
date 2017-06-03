///  @file NodeManager.cpp
///  @brief NodeManager class definition

#include <vector>
#include <SDL2\SDL.h>
#include <SDL2\SDL_ttf.h>
#include <iostream>

#include "NodeManager.h"

//Additional File Includes
#include "Node.h"

//Module Contents
NodeManager::NodeManager()
{
	//loads the font for draw weights (not in final build)
	TTF_Init();
	m_font = TTF_OpenFont("pixelFont.ttf", 8);
	
	if (m_font == NULL)
	{
		std::cout << "Font not loaded" << std::endl;
	}
	InitNMap();
}

NodeManager::~NodeManager()
{
	//deletes all nodes in nodemap, clears the nodemap and deletes the font
	for (int i = 0; m_NodeMap.begin() == m_NodeMap.end(); i++)
	{
		while (m_NodeMap.at(i).begin() != m_NodeMap.at(i).end())
		{
			delete m_NodeMap.at(i).at(0);
		}
	}

	m_NodeMap.clear();

	TTF_CloseFont (m_font);
	m_font = NULL;
}

void NodeManager::InitNMap()
{
	//fills the node map with a 30/30 grid of nodes
	for (int i = 0; i < 30; i++)
	{
		std::vector<Node*> tempVec;
		for (int j = 0; j < 30; j++)
		{
			tempVec.push_back(new Node({ 255,255,255,255 }, { 0,0,0,255 }, i, j, false));
		}
		m_NodeMap.push_back(tempVec);
	}
}

void NodeManager::ClearNMap()
{
	//resets values (but not positions) of all nodes in node map
	for (int i = 0; i < m_NodeMap.size(); i++)
	{
		for (int j = 0; j < m_NodeMap.size(); j++)
		{
			m_NodeMap.at(i).at(j)->ResetNode();
		}
	}
}

void NodeManager::DrawNMap(SDL_Renderer *_renderer, bool _render)
{
	//draws the nodes and parents lines of all the nodes in nodemap
	for (int i = 0; i < m_NodeMap.size(); i++)
	{
		for (int j = 0; j < m_NodeMap.at(i).size(); j++)
		{
			m_NodeMap.at(i).at(j)->DrawNode(_renderer);
		}
	}
	for (int i = 0; i < m_NodeMap.size(); i++)
	{
		for (int j = 0; j < m_NodeMap.at(i).size(); j++)
		{
			m_NodeMap.at(i).at(j)->DrawParentLine(_renderer);
		}
	}

	//DrawWeights(_renderer);

	//obsolete, render choice is decided in algorithm
	if (_render == true)
	{
		SDL_RenderPresent(_renderer);
	}
	
}

void NodeManager::RemovePathing()
{
	//resets the parent nodes for each node in node map
	for (int i = 0; i < m_NodeMap.size(); i++)
	{
		for (int j = 0; j < m_NodeMap.at(i).size(); j++)
		{
			m_NodeMap.at(i).at(j)->m_parent = nullptr;
		}
	}
}

void NodeManager::DrawWeights(SDL_Renderer *_renderer)
{
	//draws the weights of all node in the node map (not in final build)
	for (int i = 0; i < m_NodeMap.size(); i++)
	{
		for (int j = 0; j < m_NodeMap.at(i).size(); j++)
		{
			m_NodeMap.at(i).at(j)->DrawWeights(_renderer, m_font);
		}
	}
}