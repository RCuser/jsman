//Jeremy Martinez
//
// This file adds all sound to every aspect of the game, first it generates a source for each
// sound and then sets the volume for each sound. There is currently only one sound that loops
// which is used for the menu and background music of the game. Each sound is then placed in 
// their respectable positions in order to play at the right time, such as at button press or
// collision. The other action this file takes care of is the character jump, which happens when
// the space button is pressed. The characters y velocity is incremented until it reaches a limit,
// then the character falls back down until it reaches a platform.
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <time.h>
#include "ppm.h"
#include "chrisR.h"
#include "danbiY.h"
#include </usr/include/AL/alut.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "struct.h"

ALuint alBuffer;
ALuint alSource;
ALuint alJump;
ALuint alBGM;
ALuint alMenu;
ALuint alDeath;
ALuint alHit;
ALuint alKill;
ALuint alHitE;
ALuint alShotgun;
ALuint alSwitch;

extern bool touchPlatform;
extern int keys[];

void playSound(ALuint source) 
{  
    alSourcePlay(alSource); 
}

void cleanupOpenal() 
{
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
	
}

int initOpenal() {
	//Get started right here.
	alutInit(0, NULL);
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	//ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./shoot1.wav");
	//Source refers to the sound.
	//ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 2.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	//BGM
	alBuffer = alutCreateBufferFromFile("./sample2.wav");
	//Generate source
	alGenSources(1, &alBGM);
	alSourcei(alBGM, AL_BUFFER, alBuffer);
	//Set volume
	alSourcef(alBGM, AL_GAIN, 0.4f);
	alSourcef(alBGM, AL_PITCH, 1.0f);
	alSourcei(alBGM, AL_LOOPING, AL_TRUE);
	//jump
	alBuffer = alutCreateBufferFromFile("./jump.wav");
	//generate source
	alGenSources(1, &alJump);
	alSourcei(alJump, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alJump, AL_GAIN, 0.7f);
	alSourcef(alJump, AL_PITCH, 1.0f);
	alSourcei(alJump, AL_LOOPING, AL_FALSE);
	//Menu
	alBuffer = alutCreateBufferFromFile("./Menu.wav");
	//Generate source 
	alGenSources(1, &alMenu);
	alSourcei(alMenu, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alMenu, AL_GAIN, 1.2f);
	alSourcef(alMenu, AL_PITCH, 1.0f);
	alSourcei(alMenu, AL_LOOPING, AL_FALSE);
	//death
	alBuffer = alutCreateBufferFromFile("./death.wav");
	//Generate Source
	alGenSources(1, &alDeath);
	alSourcei(alDeath, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alDeath, AL_GAIN, 2.0f);
	alSourcef(alDeath, AL_PITCH, 1.0f);
	alSourcei(alDeath, AL_LOOPING, AL_FALSE);
	//Hit confirm
	alBuffer = alutCreateBufferFromFile("./hit.wav");
	//Generate Source
	alGenSources(1, &alHit);
	alSourcei(alHit, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alHit, AL_GAIN, 2.0f);
	alSourcef(alHit, AL_PITCH, 1.0f);
	alSourcei(alHit, AL_LOOPING, AL_FALSE);
	//enemy death
	alBuffer = alutCreateBufferFromFile("./kill.wav");
	//Generate Source
	alGenSources(1, &alKill);
	alSourcei(alKill, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alKill, AL_GAIN, 2.0f);
	alSourcef(alKill, AL_PITCH, 1.0f);
	alSourcei(alKill, AL_LOOPING, AL_FALSE);
	//enemy hit
	alBuffer = alutCreateBufferFromFile("./enemyhit.wav");
	//Generate Source
	alGenSources(1, &alHitE);
	alSourcei(alHitE, AL_BUFFER, alBuffer);
	//set volume 
	alSourcef(alHitE, AL_GAIN, 2.0f);
	alSourcef(alHitE, AL_PITCH, 1.0f);
	alSourcei(alHitE, AL_LOOPING, AL_FALSE);
	//shotgun
	alBuffer = alutCreateBufferFromFile("./shoot.wav");
	//Generate Source
	alGenSources(1, &alShotgun);
	alSourcei(alShotgun, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alShotgun, AL_GAIN, 2.0f);
	alSourcef(alShotgun, AL_PITCH, 1.0f);
	alSourcei(alShotgun, AL_LOOPING, AL_FALSE);
	//weapon switch
	alBuffer = alutCreateBufferFromFile("./switch.wav");
	//Generate Source
	alGenSources(1, &alSwitch);
	alSourcei(alSwitch, AL_BUFFER, alBuffer);
	//set volume
	alSourcef(alSwitch, AL_GAIN, 2.0f);
	alSourcef(alSwitch, AL_PITCH, 1.0f);
	alSourcei(alSwitch, AL_LOOPING, AL_FALSE);

	return 1;
}

