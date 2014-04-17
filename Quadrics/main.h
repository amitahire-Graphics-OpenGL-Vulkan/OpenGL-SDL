# ifndef _MAIN_H
# define _MAIN_H

# include <GL/gl.h>                                     // header for standard OpenGL calls (gl_ calls )
# include <GL/glu.h>                                    // header for OpenGl Utility library (glu_ calls)

# include <stdlib.h>                                    // for exit()
# include <string.h>                                    // for memset
# include <math.h>                                      // standard math library

# include <iostream>                                    // input/output streams (cout, cerr etc.)

# include "SDL.h"                                       // the header gets included from the path supplied by compiler flags
                                                        // see Makefile for this (or try running sdl-config --libs or --cflags

#define SCREEN_WIDTH 800                                // We want our screen width 800 pixels
#define SCREEN_HEIGHT 600                               // We want our screen height 600 pixels
#define SCREEN_DEPTH 16                                 // We want 16 bits per pixel


#define MAX_TEXTURES 1                                  // This says how many texture we will be using

using namespace std;

extern int  VideoFlags;                                 // the pixel format flags

extern SDL_Surface * MainWindow;                        // our drawable portion of the window

typedef unsigned int UINT;
extern UINT g_Textures[MAX_TEXTURES];                   // This is our texture data array

// This loads a texture into openGL from a file(IE , "bitmap.bmp")
void CreateTexture(UINT textureArray[],char *strFileName,int textureID);

// This initializes the whole program
void Init(void);

// This controls our main program loop
void MainLoop(void);

// This toggles between Fullscreen and windowed mode
void ToggleFullScreen(void);

// This is our function that makes creating a window modular and easy
void CreateMyWindow(const char * strWindowName, int width, int heigh, int VideoFlags);

// This allows us to configure our window for openGL and backbuffered
void SetupPixelFormat(void);

// This initializes our screen translations and projections
void InitializeOpenGL(int width, int height);

// This resizes the OpenGL viewport
void SizeOpenGLScreen(int width, int height);

// This draws everything to the screen
void RenderScene(void);

// This handles the keypress events generated when the user presses a key
void KeyPressed(SDL_keysym * keysym);

// This shuts down SDL and quits program
void Quit(int ret_val);

# endif // _MAIN_H


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file for this tutorial (Since the texture tutorial)
// 
// 
//
// DigiBen
// www.GameTutorials.com
//
//
