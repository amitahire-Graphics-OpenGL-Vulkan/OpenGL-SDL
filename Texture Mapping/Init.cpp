//***********************************************************************//
//                                                                       //
//      - "Talk to me like I'm a 3 year old!" Programming Lessons -      //
//                                                                       //
//      $Author:        DigiBen     digiben@gametutorials.com            //
//                                                                       //
//      $Program:       Texturing                                        //
//                                                                       //
//      $Description:   Texture maps a shape                             //
//                                                                       //
//      $Date:          7/3/01                                           //
//                                                                       //
//***********************************************************************//


#include "main.h"                                       // includes our function protoypes and globals                        

int VideoFlags = 0;                                     // Video Flags for the Create Window function
SDL_Surface * MainWindow = NULL;                        // drawing surface on the SDL window

typedef unsigned char byte;


///////////////////////////////////      SWAP      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  This swaps 2 elements we pass to it
/////
///////////////////////////////////      SWAP      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

static void swap(unsigned char & a, unsigned char & b)
{
    unsigned char temp;

    temp = a;
    a    = b;
    b    = temp;

    return;
}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

/////////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  This creates a texture in OpenGL that we can texture map 
/////
/////////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateTexture(unsigned int textureArray[],char *strFileName,int textureID)
{
    SDL_Surface *pBitmap[1];

    if( strFileName == NULL )                           // Return from the function if no file name was passed in
        return ;

    // We need to load the texture data, so we use a cool function that SDL offers.
    
    pBitmap[0] = SDL_LoadBMP(strFileName);              // Load the bitmap and store the data

    if(pBitmap[0] == NULL)                                // If we can't load the file, quit!
    {
        cerr << " Failed loading " << strFileName << " : " << SDL_GetError() << endl;
        Quit(0);
    }

    // Now that we have the texture data, we need to register our texture with OpenGL
    // To do this we need to call glGenTextures().  The 1 for the first parameter is
    // how many texture we want to register this time (we could do a bunch in a row).
    // The second parameter is the array index that will hold the reference to this texture.

    // Generate a texture with the associative texture ID stored in the array
    glGenTextures(1, &textureArray[textureID]);

    // Now that we have a reference for the texture, we need to bind the texture
    // to tell OpenGL this is the reference that we are assigning the bitmap data too.
    // The first parameter tells OpenGL we want are using a 2D texture, while the
    // second parameter passes in the reference we are going to assign the texture too.
    // We will use this function later to tell OpenGL we want to use this texture to texture map.

    // Bind the texture to the texture arrays index and init the texture
    glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

    // WARNING   : GO THROUGH THESE FEW LINES FOR SWAPPING ROWS ONLY IF YOU REALLY NEED TO, OR ELSE SKIP
    // THE LINES MARKED BELOW. Just take it for granted that these lines of code swap rows in the texture 
    // as required by us.
    
    // <SKIP> <SKIP> <SKIP> <SKIP>   (FROM HERE)        -------------------
    //
    // IMPORTANT : SDL loads Bitmaps differently when compared to the default windows loader. The row 0
    // corresponds to the top row and NOT the bottom row. Therefore if we don't invert the order of the rows,
    // then the textures will appear (vertically) inverted.
    // Therefore we must reverse the ordering of the rows in the data of the loaded surface ( the member
    //  'pixels' of the structure)
    
    // Rearrange the pixelData 
    
    int width  = pBitmap[0] -> w;
    int height = pBitmap[0] -> h;
    unsigned char * data = (unsigned char *) (pBitmap[0] -> pixels);         // the pixel data
    unsigned char * newData = new unsigned char[width*height*3];
    int channels = 3; // R,G,B

    int BytesPerPixel = pBitmap[0] -> format -> BytesPerPixel ; 

    //////////// This is how we swap the rows :
    // For half the rows, we swap row 'i' with row 'height - i -1'. (if we swap all the rows
    // like this and not the first half or the last half, then we get the same texture again !
    //
    // Now these rows are not stored as 2D arrays, instead they are stored as a long 1D array.
    // So each row is concatenated after the previous one to make this long array. Our swap 
    // function swaps one byte at a time and therefore we swap BytesPerPixel (= total bits per pixel)
    // bytes succesively.
    //
    // so the three loops below are :
    // for the first half of the rows
    //   for all the width (which is width of image * BytesPerPixel, where BytesPerPixel = total bits per pixel).
    //   (Here, for each pixel, we have to increment j by total bits per pixel (to get to next pixel to swap))
    //      for(each byte in this pixel i.e k = 0 to BytesPerPixel - 1, i.e BytesPerPixel bytes.
    //        swap the byte with the corresponding byte in the 'height -i -1'th row ('i'th row from bottom)
    for( int i = 0 ; i < (height / 2) ; ++i )
        for( int j = 0 ; j < width * BytesPerPixel; j += BytesPerPixel )
            for(int k = 0; k < BytesPerPixel; ++k)
                swap( data[ (i * width * BytesPerPixel) + j + k], data[ ( (height - i - 1) * width * BytesPerPixel ) + j + k]);
 
    // </SKIP> </SKIP> </SKIP> </SKIP>    (TO HERE)   -------------------
   
    // the following lines extract R,G and B values from any bitmap

    for(int i = 0; i < (width * height); ++i)
    {
        byte r,g,b;                                                // R,G and B that we will put into pImage

        Uint32 pixel_value = 0;                                    // 32 bit unsigned int (as dictated by SDL)

        // the following loop extracts the pixel (however wide it is 8,16,24 or 32) and 
        // creates a long with all these bytes taken together.
        
        for(int j = BytesPerPixel - 1 ; j >=0; --j)                // for each byte in the pixel (from the right)
        {
            pixel_value = pixel_value << 8;                        // left shift pixel value by 8 bits
            pixel_value = pixel_value | data[ (i * BytesPerPixel) + j ];  // then make the last 8 bits of pixel value  =
        }                                                                 // the byte that we extract from pBitmap's data

        SDL_GetRGB(pixel_value, pBitmap[0] -> format, (Uint8 *)&r, (Uint8 *)&g, (Uint8 *)&b);     // here we get r,g,b from pixel_value which is stored in the form specified by pBitmap->format

        newData[(i * channels) + 0] = r;          // in our tImage classes we store r first
        newData[(i * channels) + 1] = g;          // then g
        newData[(i * channels) + 2] = b;          // and finally b (for bmps - three channels only)

        pixel_value = 0;                                           // reset pixel , else we get incorrect values of r,g,b
    }


    // Now comes the important part, we actually pass in all the data from the bitmap to
    // create the texture. Here is what the parameters mean in gluBuild2DMipmaps():
    // (We want a 2D texture, 3 channels (RGB), bitmap width, bitmap height, It's an RGB format,
    //  the data is stored as unsigned bytes, and the actuall pixel data);

    // What is a Mip map?  Mip maps are a bunch of scaled pictures from the original.  This makes
    // it look better when we are near and farther away from the texture map.  It chooses the
    // best looking scaled size depending on where the camera is according to the texture map.
    // Otherwise, if we didn't use mip maps, it would scale the original UP and down which would
    // look not so good when we got far away or up close, it would look pixelated.

    // Build Mipmaps (builds different versions of the picture for distances - looks better)

    // Build Mipmaps (builds different versions of the picture for distances - looks better)
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap[0]->w, pBitmap[0]->h, GL_RGB, GL_UNSIGNED_BYTE, newData);

    // Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
    // is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
    // but looks blochy and pixilated.  Good for slower computers though.  Read more about 
    // the MIN and MAG filters at the bottom of main.cpp
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

    // Now we need to free the bitmap data that we loaded since openGL stored it as a texture
    delete [] newData;
    
    SDL_FreeSurface(pBitmap[0]);                        // Free the texture data we dont need it anymore
}


