#include <stdio.h>
#include <stdlib.h>
#include "collisions.h"
#include "common/stlLoad/loadStl.h"
int modelCollide(modelReference* a, point3d pa, modelReference* b, point3d pb);//FIXME make modelReferences not be pointers.

void handleCollisions(ship* ref, ship** col, int colCount){
	point3d pos;
	P3DEQUAL(pos, ref->myPosition);
	double rad = ref->myModel.dat->radius;
	rotatePoints(&(ref->myModel), ref->myRotation);
	for(int idx = 0; idx < colCount; idx++){
		point3d pos2;
		P3DEQUAL(pos2, col[idx]->myPosition);
		double rad2 = col[idx]->myModel.dat->radius;
		if(DISTANCE(pos, pos2) > rad+rad2) continue;
		rotatePoints(&(col[idx]->myModel), col[idx]->myRotation);
		if(modelCollide(&(ref->myModel), pos, &(col[idx]->myModel), pos2)){
		}
	}
}

int modelCollide(modelReference* a, point3d pa, modelReference* b, point3d pb){
	int offset[3];
	vector e[2];//for applying offsets
	model* am = a->dat;
	model* bm = b->dat;
	vector* apts = a->rotatedPoints;
	vector* bpts = b->rotatedPoints;
	for(int dim = 0; dim < 3; dim++){
		offset[dim] = pb[dim]-pa[dim];
	}
	for(int pass = 0; pass < 2; pass++){
		for(int ta = 0; ta < am->triangleCount; ta++){
			struct tri* t = &(am->triangles[ta]);
			for(int eb = 0; eb < bm->edgeCount; eb++){
				for(int dim = 0; dim < 3; dim++){
					e[0][dim] = bpts[bm->edges[eb][0]][dim] + offset[dim];
					e[1][dim] = bpts[bm->edges[eb][1]][dim] + offset[dim];
				}
				if(intersect_triangle(e[0], e[1], apts[t->v[0]], apts[t->v[1]], apts[t->v[2]])) return 1;
			}
		}
		if(pass == 0){
			for(int dim = 0; dim < 3; dim++){
				offset[dim] *= -1;
			}
			void* xchange = a;
			a = b;
			b = xchange;

			xchange = am;
			am = bm;
			bm = xchange;

			xchange = apts;
			apts = bpts;
			bpts = xchange;
		}
	}
	return 0;
}
#define EPSILON 0.00001
int intersect_triangle(vector l1, vector l2, vector vert0, vector vert1, vector vert2){//credit Tomas Moller and Ben Trumbore "Fast, minimum storage ray/triangle intersection"
	float dir[3];//FIXME these should all be doubles
	dir[0] = l2[0]-l1[0];//FIXME copy the macros from RELIGN
	dir[1] = l2[1]-l1[1];
	dir[2] = l2[2]-l1[2];
	float t, u, v;
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;
	/* find vectors for two edges sharing vert0 */

	for(int dim = 0; dim < 3; dim++){
		edge1[dim] = vert1[dim]-vert0[dim];
		edge2[dim] = vert2[dim]-vert0[dim];
	}
//	SUB(edge1, vert1, vert0);
//	SUB(edge2, vert2, vert0);

	/* begin calculating determinant - also used to calculate U parameter */

//	CROSS(pvec, dir, edge2);
	//crossf(dir, edge2, pvec);
	CROSS(dir, edge2, pvec);

	/* if determinant is near zero, ray lies in plane of triangle */

	det = DOT(edge1, pvec);
	//det = dotf(edge1, pvec);
	/* the non-culling branch */
	if (det > -EPSILON && det < EPSILON) return 0;
	inv_det = 1.0 / det;
	/* calculate distance from vert0 to ray origin */
	//SUB(tvec, l1, vert0);
	for(int dim = 0; dim < 3; dim++){
		tvec[dim] = l1[dim]-vert0[dim];
	}
	/* calculate U parameter and test bounds */
	u = DOT(tvec, pvec) * inv_det;
//	u = dotf(tvec, pvec) * inv_det;
	if (u < 0.0 || u > 1.0) return 0;
	/* prepare to test V parameter */
//	CROSS(qvec, tvec, edge1);
	//crossf(tvec, edge1, qvec);
	CROSS(tvec, edge1, qvec);
	/* calculate V parameter and test bounds */
	v = DOT(dir, qvec) * inv_det;
//	v = dotf(dir, qvec) * inv_det;
	if (v < 0.0 || u + v > 1.0) return 0;
	/* calculate t, ray intersects triangle */
	t = DOT(edge2, qvec) * inv_det;
	//t = dotf(edge2, qvec) * inv_det;
	if(t > 1 || t <0) return 0;
	return 1;
}
