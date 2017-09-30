#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <SDL2/SDL.h>
#include "def.h"
#include "../common/controlMap.h"
controlMap ctls;
int weapSel = 1;

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
	if(abs(value) > DEADZONE){
		if(value < 0) value+=DEADZONE;
		else value-=DEADZONE;
	}else{
		value = 0;
	}
	switch(axis){
		case 0:
			ctls.roll = -value/(CONTROLLERAXISMAX-DEADZONE);
			return 1;
		case 1:
			ctls.accel = -value/(CONTROLLERAXISMAX-DEADZONE);
			return 1;
		case 4:
			ctls.pitch = -value/(CONTROLLERAXISMAX-DEADZONE);
			return 1;
		case 3:
			ctls.yaw = value/(CONTROLLERAXISMAX-DEADZONE);
			return 1;
		case 5:
			if(value > (CONTROLLERAXISMAX-DEADZONE)/2){
				ctls.fire = weapSel;
			}else{
				ctls.fire = -1;
			}
		default:
			return 0;
	}
}

int spJoyButton(Uint8 button, int value){
	if(button == 8){
		puts("spawning");
		if(value){
			ctls.spawn = 0;
			return 1;
		}else ctls.spawn = -1;
	}else if(button == 4){
		if(!value) return 0;
		weapSel--;
	}else if(button == 5){
		if(!value) return 0;//FIXME inelegant
		weapSel++;
	}else printf("unknown button %d\n", button);
	return 0;
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
		else if(evnt.type == SDL_JOYBUTTONDOWN || evnt.type == SDL_JOYBUTTONUP){
			if(spJoyButton(evnt.jbutton.button, evnt.jbutton.state)) send = 1;
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


