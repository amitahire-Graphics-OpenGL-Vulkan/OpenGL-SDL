#ifndef _TERRAIN_H
#define _TERRAIN_H

#define MAP_SIZE        1024                // This is the size of our .raw height map
#define STEP_SIZE       16                  // This is width and height of each triangle grid

typedef unsigned char BYTE;

// This returns the height (0 to 255) from a heightmap given an X and Y
int Height(BYTE *pHeightMap, int X, int Y);

// This loads a .raw file of a certain size from the file
void LoadRawFile(char *strName, int nSize, BYTE *pHeightMap);

// This turns heightmap data into primitives and draws them to the screen
void RenderHeightMap(BYTE *pHeightMap);


#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This file holds all our defines and functions for dealing with the terrain. 
// As more tutorials are put out, we will add to this file to better handle
// terrain rendering.  This is actually a huge subject, so there is much to code.
//
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
