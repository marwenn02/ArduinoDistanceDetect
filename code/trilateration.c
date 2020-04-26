typedef struct {
	float x;
	float y;
	float z;
} Point;

typedef struct {
	Point *pos;
	float r;
} Vector;

float square(float value) {
	return value * value;
}

float norm(Point *pt) {
	return sqrt(square(pt->x) + square(pt->y) + square(pt->z));
}

float dot(Point *a, Point *b) {
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z); 
}

Point *vectorSubtract(Point *a, Point *b) {
	Point *res = malloc(sizeof *res);
	
	res->x = a->x - b->x;
	res->y = a->y - b->y;
	res->z = a->z - b->z;
	
	return res;	
} //this infers a promise to free!

Point *vectorAdd(Point *a, Point *b) {
	Point *res = malloc(sizeof *res);
	
	res->x = a->x + b->x;
	res->y = a->y + b->y;
	res->z = a->z + b->z;

	return res;
} //this infers a promise to free!

Point *scalarMult(Point *a, float b) {
        Point *res = malloc(sizeof *res);

        res->x = b * a->x;
        res->y = b * a->y;
        res->z = b * a->z;

        return res;
} //this infers a promise to free!

Point *scalarDivide(Point *a, float b) {
        Point *res = malloc(sizeof *res);

        res->x = a->x / b;
        res->y = a->y / b;
        res->z = a->z / b;

        return res;
} //this infers a promise to free!

Point *crossProduct(Point *a, Point *b) {
        Point *res = malloc(sizeof *res);

        res->x = (a->y * b->z) - (a->z * b->y);
        res->y = (a->z * b->x) - (a->x * b->z);
        res->z = (a->x * b->y) - (a->y * b->x);

        return res;
} //this infers a promise to free!

void trilaterate(Point *result, Vector *A, Vector *B, Vector *C) {
	Point *ex, *ey, *ez, *a, *BAdifference, *CAdifference, *normalisedEx, *exMulResX, *eyMulResY, *ezMulResZ, *exMulResXPlusEyMulResY;
	float scalar, exdot, dist, j, resX, resY, resZ, b;
	
	BAdifference = vectorSubtract(B->pos, A->pos);
	scalar = norm(BAdifference);
	ex = scalarDivide(BAdifference, scalar);
	
	CAdifference = vectorSubtract(C->pos, A->pos);
	exdot = dot(ex, CAdifference);
	normalisedEx = scalarMult(ex, exdot);
	a = vectorSubtract(CAdifference, normalisedEx);

	ey = scalarDivide(a, norm(a));
	ez = crossProduct(ex, ey);
	dist = norm(BAdifference);
	j = dot(ey, CAdifference);

	resX = (square(A->r) - square(B->r) + square(dist)) / (2 * dist);
	resY = (square(A->r) - square(C->r) + square(exdot) + square(j)) / (2 * j) - (exdot / j) * resX;
	
	b = square(A->r) - square(resX) - square(resY);

	if(((b > 0) && (b < 0.0000000001)) || ((b < 0) && (b > -0.0000000001))) {
		b = 0; // if abs(b) < 0.0000000001 (float math error)
	}
	if(b < 0) {
		result = malloc(sizeof *result);
		result->x = -1;
		result->y = -1;
		result->z = -1;
		return; // no point found
	}

	resZ = sqrt(b);

	eyMulResY = scalarMult(ey, resY);
	exMulResX = scalarMult(ex, resX);
	ezMulResZ = scalarMult(ez, resZ);

	exMulResXPlusEyMulResY = vectorAdd(exMulResX, eyMulResY);
	result = vectorAdd(A->pos, exMulResXPlusEyMulResY); // take the middle of the two points and put this into the given point (functional programming)
	
	if(BAdifference)
		free(BAdifference);
	if(ex)
		free(ex);
	if(CAdifference)
		free(CAdifference);
	if(normalisedEx)
		free(normalisedEx);
	if(a)
		free(a);
	if(ey)
		free(ey);
	if(ez)
		free(ez);
	if(eyMulResY)
		free(eyMulResY);
	if(exMulResX)
		free(exMulResX);
	if(ezMulResZ)
		free(ezMulResZ);
	if(exMulResXPlusEyMulResY)
		free(exMulResXPlusEyMulResY);
}
