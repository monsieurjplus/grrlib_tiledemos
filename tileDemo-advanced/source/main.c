/*===========================================
    NoNameNo TileDemo
    A sample code to show how to use tile/tileset
    The map have been drawn with the mega mighty
    TileStudio (http://tilestudio.sourceforge.net/)
    Special Greetings to TLB for some GFx ;=)
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>


///////////////////////////////////////////////
// Includes of sources files from the project
// Make sure to include it in Makefile
///////////////////////////////////////////////

#include "gfx/tile1.h"		// Map's tileset (all blocks 32x32)
#include "gfx/perso.h"		// Perso's tileset (all frames 64x64)
#include "gfx/bg.h"       	// Background (repeated bitmap)
#include "gfx/nonameno.h" 	// Letters displayed on upper layer (all letters 32x32)
#include "gfx/BMfont1.h"	// Font
#include "maps/Map1.h"		// Map data (tiles position and collides)

#define TileMapWidth (32)	// Width of map tiles
#define TileMapHeight (32)	// Height of map tiles


int main() {
    // Perso init position
	int startPosX = 9;
	int startPosY = 6;
	
	// Perso size
	int persoHalfHeight = 1;
	int persoHeight     = 2;
	int persoHalfWidth  = 1;
	int persoWidth      = 2;
	
	// Map position
	int posX = startPosX;
    int posY = startPosY;
	
	bool checkCollideY = false;
	
    int cptx = 0, cpty = 0;
    int bgx = -32, bgy = -32;
	
	
	// Counters used to process each tile of the map
    int x = 0;
    int y = 0;
	
	// Directions X/Y, defined by controls
    int dirx = 0;
    int diry = 0;
	
	// Frame number to draw of the perso
    float idperso = 0;

    bool isPaused = false;


    ////////////////
	// SYSTEM INITS
	////////////////

    // Init Video mode, aspect ratio, FIFO buffer, FAT system, so on.
    GRRLIB_Init();
		
	// Init Controls (Wiimote, Balance board, Sensor bar, so on.)
    WPAD_Init();
	

    /////////////////////////////
	// LOAD TEXTURES / TILESETS
    /////////////////////////////
		
	// Load texture from image (c array)
    GRRLIB_texImg *tex_tile1 = GRRLIB_LoadTexture(tile1);
	// Handle the texture to be used (set number of tiles, start position, offset position, so on)
    GRRLIB_InitTileSet(tex_tile1, TileMapWidth, TileMapHeight, 0);
	
	// Load texture from image (c array)
    GRRLIB_texImg *tex_perso = GRRLIB_LoadTexture(perso);
	// Handle the texture to be used (set number of tiles, start position, offset position, so on)
    GRRLIB_InitTileSet(tex_perso, 64, 64, 0);
	
	// Load texture from image (c array)
    GRRLIB_texImg *tex_bg = GRRLIB_LoadTexture(bg);

    // Load texture to draw text (native printf doesn't work with GRRLib)
    GRRLIB_texImg *tex_BMfont1 = GRRLIB_LoadTexture(BMfont1);
	// Handle the texture to be used, in order to draw characters from the fonttype
    GRRLIB_InitTileSet(tex_BMfont1, 32, 32, 32);


    ////////////////
    // ENDLESS LOOP
	////////////////
    while(1) {
	    // Get status of Wii controller
        WPAD_ScanPads();
		
		// If HOME button is pressed, the loop is interrupted. 0 is the Wiimote channel (Wiimote number 1 out of 4).
		// WPAD_ButtonsXXX returns a hexadecimal value representing all buttons down. The mask on HOME button allows to check if this buttons is pressed
		// (HOME button hexadecimal value: 0x0080)
        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
          break;
		}
		
		// Pause
		if (WPAD_ButtonsDown(0) & WPAD_BUTTON_PLUS) {
		  if (isPaused == true) {
		    isPaused = false;
		  } else {
		    isPaused = true;
		  }
		}

        
		if (isPaused == false) {
			// Set directions X/Y values and perso frame to display, depending controls
			// Be careful, here, Wiimore is used horizontally, so controls are inverted
        
			// We allow to move horizontally while falling
			if (dirx == 0) {
				if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) { // If DOWN button is held: go right
					dirx    = -4; // move perso 4px on right
					idperso = 1;  // select frame 1 of the perso (watching on his right)
				} else if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) { // If UP button is held: go left
					dirx    = 4;  // move perso 4px on left
					idperso = 8;  // select frame 4 of the perso (watching on his left)
				}
			}
		
			// Jumping or crouching is only possible on the ground
			if (diry == 0) {
				if (dirx == 0 && (WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT)) { // If LEFT button is held and perso is stopped: go bottom
					diry    = -4; // move perso 4px on bottom
					idperso = 15; // select frame 15 of the perso (back)
				} else if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) { // If A button is held and perso is on the ground: jump
					diry    = 16; // move perso 16px on top
					idperso = 15; // select frame 15 of the perso (back)
				}
			}


			// If no movement, select frame 0 of the perso (front of the screen)
			if ((dirx == 0) && (diry == 0)) {
				idperso = 0;
			}


			if 	(
					(
						// Checks if ground at bottom (grey ground only)
						(Map1Info[posY + persoHeight + 1][posX + persoWidth] == 1) || (Map1Info[posY + persoHeight + 1][posX + persoHalfWidth] == 1)
					) 
					|| 
					(
						// Checks if ladder at bottom. Allow to move freely on ladders
						(Map1Info[posY + persoHeight + 1][posX + persoWidth] == 10064) || (Map1Info[posY + persoHeight + 1][posX + persoHalfWidth] == 10064)
					)
				) 
			{
				// Does nothing there
			} else { 
				// Falling
				diry = -4;
			}




			if (dirx < 0) { // If goes on RIGHT
				// Checks collide on right to column
				if 	(
						(Map1Info[posY + persoHalfHeight][posX + persoWidth + 1] == 8) 
						|| 
						(Map1Info[posY + persoHalfHeight][posX + persoWidth + 1] == 2) 
						|| 
						(Map1Info[posY + persoHeight][posX + persoWidth + 1] == 8) 
						|| 
						(Map1Info[posY + persoHeight][posX + persoWidth + 1] == 2)
					) 
				{
					dirx = 0;
				} else {
					// Animation walking on right
					idperso++;
					if (idperso > 7) {
						idperso = 1;
					}
				}
			} else if (dirx > 0) { // If goes on LEFT
				// Checks collide on left to column
				if	(
						(Map1Info[posY + persoHalfHeight][posX] == 8) 
						|| 
						(Map1Info[posY + persoHalfHeight][posX] == 2) 
						|| 	
						(Map1Info[posY + persoHeight][posX] == 8) 
						|| 	
						(Map1Info[posY + persoHeight][posX] == 2)
					) 
				{	
					dirx = 0;
				} else {
					// Animation walking on left
					idperso++;
					if (idperso > 14) {
						idperso = 8;
					}
				}
			}


			// Jumping / Falling
			if (diry < 0) { 
				// Checks if bottom tile is grey ground
				if ((Map1Info[posY + persoHeight + 1][posX + persoWidth] == 1) || (Map1Info[posY + persoHeight + 1][posX + persoHalfWidth] == 1)) {
					diry = 0;
				}
			}
		
			// Animation while jumping / falling / climbing a ladder
			if (diry != 0) {
				idperso++;
				if (idperso > 22) {
					idperso = 16;
				}
			}

		
			// Move the background horizontally
			if (dirx > 0) {
				bgx++;
			} else if (dirx < 0) {
				bgx--;
			}
			if ((bgx > -1) || (bgx < -63)) {
				bgx = -32;
			}


			// Move the map horizontally depending perso movements.
			// This allows to display map 4pixels per 4pixels, instead of tile per tile
			cptx += dirx;
			if (cptx == TileMapWidth) {
				cptx = 0;
				dirx = 0;
				posX--;
			} else if (cptx == -TileMapWidth) {
				cptx = 0;
				dirx = 0;
				posX++;
			}

        
			// Move the background vertically
			if (diry > 0) {
				bgy++;
			} else if (diry < 0) {
				bgy--;
			}
			if ((bgy > -1) || (bgy < -63)) {
				bgy = -32;
			}


			// Move the map vertically depending perso movements
			// This allows to display map 4pixels per 4pixels, instead of tile per tile
			cpty += diry;
			if (cpty == TileMapHeight) {
				cpty = 0;
				posY--;
				checkCollideY = true;
			} else if (cpty == -TileMapHeight) {
				cpty = 0;
				posY++;
				checkCollideY = true;
			} else {
				checkCollideY = false;
			}
		
			if (checkCollideY == true) {
				// We check if perso is on the ground or on a ladder.
				// It allows to let the animation of fall/jump to continue to run instead.
				if	(
						(
							(Map1Info[posY + persoHeight + 1][posX + persoWidth] == 1) 
							|| 
							(Map1Info[posY + persoHeight + 1][posX + persoHalfWidth] == 1)
						) 
						|| 
						(
							(Map1Info[posY + persoHeight + 1][posX + persoWidth] == 10064) 
							|| 
							(Map1Info[posY + persoHeight + 1][posX + persoHalfWidth] == 10064)			
						)
					) 
				{
					diry = 0;
				}
			}
		}
		
		
		
		//////////////////////////
		// DRAWING THE BACKGROUND
		//////////////////////////
		
        GRRLIB_DrawImg(bgx,       // X of upper left corner
		               bgy,       // Y of upper left corner
					   tex_bg,    // texture
					   0,         // rotation
					   1,         // scale X
					   1,         // scale Y
					   0xFFFFFFFF // color
					  );


		////////////////////////////////
		// DRAWING EACH TILE OF THE MAP
        ////////////////////////////////

        // Reminder:
		//   TileMapWidth  = 32px
		//   TileMapHeight = 32px

        // Process 17 tiles on height of the screen (17 x 32px = 512px)
        for (y = 0; y <= (16); y++) {
			// Process 22 tiles on width of the screen (22 * 32 = 704px)
            for (x = 0; x <= (21); x++) {
			    // Only displays tiles with content (others are "transparent" on the screen)
                if (Map1Data[y + posY - startPosY][x + posX - startPosX] != 0) {
                    GRRLIB_DrawTile(x * TileMapWidth + cptx - TileMapWidth, 					// X position to draw (upper left)
					                y * TileMapHeight + cpty - TileMapHeight,					// Y position to draw (upper left)
									tex_tile1,													// texture of the tileset
									0,															// rotation
									1,															// scale X
									1,															// scale Y
									0xFFFFFFFF,													// color
									Map1Data[y + posY - startPosY][x + posX - startPosX] - 1 	// Frame to display (from the tileset)
								   );
                }
            }
        }
		
		
		/////////////////////
		// DRAWING THE PERSO
		/////////////////////
		
        GRRLIB_DrawTile(TileMapWidth * startPosX,  // horizontal center of the screen
						TileMapHeight * startPosY, // vertical center of the screen
						tex_perso, 				    // texture of the tileset
						0, 						    // rotation
						1, 						    // scale X
						1, 						    // scale Y
						0xFFFFFFFF, 			    // color
						(int)idperso			    // Frame to display (from the tileset)
					   );

        

        // Debugging

		int printDirX = dirx;
		int printDirY = diry;
		int printPosX = posX;
		int printPosY = posY;		
		GRRLIB_Printf(0, 0, tex_BMfont1, 0xFFFFFFFF, 1, "X:%d Y:%d SX:%d SY:%d", printDirX, printDirY, printPosX, printPosY);
		GRRLIB_Printf(0, 32, tex_BMfont1, 0xFFFFFFFF, 1, "CX:%d CY:%d", cptx, cpty);
		
		
		if (isPaused == true) {
		  GRRLIB_Printf(128, 64, tex_BMfont1, 0xFFFFFFFF, 1, "PAUSE");
		}
		
				
		/////////////
		// RENDERING
		/////////////
		
		GRRLIB_Render();
    }


    // Free textures
    GRRLIB_FreeTexture(tex_tile1);
    GRRLIB_FreeTexture(tex_perso);
    GRRLIB_FreeTexture(tex_bg);
	GRRLIB_FreeTexture(tex_BMfont1);
	
	
	// Free screen access / frame buffers / FIFO to allow 
	// next app to handle the system without any trouble
    GRRLIB_Exit();

    exit(0);
}
