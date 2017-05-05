// Ensure the header file is included only once in multi-file projects
#ifndef CURVE_EVAL_H
#define CURVE_EVAL_H

// Include needed header files
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

class CurveEval : public GlObjects {
public:
	CurveEval();
	GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts, float z);
	GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts, float z);
	GsVec eval_bspline(float t, int k, const GsArray<GsVec>& ctrlpnts);
	GsVec eval_catrom(int t, const GsArray<GsVec>& ctrlpnts, bool minus);
	GsVec eval_bover(int t, const GsArray<GsVec>& ctrlpnts, bool minus);
	float cFunc(int n, int i);
	int factorial(int n);
	float CurveEval::N(int i, int k, float u);
};
#endif // CURVE_EVAL_H