void soundMenu()
{
	initOpenal();
	alSourcePlay(alMenu);
}

void soundHit()
{
	initOpenal();
        alSourcePlay(alHit);
	usleep(2000);	
}

void soundKill()
{
	initOpenal();
	alSourcePlay(alKill);
	usleep(2000);
}

void soundDeath()
{
	initOpenal();
	alSourcePlay(alDeath);
	usleep(2000);
}

void soundBGM()
{
	initOpenal();
	alSourcePlay(alBGM);
	usleep(20000);
}

void soundJump()
{
	initOpenal();
	alSourcePlay(alJump);
	usleep(2000);   
}

void soundBullet()
{
	initOpenal();
	alSourcePlay(alSource);
	usleep(20000);
}

void soundHitEnemy()
{
    initOpenal();
    alSourcePlay(alHitE);
    usleep(2000);
}

void soundShotgun()
{
    initOpenal();
    alSourcePlay(alShotgun);
    usleep(2000);
}

void soundSwitch()
{
    initOpenal();
    alSourcePlay(alSwitch);
    usleep(2000);
}

void gravity(Game *game) {
	if(touchPlatform == 0) {
	game->character.velocity.y -= 1;
	game->character.center.y = game->character.center.y + game->character.velocity.y;
	}
	if(touchPlatform == 1)
	game->character.velocity.y = 0;
}

void characterJump(Game *game) 
{
	if (keys[XK_Up] ) 
	{
		if(touchPlatform == 1) 
		{
			while(game->character.velocity.y < 17)
				game->character.velocity.y = game->character.velocity.y + 1; 
			if(touchPlatform > 0)
			{
				soundJump();
			}
			touchPlatform = 0;	
		}
	} 
}

void drawHealth(float x, int health)
{
        int y = 550;
	x = x+240;
	if(x < 635)
		x = 635;
	//draw black box
	glColor3ub(0,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x-105,y-10);
	glVertex2i(x-105,y+10);
	glVertex2i(x+105,y+10);
	glVertex2i(x+105,y-10);
	glEnd();
	glColor3ub(200,0,0);
	glBegin(GL_QUADS);
	glVertex2i(x-100,y-7);
	glVertex2i(x-100,y+7);
	glVertex2i(x+100,y+7);
	glVertex2i(x+100,y-7);
	glEnd();
	glColor3ub(0,200,0);
	glBegin(GL_QUADS);
	glVertex2i(x-100,y-7);
	glVertex2i(x-100,y+7);
	glVertex2i(x-100+(health * 2),y+7);
	glVertex2i(x-100+(health * 2),y-7);
	glEnd();
}

