/////////////////////////////////////////////////////////
// Implementation of CLM search algorithm.
//
/////////////////////////////////////////////////////////
//
// Written by:
//		Xiaoguang Yan
// Email:
//		xiaoguang.yan@gmail.com
// Homepage:
//		http://sites.google.com/site/xgyanhome/
// Version:
//		0.9
// Date:
//		June, 2011.
////////////////////////////////////////////////////////

#include "StdAfx.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "CLM.h"
#include "CLM_priv.h"

/*extern DWORD CountsPerSec;
static LARGE_INTEGER L1;
static LARGE_INTEGER L2;*/

#define NUM_ITER		10		// Default number of iterations.

static void DumpResponse(CvMat * r);

int CLM_Search(CLM_MODEL *Model, IplImage *Image, CLM_SI *Initial, CLM_SI *Final, CLM_OPTIONS *Options)
{
	CLM_SHAPE_MODEL *pShapeData = &Model->ShapeModel;
	CLM_PATCH_MODEL *pTemplateData = &Model->PatchModel;

	int iter;

	CLM_SI Inter;

	// Prepare:
	Inter = *Initial;
	Inter.xy = cvCloneMat(Initial->xy);
	Inter.AlignedXY = cvCloneMat(Initial->AlignedXY);

	int nIterations = Options?Options->NumInterations:NUM_ITER;
	nIterations = nIterations?nIterations:NUM_ITER;

	// Do search:
	float coeffs[8*100];

	int NumPatches = pTemplateData->NumPatches;
	for(iter = 0; iter < nIterations; iter++)
	{
		CLM_SvmSearch(&Inter, Model, Image, coeffs, Options);

		CLM_Optimize(Model, &Inter, coeffs, Options);
	}
	//CLM_DumpSi(&Inter);

	// Assemble return value:
	CLM_CopySi(Final, &Inter);

	// Clean up: ... should make Si a class...
	cvReleaseMat(&Inter.xy);
	cvReleaseMat(&Inter.AlignedXY);

	return 0;
}

