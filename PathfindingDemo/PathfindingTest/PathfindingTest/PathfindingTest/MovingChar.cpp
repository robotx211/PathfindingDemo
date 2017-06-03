///  @file MovingChar.cpp
///  @brief MovingChar class definition

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "MovingChar.h"

//Additional File Includes
#include "Node.h"
#include "NodeManager.h"

//Module Contents
MovingChar::MovingChar(SDL_Renderer *_renderer)
{
	//loads the texture, else prints error message
	m_Texture = IMG_LoadTexture(_renderer, "TestChar.jpg");

	if (m_Texture == NULL || m_Texture == nullptr)
	{
		std::cout << "Texture not loaded" << std::endl;
	}
	//sets texture size (height and width, as texture is square)
	m_size = 30;
}

void MovingChar::m_genPath(Node *_start, Node *_goal)
{
	//creates waypoints from each node from the goal to the start, and places them at the FRONT of the path
	//so the path is read start to goal

	std::vector<Waypoint> path;

	Node *current = _goal;

	while (current != _start)
	{
		path.insert(path.begin(), Waypoint(current));
		current = current->m_parent;
	}

	path.insert(path.begin(), Waypoint(current));

	//resets the path within the agent, the path position, teh current waypoint and the x and y position
	m_path = path;
	m_pathPosition = 0;

	m_currentWaypoint = m_path.at(0);

	m_PosX = m_path.at(0).m_PosX;
	m_PosY = m_path.at(0).m_PosY;
}

void MovingChar::MoveToWaypoint()
{
	//moves the agent 1 pixel towards the waypoint in both x and y, if required

	if (m_PosX < m_currentWaypoint.m_PosX)
	{
		m_PosX+=1;
	}
	else if (m_PosX > m_currentWaypoint.m_PosX)
	{
		m_PosX-=1;
	}
	else if (m_PosX == m_currentWaypoint.m_PosX)
	{
		
	}

	if (m_PosY < m_currentWaypoint.m_PosY)
	{
		m_PosY+=1;
	}
	else if (m_PosY > m_currentWaypoint.m_PosY)
	{
		m_PosY-=1;
	}
	else if (m_PosY == m_currentWaypoint.m_PosY)
	{

	}


	//if the agent has reached the current waypoint, set the current waypoint to be the next waypoint
	if (m_PosX == m_currentWaypoint.m_PosX && m_PosY == m_currentWaypoint.m_PosY)
	{

		m_pathPosition++;
		if (m_pathPosition != m_path.size())
		{
			m_currentWaypoint = m_path.at(m_pathPosition);
		}
		
	}
}

void MovingChar::Update()
{
	//if the path has not finished being pathed
	if (m_pathPosition != m_path.size())
	{
		MoveToWaypoint();
	}
		
}

void MovingChar::Draw(SDL_Renderer *_renderer)
{
	//draws the agent at it's position on screen
	SDL_Rect temp = { m_PosX, m_PosY, m_size, m_size };

	SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 0);
	SDL_RenderFillRect(_renderer, &temp);

	SDL_RenderCopy(_renderer, m_Texture, NULL, &temp);

}