void stage2(Game *game) 
{
	
	game->platform[4].width = 252; 
	game->platform[4].height = 24; 
	game->platform[4].center.x = 2650; 
	game->platform[4].center.y = 80;
	game->platform[5].width = 252; 
	game->platform[5].height = 24; 
	game->platform[5].center.x = 3280;
	game->platform[5].center.y = 330;
	game->platform[6].width = 252; 
	game->platform[6].height = 24; 
	game->platform[6].center.x = 3280;
	game->platform[6].center.y = 110;
	game->platform[7].width = 252; 
	game->platform[7].height = 24; 
	game->platform[7].center.x = 3910;
	game->platform[7].center.y = 210;
	game->platform[8].width = 252; 
	game->platform[8].height = 24; 
	game->platform[8].center.x = 4540;
	game->platform[8].center.y = 210;
	game->platform[9].width = 252; 
	game->platform[9].height = 24; 
	game->platform[9].center.x = 5170;
	game->platform[9].center.y = 330;
	game->platform[10].width = 252; 
	game->platform[10].height = 24; 
	game->platform[10].center.x = 5170;
	game->platform[10].center.y = 110;
	game->platform[11].width = 252; 
	game->platform[11].height = 24; 
	game->platform[11].center.x = 5800;
	game->platform[11].center.y = 210;
	
}