/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function TOGGLES between FULLSCREEN and WINDOWED mode
/////
/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void ToggleFullScreen(void)
{
    if(SDL_WM_ToggleFullScreen(MainWindow) == 0)        // try to toggle fullscreen mode for window 'main_window' 
    {
        cerr << "Failed to Toggle Fullscreen mode : " << SDL_GetError() << endl;   // report error in case toggle fails
        Quit(0);
    }
}


///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  This function CREATES our WINDOW for drawing the GL stuff
/////
///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateMyWindow(const char * strWindowName, int width, int height, int VideoFlags)
{
    MainWindow = SDL_SetVideoMode(width, height, SCREEN_DEPTH, VideoFlags);     // SCREEN_DEPTH is macro for bits per pixel

    if( MainWindow == NULL )                            // if window creation failed
    {
        cerr << "Failed to Create Window : " << SDL_GetError() << endl;         // report error 
        Quit(0);
    }

    SDL_WM_SetCaption(strWindowName, "GameTutorials");  // set the window caption (first argument) and icon caption (2nd arg)
}


/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  Sets the pixel format for openGL and video flags for SDL
/////
/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SetupPixelFormat(void)
{
    //////// SURFACE IS THE DRAWABLE PORTION OF AN SDL WINDOW \\\\\\\\*
    
    /////////////  we set the common flags here
    VideoFlags    = SDL_OPENGL;                         // it's an openGL window
    VideoFlags   |= SDL_HWPALETTE;                      // exclusive access to hardware colour palette
    VideoFlags   |= SDL_RESIZABLE;                      // the window must be resizeable

    const SDL_VideoInfo * VideoInfo = SDL_GetVideoInfo();     // query SDL for information about our video hardware

    if(VideoInfo == NULL)                                     // if this query fails
    {
        cerr << "Failed getting Video Info : " << SDL_GetError() << endl;          // report error
        Quit(0);
    }

    ///////////// we set the system dependant flags here
    if(VideoInfo -> hw_available)                      // is it a hardware surface
        VideoFlags |= SDL_HWSURFACE;
    else
        VideoFlags |= SDL_SWSURFACE;

    // Blitting is fast copying / moving /swapping of contiguous sections of memory
    // for more about blitting check out : http://www.csc.liv.ac.uk/~fish/HTML/blitzman/bm_blitter.html
    if(VideoInfo -> blit_hw)                            // is hardware blitting available
        VideoFlags |= SDL_HWACCEL;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );      // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   SCREEN_DEPTH);         // size of depth buffer 
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);       // we aren't going to use the stencil buffer
    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     // this and the next three lines set the bits allocated per pixel -
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   // - for the accumulation buffer to 0
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);
}


