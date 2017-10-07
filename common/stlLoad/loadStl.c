#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loadStl.h"
#include "../geo/geo.h"

int modelCount = 0;
model* models = NULL;
void loadModels(char* dest);
void loadModel(char* dest, model* target);

void loadModels(char* dest){
	char index[100];
	sprintf(index, "%s/index.txt", dest);
	FILE* modelList = fopen(index, "r");
	if(modelList == NULL){
		puts("could not open model list");
	}
	fscanf(modelList, "%d", &modelCount);
	models = calloc(modelCount, sizeof(model));
	char path[100];
	char completepath[100];
	for(int modelIdx = 0; modelIdx < modelCount;  modelIdx++){
		fscanf(modelList, "%s", path);
		sprintf(completepath, "%s%s", dest, path);
		printf("loading model %s\n", completepath);
		loadModel(completepath, &(models[modelIdx]));
	}
	fclose(modelList);
}

void loadModel(char* dest, model* target){
	FILE* thisModel = fopen(dest, "r");
	if(thisModel == NULL){
		printf("Could not open file %s\n", dest);
		return;
	}
	fseek(thisModel, 80, SEEK_SET);//get past the header
	fread(&(target->triangleCount), sizeof(uint32_t), 1, thisModel);//number of triangles
	printf("model %s had %d triangles\n", dest, target->triangleCount);
	target->triangles = calloc(target->triangleCount, sizeof(struct tri));
	fread(target->triangles, sizeof(struct tri), target->triangleCount, thisModel);
	fclose(thisModel);
	//find radius
	target->radius = 0;
	for(int triIdx = 0; triIdx < target->triangleCount; triIdx++){
		double newRadius = VECLEN(target->triangles[triIdx].p1);
		if(newRadius > target->radius) target->radius = newRadius;
		newRadius = VECLEN(target->triangles[triIdx].p2);
		if(newRadius > target->radius) target->radius = newRadius;
		newRadius = VECLEN(target->triangles[triIdx].p3);
		if(newRadius > target->radius) target->radius = newRadius;

		triNormal(&(target->triangles[triIdx]));
	}
	//find normal
}

void triNormal(struct tri* t){
	vectorf a = {t->p1[0]-t->p2[0], t->p1[1]-t->p2[1], t->p1[2]-t->p2[2]};
	vectorf b = {t->p2[0]-t->p3[0], t->p2[1]-t->p3[1], t->p2[2]-t->p3[2]};
	vectorf normal;
	//crossf(a, b, normal);
	CROSS(a, b, normal);
	vecfNormalize(normal);
	t->vec[0] = normal[0];
	t->vec[1] = normal[1];
	t->vec[2] = normal[2];
}
