#include <stdio.h>
#include <stdlib.h>
#include "collisions.h"
#include "common/stlLoad/loadStl.h"
int modelCollide(model* a, point3d pa, quaternion ra, model* b, point3d pb, quaternion rb);

void handleCollisions(ship* ref, ship** col, int colCount){
	point3d pos;
	p3dEqual(pos, ref->myPosition);
	double rad = ref->myModel->radius;
	for(int idx = 0; idx < colCount; idx++){
		point3d pos2;
		p3dEqual(pos2, col[idx]->myPosition);
		double rad2 = col[idx]->myModel->radius;
		//if(p3dDistance(pos, pos2) > rad+rad2) continue;
		if(modelCollide(ref->myModel, pos, ref->myRotation, col[idx]->myModel, pos2, col[idx]->myRotation)){
//			puts("collided ships");
		}
	}
}

int modelCollide(model* a, point3d pa, quaternion ra, model* b, point3d pb, quaternion rb){//FIXME... we might have to perform the same rotation many times per tick
	int offset[3];
	for(int dim = 0; dim < 3; dim++){
		offset[dim] = pb[dim]-pa[dim];
	}
	for(int ta = 0; ta < a->triangleCount; ta++){
		struct tri tria;
		rotfVector(a->triangles[ta].p1, ra, tria.p1);
		rotfVector(a->triangles[ta].p2, ra, tria.p2);
		rotfVector(a->triangles[ta].p3, ra, tria.p3);
		for(int eb = 0; eb < b->edgeCount; eb++){
			struct edge edgeb;
			rotfVector(b->edges[eb].a, rb, edgeb.a);
			rotfVector(b->edges[eb].b, rb, edgeb.b);
			for(int dim = 0; dim < 3; dim++){
				edgeb.a[dim]+=offset[dim];
				edgeb.b[dim]+=offset[dim];
			}
			if(intersect_triangle(edgeb.a, edgeb.b, tria.p1, tria.p2, tria.p3)) return 1;
		}
	}

//MAD Code Dupe

	for(int dim = 0; dim < 3; dim++){
		offset[dim] = pa[dim]-pb[dim];
	}
	for(int tb = 0; tb < b->triangleCount; tb++){
		struct tri trib;
		rotfVector(b->triangles[tb].p1, rb, trib.p1);
		rotfVector(b->triangles[tb].p2, rb, trib.p2);
		rotfVector(b->triangles[tb].p3, rb, trib.p3);
		for(int ea = 0; ea < a->edgeCount; ea++){
			struct edge edgea;
			rotfVector(a->edges[ea].a, ra, edgea.a);
			rotfVector(a->edges[ea].b, ra, edgea.b);
			for(int dim = 0; dim < 3; dim++){
				edgea.a[dim]+=offset[dim];
				edgea.b[dim]+=offset[dim];
			}
			if(intersect_triangle(edgea.a, edgea.b, trib.p1, trib.p2, trib.p3)) return 1;
		}
	}
	return 0;
}
#define EPSILON 0.00001
int intersect_triangle(vectorf l1, vectorf l2, vectorf vert0, vectorf vert1, vectorf vert2){//credit Tomas Moller and Ben Trumbore "Fast, minimum storage ray/triangle intersection"
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
