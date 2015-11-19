//Luis Arevalo
//CMPS 335
//luisA.cpp
//Making a min-boss: Creation, deletion, and, collision.
#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "struct.h"
using namespace std;

GLuint ilhouetteTexture2;
int silhouette2 = 1;
Ppmimage *eImage2 = NULL;
GLuint eTexture3;
int e = 1; 
miniBoss *mbhead = NULL;
extern unsigned char *buildAlphaData3(Ppmimage *img);
/*
void set_mbhealth(Game *game)
{
	game->mb.mbhealth = 125;
}
*/

/*
void physics_Miniboss(Game *g)
{
	//Mini-Boss collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break into enemies into pieces
	a = g->mbhead;
	while (a) {
		//is there a bullet within its radius?
		Bullet *b = g->bhead;
		while (b) {
			d0 = b->pos[0] - a->pos[0];
			d1 = b->pos[1] - a->pos[1];
			dist = (d0*d0 + d1*d1);
			if (dist < (a->radius*a->radius)) {
				//mini-boss hit.
				if (a->radius > 20.0) {
					//break it into pieces.
					Asteroid *ta = a;
					buildAsteroidFragment(ta, a);
					int r = rand()%10+5;
					for (int k=0; k<r; k++) {
						//get the next asteroid position in the array
						Asteroid *ta = new Asteroid;
						buildAsteroidFragment(ta, a);
						//add to front of asteroid linked list
						ta->next = g->ahead;
						if (g->ahead != NULL)
							g->ahead->prev = ta;
						g->ahead = ta;
						g->nasteroids++;
					}
				} else {
					a->color[0] = 1.0;
					a->color[1] = 0.1;
					a->color[2] = 0.1;
					//asteroid is too small to break up
					//delete the asteroid and bullet
					Asteroid *savea = a->next;
					deleteAsteroid(g, a);
					a = savea;
					g->nasteroids--;
				}
				//delete the bullet...
				Bullet *saveb = b->next;
				deleteBullet(g, b);
				b = saveb;
				g->nbullets--;
				if (a == NULL)
					break;
				continue;
			}
			b = b->next;
		}
		if (a == NULL)
			break;
		a = a->next;
	}	
}*/

void delete_miniBoss(Game *g,miniBoss *node)
{
    //remove a node from linked list
    if (node->prev == NULL) 
    {
		if (node->next == NULL) 
		{
			mbhead = NULL;
		}
		else
		{
			node->next->prev = NULL;
			mbhead = node->next;
		}
    }
    else
    {
		if (node->next == NULL)
		{
			node->prev->next = NULL;
		}
		else
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
    
    delete node;
    node = NULL;
}

unsigned char *buildAlphaData3(Ppmimage *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;

		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void render_miniBoss()
{
    //Draw mini-boss
	eImage2 = ppm6GetImage("./images/bossStand.ppm");
	glGenTextures(1, &eTexture3);
	glGenTextures(1, &ilhouetteTexture2);

	glBindTexture(GL_TEXTURE_2D, eTexture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	unsigned char *ilhouetteData = buildAlphaData3(eImage2);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, eImage2->width, eImage2->height, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, ilhouetteData);
	free(ilhouetteData);
}

void maker_miniBoss()
{
	glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, eTexture3);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-22, -17.5);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-22, 17.5);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(22, 17.5);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(22, -17.5);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	
	//set_mbhealth(&);
}