//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////    This function resizes the viewport for OpenGL.
/////
//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SizeOpenGLScreen(int width, int height)            // Initialize The GL Window
{
    if (height==0)                                        // Prevent A Divide By Zero error
    {
        height=1;                                        // Make the Height Equal One
    }

    glViewport(0,0,width,height);                        // Make our viewport the whole window
                                                        // We could make the view smaller inside
                                                        // Our window if we wanted too.
                                                        // The glViewport takes (x, y, width, height)
                                                        // This basically means, what our our drawing boundries

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

                                                        // Calculate The Aspect Ratio Of The Window
                                                        // The parameters are:
                                                        // (view angle, aspect ration of the width to the height, 
                                                        //  The closest distance to the camera before it clips, 
                  // FOV        // Ratio                //  The farthest distance before it stops drawing)
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}


/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////         This function handles all the initialization for openGL
/////
/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height)
{
   glEnable( GL_DEPTH_TEST );                             // Enables Depth Test i.e enables use of depth buffer for drawing

 /////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    // This allows us to use texture mapping, otherwise we just use colors.

    glEnable(GL_TEXTURE_2D);                            // Enable Texture Mapping

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

    SizeOpenGLScreen(width, height);                       // resize the OpenGL Viewport to the given height and width
}

//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     This function handles the keypress events generated when the user presses a key
//////
//////////////////////////////////      HANDLE KEY PRESS EVENT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void HandleKeyPressEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                                  // which key have we got
    {
        case SDLK_F1 :                                     // if it is F1
            ToggleFullScreen();                            // toggle between fullscreen and windowed mode
            break;

        case SDLK_ESCAPE:                                  // if it is ESCAPE
            Quit(0);                                       // quit after cleaning up
    }
}
////////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     create the window and calling the initialization functions
//////
////////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int main(void)
{
    // print user instructions
    cout << " Hit the F1 key to Toggle between Fullscreen and windowed mode" << endl;
    cout << " Hit ESC to quit" << endl;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                      // try to initialize SDL video module
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      // report error if it fails
        return 1;                                             // we use this instead of Quit because SDL isn't yet initialized
    }

    // Set up the format for the pixels of the OpenGL drawing surface
    SetupPixelFormat();
    // Create our window, we pass caption for the window, the width, height and video flags required
    CreateMyWindow("www.GameTutorials.com - TextureMapping", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

    // Initializes our OpenGL drawing surface
    
    Init();

    // Run our message loop
    MainLoop();

    // quit main returning success
    return 0;
}


////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////      This will shutdown SDL and quit the program
//////
////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Quit(int ret_val)
{
    SDL_Quit();                                            // shuts down SDL stuff

    exit(ret_val);                                         // quit the program
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//  
//
// We added the CreateTexture() function.  This will come in handy moving on.
// You will use this a lot.  When using texture maps, we also need to enable 
// GL_TEXTURE_2D.
//
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
