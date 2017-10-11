#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loadStl.h"
#include "../geo/geo.h"

int modelCount = 0;
model* models = NULL;
void loadModels(char* dest);
void loadModel(char* dest, model* target);

void rotatePoints(modelReference* ref, quaternion rotation){
	if(ref->upToDate) return;
	model* myModel = ref->dat;
	for(int pIdx = 0; pIdx < myModel->pointCount; pIdx++){
		rotVector(myModel->points[pIdx], rotation, ref->rotatedPoints[pIdx]);
	}
	ref->upToDate = 1;
}

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
void addEdge(model* t, int a, int b){
	if(a > b){//flip around
		int c = a;
		a = b;
		b = c;
	}
	int new = 1;
	for(int eIdx = 0; eIdx < t->edgeCount; eIdx++){
		if(t->edges[eIdx][0] == a && t->edges[eIdx][1] == b){
			new = 0;
			break;
		}
	}
	if(!new) return;
	(t->edgeCount)++;
	t->edges = realloc(t->edges, sizeof(edge)*t->edgeCount);
	t->edges[t->edgeCount-1][0] = a;
	t->edges[t->edgeCount-1][1] = b;
}
int addPoint(float* pt, model* t){
	for(int pIdx = 0; pIdx < t->pointCount; pIdx++){
		if(pt[0] == t->points[pIdx][0] && pt[1] == t->points[pIdx][1] && pt[2] == t->points[pIdx][2]){
			return pIdx;
		}
	}
	t->pointCount++;
	t->points = realloc(t->points, sizeof(vector)*t->pointCount);
	for(int dim = 0; dim < 3; dim++){
		t->points[t->pointCount-1][dim] = pt[dim];
	}
	return t->pointCount-1;

}
void loadModel(char* dest, model* target){
	FILE* thisModel = fopen(dest, "r");
	if(thisModel == NULL){
		printf("Could not open file %s\n", dest);
		return;
	}
	fseek(thisModel, 80, SEEK_SET);//get past the header
	fread(&(target->triangleCount), sizeof(uint32_t), 1, thisModel);//number of triangles
	struct loadTri* readIn = calloc(target->triangleCount, sizeof(struct loadTri));
	target->triangles = calloc(target->triangleCount, sizeof(struct tri));
	fread(readIn, sizeof(struct loadTri), target->triangleCount, thisModel);
	fclose(thisModel);

	//find points and triangles
	for(int t = 0; t < target->triangleCount; t++){
		target->triangles[t].v[0] = addPoint(readIn[t].p1, target);
		target->triangles[t].v[1] = addPoint(readIn[t].p2, target);
		target->triangles[t].v[2] = addPoint(readIn[t].p3, target);
		//find normal
		triNormal(&(readIn[t]));//FIXME we should be able to just do it without the readin data
		for(int dim = 0; dim < 3; dim++){
			target->triangles[t].normal[dim] = readIn[t].vec[dim];
		}
	}
	free(readIn);
	//find radius
	target->radius = 0;
	for(int p = 0; p < target->pointCount; p++){
		double newRadius = VECLEN(target->points[p]);
		if(newRadius > target->radius) target->radius = newRadius;
	}
	//find edges
	target->edges = NULL;
	target->edgeCount = 0;
	for(int triIdx = 0; triIdx < target->triangleCount; triIdx++){
		for(int side = 0; side < 3; side++){
			addEdge(target, target->triangles[triIdx].v[side], target->triangles[triIdx].v[(side+1)%3]);
		}
	}
	printf("model %s had %d triangles, %d edges, %d points\n", dest, target->triangleCount, target->edgeCount, target->pointCount);
}

void triNormal(struct loadTri* t){
	vectorf a = {t->p1[0]-t->p2[0], t->p1[1]-t->p2[1], t->p1[2]-t->p2[2]};
	vectorf b = {t->p2[0]-t->p3[0], t->p2[1]-t->p3[1], t->p2[2]-t->p3[2]};
	vectorf normal;
	CROSS(a, b, normal);
	vecfNormalize(normal);
	t->vec[0] = normal[0];
	t->vec[1] = normal[1];
	t->vec[2] = normal[2];
}
