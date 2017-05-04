# include "curve_eval.h"

CurveEval::CurveEval()
{
}

//----------------------------------------------------------------------------------------------------------
GsVec CurveEval::eval_bezier(float t, const GsArray<GsVec>& ctrlpnts, float z) {
	int n = ctrlpnts.size() - 1;
	GsVec2 point = GsVec2(0.0f,0.0f);
	for (int i = 0; i <= n; i++) {
		point = point + (ctrlpnts(i) * cFunc(n, i) * pow(t, i) * pow((1 - t), (n - i)));
	}
	return GsVec(point.x, point.y, z);
}

//----------------------------------------------------------------------------------------------------------
GsVec CurveEval::eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts, float z) {
	int n = ctrlpnts.size();
	GsVec2 point = GsVec2(0.0f, 0.0f);
	float b;
	for (int i = 1; i <= n; i++) {
		b = 1;
		for (int j = 1; j <= n; j++) {
			if (i != j) {
				b = b*((t - j) / (i - j));
			}
		}
		point = point + (ctrlpnts(i-1)*b);
	}
	return GsVec(point.x, point.y, z);
}

//----------------------------------------------------------------------------------------------------------
GsVec CurveEval::eval_bspline(float t, int k, const GsArray<GsVec>& ctrlpnts, float z) {
	int n = ctrlpnts.size() - 1;
	GsVec2 point = GsVec2(0.0f, 0.0f);
	for (int i = 0; i <= n; i++) {
		point = point + (ctrlpnts(i)*N(i, 3, t));
	}
	return GsVec(point.x, point.y, z);
}

//----------------------------------------------------------------------------------------------------------
GsVec CurveEval::eval_catrom(int t, const GsArray<GsVec>& ctrlpnts, bool minus) {
	GsVec point;
	GsVec I = (ctrlpnts(t + 1) - ctrlpnts(t - 1)) / 2;
	if (minus) {
		point = ctrlpnts(t) - (I / 3);
	}
	else {
		point = ctrlpnts(t) + (I / 3);
	}
	return point;
}

//----------------------------------------------------------------------------------------------------------
GsVec CurveEval::eval_bover(int t, const GsArray<GsVec>& ctrlpnts, bool minus) {
	GsVec point;

	float dplus = dist(ctrlpnts(t+1), ctrlpnts(t));
	float dminus = dist(ctrlpnts(t), ctrlpnts(t-1));

	GsVec vhatplus  = (ctrlpnts(t+1) - ctrlpnts(t)) / dplus;
	GsVec vhatminus = (ctrlpnts(t) - ctrlpnts(t-1)) / dminus;

	GsVec v = (dplus*vhatminus + dminus*vhatplus) / (dplus + dminus);

	if (minus) {
		point = ctrlpnts(t) - ((dminus * v) / 3);
	}
	else {
		point = ctrlpnts(t) + ((dplus * v) / 3);
	}
	return point;
}

//----------------------------------------------------------------------------------------------------------
float CurveEval::cFunc(int n, int i) {
	return (factorial(n) / (factorial(i)*factorial(n - i)));
}

//----------------------------------------------------------------------------------------------------------
int CurveEval::factorial(int n){
	if (n == 0 || n == 1) {
		return 1;
	}
	else {
		return n * factorial(n - 1);
	}
}

//----------------------------------------------------------------------------------------------------------
float CurveEval::N(int i, int k, float u) {
	float ui = float(i);
	if (k == 1) {
		return ui <= u && u < ui + 1 ? 1.0f : 0;
	}
	else {
		return ((u - ui) / (k - 1)) * N(i, k - 1, u) + ((ui + k - u) / (k - 1)) * N(i + 1, k - 1, u);
	}
}