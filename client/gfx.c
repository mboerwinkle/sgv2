
//Much of this was plagiarized shamelessly from glutplane.c on the OpenGl examples page. Original copyright notice below.

/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees 
	 and is provided without guarantee or warrantee expressed or 
	 implied. This program is -not- in the public domain. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include "gfx.h"
#include "def.h"
#include "../common/stlLoad/loadStl.h"
#define v3f glVertex3f  /* v3f was the short IRIS GL name for glVertex3f */
void rotateView(){
	gluLookAt(0,0,0,facing[0], facing[1], facing[2], upVector[0], upVector[1], upVector[2]);
}
void drawBullet(networkBullet* targ){
	double red, green, blue;
	int* origin = targ->origin;
	float dir[3];
	int mult = 1;
	if(targ->type == 0){//bullets
		red = 1;
		green = 0.7;
		blue = 0;
		mult = 500;
	}else{
		puts("unknown bullet type");
	}
	for(int dim = 0; dim < 3; dim++){
		dir[dim] = (float)(targ->dir[dim])/127*mult;
	}
	glColor3f(red, green, blue);
	point3d end = {origin[0]+dir[0], origin[1]+dir[1], origin[2]+dir[2]};
	drawLine(origin, end);
}
void drawLine(point3d a, point3d b){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	rotateView();
	glLineWidth(2); 
	glBegin(GL_LINES);
	glVertex3f((a[0]-myPos[0]), (a[1]-myPos[1]), (a[2]-myPos[2]));
	glVertex3f((b[0]-myPos[0]), (b[1]-myPos[1]), (b[2]-myPos[2]));
	glEnd();
	glPopMatrix();
}
void drawShip(short type, point3d where, quaternion rot, char color) {
	double red = 1, green = 1, blue = 1;
	if(color == 0){
		red = 0.5;
		green = 0.3;
		blue = 0.7;
	}else if(color == 1){
		red = 0.8;
		green = 0.1;
		blue = 0.2;
	}

/*
		glColor3f(1, 1, 1);
		vector lineDir = {1, 0, 0};
		rotVector(lineDir, rot);
		point3d end = {where[0]+lineDir[0]*5000, where[1]+lineDir[1]*5000, where[2]+lineDir[2]*5000};
		drawLine(where, end);
*/

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	double pos[3];
	for (int i = 0; i < 3; i++) {
		if(where[i] > myPos[i]){//FIXME elegance
			pos[i] = (where[i]-myPos[i]);
		}else{
			pos[i] = -(myPos[i]-where[i]);
		}
	}
	//FIXME speed. use of glTranslated is slow.
	quaternion revRot = {rot[0], -rot[1], -rot[2], -rot[3]};
	double rotMatrix[16];
	generateRotationMatrix(revRot, rotMatrix);
	rotateView();
	glTranslated(pos[0], pos[1], pos[2]);
	glMultMatrixd(rotMatrix);
	glBegin(GL_TRIANGLES);
	model* m = &(models[type]);
	for(int idx = 0; idx < m->triangleCount; idx++){
		struct tri* t = &(m->triangles[idx]);
		vector rotatedNormal = {t->normal[0], t->normal[1], t->normal[2]};
		rotVector(rotatedNormal, rot, rotatedNormal);
		double mult =  0.2+fabs(DOT(rotatedNormal, facing))*0.8;
		glColor3f(red*mult, green*mult, blue*mult);
		v3f(m->points[t->v[0]][0], m->points[t->v[0]][1], m->points[t->v[0]][2]);
		v3f(m->points[t->v[1]][0], m->points[t->v[1]][1], m->points[t->v[1]][2]);
		v3f(m->points[t->v[2]][0], m->points[t->v[2]][1], m->points[t->v[2]][2]);
	}
	glEnd();
	glPopMatrix();
}

static SDL_Window *window;
static SDL_GLContext context;

void gfxClear() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void gfxFlip() {
	SDL_GL_SwapWindow(window);
}
int has_joystick = 0;
void initGfx() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	window = SDL_CreateWindow("Space Game - 3D edition", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		fputs("SDL2 window creation failed", stderr);
		fputs(SDL_GetError(), stderr);
		SDL_Quit();
		return;
	}
	if(SDL_NumJoysticks() > 0){
		SDL_JoystickOpen(0);
		has_joystick = 1;
	}
	context = SDL_GL_CreateContext(window);
	glClearColor(0, 0, 0, 1);
	//Occlusion is a thing
	glEnable(GL_DEPTH_TEST);
	//Edit the projection matrix
	glMatrixMode(GL_PROJECTION);
	//This multiplies a specially designed matrix onto the selected one
//	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5000);
	gluPerspective(120, 8.0/6.0, 1, 10000);
	//Edit the modelview matrix (The "default" matrix)
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);//FIXME not anything to fix here. just mentioning that GL_SMOOTH looks hella nice.//as opposed to GL_FLAT
}

void quitGfx() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
