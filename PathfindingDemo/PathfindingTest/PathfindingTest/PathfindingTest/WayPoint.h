/// \file WayPoint.h
/// \brief Waypoint struct declaration
/// \author Robert Brede

#ifndef _PATH_H_
#define _PATH_H_

#include <vector>

#include "Node.h"

//waypoints are used to store on screen positions, for a agent to move to
//they are based off of nodes on the node map
struct Waypoint
{
	Waypoint()
	{
		m_PosX = 0;
		m_PosY = 0;
	}
	//if a node is passed into the constructor, the waypoint positions x and y are set to mirror the centre of the node's on screen position
	Waypoint(Node *_node)
	{
		m_PosX = (_node->m_posX * _node->m_size);
		m_PosY = (_node->m_posY * _node->m_size);
	}




	int m_PosX;
	int m_PosY;


};

#endif //_PATH_H_
