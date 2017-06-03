/// \file MovingChar.h
/// \brief MovingChar class declaration
/// \author Robert Brede

#ifndef _MOVINGCHAR_H_
#define _MOVINGCHAR_H_

#include <vector>
#include <SDL2\SDL.h>

#include "Waypoint.h"

class NodeManager;
class Node;

//class to define an agant who can follow a path of waypoint
//the path from start to goal of nodes is turned into waypoints for the agent to follow
class MovingChar
{
private:
	

public:

	//the renderer is passed in so the agents texture can be loaded
	MovingChar(SDL_Renderer *_renderer);

	//generates a path of waypoints from the start to the goal node
	void m_genPath(Node *_start, Node *_goal);

	int m_PosX;
	int m_PosY;

	int m_size;

	SDL_Texture *m_Texture;

	std::vector<Waypoint> m_path;
	//stores the waypoint the agent is currently moving towards
	Waypoint m_currentWaypoint;
	//stores the position in the path vector that the current waypoint is at
	int m_pathPosition;

	//moves the agent up to 1 pixel in x and 1 pixel in y towards the current waypoint
	void MoveToWaypoint();

	//called each frame, containing all the actions the agents needs to perform each frame
	void Update();

	//called to draw the agent, called after update, so that it's position onscreen reflects it's current actual position
	void Draw(SDL_Renderer *_renderer);
	
};

#endif //_MOVINGCHAR_H_
