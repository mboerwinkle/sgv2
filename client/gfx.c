
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
void drawBullet(bullet* targ){
	GLfloat red = 0, green = 0, blue = 0;
	if(targ->type == 0){
		red = 1.0;
	}
	point3d end;
	for (int dim = 0; dim < 3; dim++) {//finding the end point from the vector.
		end[dim] = targ->pos[dim]+targ->dir[dim];
	}
	glColor3f(red, green, blue);
	drawLine(targ->pos, end);
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
void drawShip(short type, point3d where, quaternion rot, char color, char* name) {
//	point3d end = {where[0], where[1], where[2]+5};
//	drawLine(where, end);
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
	for(int idx = 0; idx < models[type].triangleCount; idx++){
		struct tri* t = &(models[type].triangles[idx]);
		vector rotatedNormal = {t->vec[0], t->vec[1], t->vec[2]};
		rotVector(rotatedNormal, rot);
		double mult =  fabs(dot(rotatedNormal, facing));
		glColor3f(0.5*mult, 0.3*mult, 0.7*mult);
		v3f(t->p1[0], t->p1[1], t->p1[2]);
		v3f(t->p2[0], t->p2[1], t->p2[2]);
		v3f(t->p3[0], t->p3[1], t->p3[2]);
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
	window = SDL_CreateWindow("Space Game - 3D edition", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_OPENGL);
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
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5000);
	//Edit the modelview matrix (The "default" matrix)
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);//FIXME not anything to fix here. just mentioning that GL_SMOOTH looks hella nice.//as opposed to GL_FLAT
}

void quitGfx() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