void stage2enemies(Game *game) 
{
	game->walkers[1].width = 22;/////////////////////////////////////////////////
	game->walkers[1].height = 14;
	game->walkers[1].center.x = 820;
	game->walkers[1].center.y = 260;
	game->walkers[1].lives = 3;
	game->walkers[2].width = 22;
	game->walkers[2].height = 14;
	game->walkers[2].center.x = 1880;
	game->walkers[2].center.y = 260;
	game->walkers[2].lives = 3;
	game->walkers[3].width = 22;
	game->walkers[3].height = 14;
	game->walkers[3].center.x = 1940;
	game->walkers[3].center.y = 260;
	game->walkers[3].lives = 3;
	game->walkers[4].width = 22;
	game->walkers[4].height = 14;
	game->walkers[4].center.x = 2600;
	game->walkers[4].center.y = 410;
	game->walkers[4].lives = 3;
	game->walkers[5].width = 22;
	game->walkers[5].height = 14;
	game->walkers[5].center.x = 2680;
	game->walkers[5].center.y = 410;
	game->walkers[5].lives = 3;
	game->walkers[6].width = 22;
	game->walkers[6].height = 14;
	game->walkers[6].center.x = 2500;
	game->walkers[6].center.y = 110;
	game->walkers[6].lives = 3;
	game->walkers[7].width = 22;
	game->walkers[7].height = 14;
	game->walkers[7].center.x = 2700;
	game->walkers[7].center.y = 110;
	game->walkers[7].lives = 3;
	game->walkers[8].width = 22;
	game->walkers[8].height = 14;
	game->walkers[8].center.x = 3100;
	game->walkers[8].center.y = 360;
	game->walkers[8].lives = 3;
	game->walkers[9].width = 22;
	game->walkers[9].height = 14;
	game->walkers[9].center.x = 3300;
	game->walkers[9].center.y = 140;
	game->walkers[9].lives = 3;
	game->walkers[10].width = 22;
	game->walkers[10].height = 14;
	game->walkers[10].center.x = 4340;
	game->walkers[10].center.y = 240;
	game->walkers[10].lives = 3;
	game->walkers[11].width = 22;
	game->walkers[11].height = 14;
	game->walkers[11].center.x = 4640;
	game->walkers[11].center.y = 240;
	game->walkers[11].lives = 3;
	game->walkers[12].width = 22;
	game->walkers[12].height = 14;
	game->walkers[12].center.x = 5070;
	game->walkers[12].center.y = 360;
	game->walkers[12].lives = 3;
	game->walkers[13].width = 22;
	game->walkers[13].height = 14;
	game->walkers[13].center.x = 5300;
	game->walkers[13].center.y = 140;
	game->walkers[13].lives = 3;
	game->walkers[14].width = 22;
	game->walkers[14].height = 14;
	game->walkers[14].center.x = 5600;
	game->walkers[14].center.y = 240;
	game->walkers[14].lives = 3;
	game->walkers[15].width = 22;
	game->walkers[15].height = 14;
	game->walkers[15].center.x = 5800;
	game->walkers[15].center.y = 240;
	game->walkers[15].lives = 3;
	game->walkers[16].width = 22;
	game->walkers[16].height = 14;
	game->walkers[16].center.x = 6000;
	game->walkers[16].center.y = 240;
	game->walkers[16].lives = 3;
	game->walkers[17].width = 22;
	game->walkers[17].height = 14;
	game->walkers[17].center.x = 7000;
	game->walkers[17].center.y = 140;
	game->walkers[17].lives = 3;
	game->walkers[18].width = 22;
	game->walkers[18].height = 14;
	game->walkers[18].center.x = 7500;
	game->walkers[18].center.y = 240;
	game->walkers[18].lives = 3;
	game->walkers[19].width = 22;
	game->walkers[19].height = 14;
	game->walkers[19].center.x = 7600;
	game->walkers[19].center.y = 240;
	game->walkers[19].lives = 3;
	game->walkers[20].width = 22;
	game->walkers[20].height = 14;
	game->walkers[20].center.x = 8000;
	game->walkers[20].center.y = 240;
	game->walkers[20].lives = 3;
	game->walkers[21].width = 22;
	game->walkers[21].height = 14;
	game->walkers[21].center.x = 7000;
	game->walkers[21].center.y = 140;
	game->walkers[21].lives = 3;
	game->walkers[22].width = 22;
	game->walkers[22].height = 14;
	game->walkers[22].center.x = 7500;
	game->walkers[22].center.y = 240;
	game->walkers[22].lives = 3;
	game->walkers[23].width = 22;
	game->walkers[23].height = 14;
	game->walkers[23].center.x = 7600;
	game->walkers[23].center.y = 240;
	game->walkers[23].lives = 3;
	game->walkers[24].width = 22;
	game->walkers[24].height = 14;
	game->walkers[24].center.x = 8000;
	game->walkers[24].center.y = 240;
	game->walkers[24].lives = 3;
	game->walkers[25].width = 22;
	game->walkers[25].height = 14;
	game->walkers[25].center.x = 8000;
	game->walkers[25].center.y = 240;
	game->walkers[25].lives = 3;
	game->walkers[26].width = 22;
	game->walkers[26].height = 14;
	game->walkers[26].center.x = 7000;
	game->walkers[26].center.y = 140;
	game->walkers[26].lives = 3;
	game->walkers[27].width = 22;
	game->walkers[27].height = 14;
	game->walkers[27].center.x = 7500;
	game->walkers[27].center.y = 240;
	game->walkers[27].lives = 3;
	game->walkers[28].width = 22;
	game->walkers[28].height = 14;
	game->walkers[28].center.x = 7600;
	game->walkers[28].center.y = 240;
	game->walkers[28].lives = 3;
	game->walkers[29].width = 22;
	game->walkers[29].height = 14;
	game->walkers[29].center.x = 8000;
	game->walkers[29].center.y = 240;
	game->walkers[29].lives = 3;
	game->walkers[30].width = 22;
	game->walkers[30].height = 14;
	game->walkers[30].center.x = 7600;
	game->walkers[30].center.y = 240;
	game->walkers[30].lives = 3;
	game->walkers[31].width = 22;
	game->walkers[31].height = 14;
	game->walkers[31].center.x = 8000;
	game->walkers[31].center.y = 240;
	game->walkers[31].lives = 3;
	game->walkers[32].width = 22;
	game->walkers[32].height = 14;
	game->walkers[32].center.x = 7600;
	game->walkers[32].center.y = 240;
	game->walkers[32].lives = 3;
	game->walkers[31].width = 22;
	game->walkers[31].height = 14;
	game->walkers[31].center.x = 8000;
	game->walkers[31].center.y = 240;
	game->walkers[31].lives = 3;
}
