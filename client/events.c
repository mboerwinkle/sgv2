#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <SDL2/SDL.h>
#include "def.h"
#include "../common/controlMap.h"
controlMap ctls;
int spKeyAction(SDL_Keycode key, int pressed){
	switch(key){
		case SDLK_r://respawn
			if(pressed){
				ctls.spawn = 0;
			}else{
				ctls.spawn = -1;
			}
			return 1;
		case SDLK_w://accelerate
			ctls.accel = pressed;
			return 1;
		case SDLK_a:
			ctls.roll = pressed;
			return 1;
		case SDLK_s:
			ctls.accel = -pressed;
			return 1;
		case SDLK_d:
			ctls.roll = -pressed;
			return 1;
		case SDLK_UP:
			ctls.pitch = pressed;
			return 1;
		case SDLK_LEFT:
			ctls.yaw = -pressed;
			return 1;
		case SDLK_DOWN:
			ctls.pitch = -pressed;
			return 1;
		case SDLK_RIGHT:
			ctls.yaw = pressed;
			return 1;
		case SDLK_SPACE:
			ctls.fire = pressed;
			return 1;
		default:
			return 0;
	}
}
int spJoyAxis(Uint8 axis, Sint16 value){
	switch(axis){
		case 0:
			if(abs(value) < DEADZONE){
				ctls.roll = 0;
			}else ctls.roll = -value/CONTROLLERAXISMAX;
			return 1;
		case 1:
			if(abs(value) < DEADZONE){
				ctls.accel = 0;
			}else ctls.accel = -value/CONTROLLERAXISMAX;
			return 1;
		case 4:
			if(abs(value) < DEADZONE){
				ctls.pitch = 0;
			}else ctls.pitch = -value/CONTROLLERAXISMAX;
			return 1;
		case 3:
			if(abs(value) < DEADZONE){
				ctls.yaw = 0;
			}else ctls.yaw = value/CONTROLLERAXISMAX;
			return 1;
		default:
			return 0;
	}
}

int handleEvents(){
	int send = 0;//if controls need to be sent
	static SDL_Event evnt;
	while(SDL_PollEvent(&evnt)){
		if(evnt.type == SDL_QUIT){
			return 1;
		}
		else if(evnt.type == SDL_KEYDOWN){
			if(spKeyAction(evnt.key.keysym.sym, 1)) send = 1;
		}
		else if(evnt.type == SDL_KEYUP){
			if(spKeyAction(evnt.key.keysym.sym, 0)) send = 1;
		}
		else if(evnt.type == SDL_JOYAXISMOTION){
			if(spJoyAxis(evnt.jaxis.axis, evnt.jaxis.value)) send = 1;
		}
		else if(evnt.type == SDL_CONTROLLERBUTTONDOWN){
		}
		else if(evnt.type == SDL_CONTROLLERBUTTONUP){
		}
	}
	if(send){
		//send controls to server
		char msg[3+sizeof(controlMap)] = "CTL";
		memcpy(msg+3, &ctls, sizeof(controlMap));
		sendto(sockfd, msg, 3+sizeof(controlMap), 0, (struct sockaddr*)&sendAddr, sizeof(sendAddr));
		ctls.spawn = -1;
	}
	return 0;
}


