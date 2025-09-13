//
// Created by atolstenko on 2/9/2023.
//

#include <iostream>

#include "HexagonGameOfLife.h"
void HexagonGameOfLife::Step(World& world) 
{
	for (int row = 0; row < world.SideSize(); row++) 
	{
		for (int col = 0; col < world.SideSize(); col++)
		{
            Point2D current = Point2D(col, row);
            int neighbors = CountNeighbors(world, current);
			
			if (neighbors < 2 || neighbors > 2) 
			{
				world.SetNext(current, false);
			} 
			else 
			{
				world.SetNext(current, true);
			}
		}
	}
}

int HexagonGameOfLife::CountNeighbors(World& world, Point2D point) 
{ 
	int neighbors = 0;
	int offset = point.y % 2 - 1;
	for (int row = point.y - 1; row <= point.y + 1; row++) 
	{
		for (int col = point.x; col <= point.x + 1; col++) 
		{
			if (row == point.y) 
			{				
				if (col == point.x) 
				{
					if (world.Get(Point2D(col - 1, row))) 
					{
						neighbors++;
					}
					continue;
					
				}
				if (world.Get(Point2D(col, row))) 
				{
					neighbors++;
				}
				continue;
			}
			if (world.Get(Point2D(col + offset, row)))
			{				
                neighbors++;
			}
		}
	}

	std::cout << std::endl;
	return neighbors; 
}
