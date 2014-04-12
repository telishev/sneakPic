#include "path_approximation.h"

/*
An Algorithm for Automatically Fitting Digitized Curves
by Philip J. Schneider
from "Graphics Gems", Academic Press, 1990
*/

/*  fit_cubic.c	*/									
/*	Piecewise cubic fitting code	*/

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "geom_helpers.h"
#include "path_builder.h"

typedef QPointF *BezierCurve;

/*
 *  B0, B1, B2, B3 :
 *	Bezier multipliers
 */
static inline double B0 (double u)
{
    double tmp = 1.0 - u;
    return (tmp * tmp * tmp);
}


static inline double B1 (double u)
{
    double tmp = 1.0 - u;
    return (3 * u * (tmp * tmp));
}

static inline double B2(double u)
{
    double tmp = 1.0 - u;
    return (3 * u * u * tmp);
}

static inline double B3 (double u)
{
    return (u * u * u);
}

#define MAXPOINTS	1000		/* The most points you can have */
/*
 *  GenerateBezier :
 *  Use least-squares method to find Bezier control points for region.
 *
 */
static void GenerateBezier (QPointF *d, int first, int last, double *uPrime, QPointF tHat1, QPointF tHat2, QPointF *bezCurve)
{
    int 	i;
    QPointF 	A[MAXPOINTS][2];	/* Precomputed rhs for eqn	*/
    int 	nPts;			/* Number of pts in sub-curve */
    double 	C[2][2];			/* Matrix C		*/
    double 	X[2];			/* Matrix X			*/
    double 	det_C0_C1,		/* Determinants of matrices	*/
    	   	det_C0_X,
	   		det_X_C1;
    double 	alpha_l,		/* Alpha values, left and right	*/
    	   	alpha_r;
    QPointF 	tmp;			/* Utility variable		*/
    nPts = last - first + 1;

 
    /* Compute the A's	*/
    for (i = 0; i < nPts; i++) 
      {
		    QPointF		v1, v2;
		    v1 = tHat1 * B1(uPrime[i]);
		    v2 = tHat2 * B2(uPrime[i]);
		    A[i][0] = v1;
		    A[i][1] = v2;
      }

    /* Create the C and X matrices	*/
    C[0][0] = 0.0;
    C[0][1] = 0.0;
    C[1][0] = 0.0;
    C[1][1] = 0.0;
    X[0]    = 0.0;
    X[1]    = 0.0;

    for (i = 0; i < nPts; i++)
      {
        C[0][0] += geom::dot (A[i][0], A[i][0]);
		    C[0][1] += geom::dot (A[i][0], A[i][1]);
    /*					C[1][0] += V2Dot(&A[i][0], &A[i][1]);*/	
		    C[1][0] = C[0][1];
		    C[1][1] += geom::dot (A[i][1], A[i][1]);

		    tmp = d[first + i] -
	        	              ( d[first] * B0(uPrime[i])+
		      		              d[first] * B1(uPrime[i]) +
	                  		    d[last] * B2(uPrime[i]) +
	                    	    d[last] * B3(uPrime[i]));
	

	        X[0] += geom::dot (A[i][0], tmp);
	        X[1] += geom::dot (A[i][1], tmp);
      }

    /* Compute the determinants of C and X	*/
    det_C0_C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
    det_C0_X  = C[0][0] * X[1]    - C[0][1] * X[0];
    det_X_C1  = X[0]    * C[1][1] - X[1]    * C[0][1];

    /* Finally, derive alpha values	*/
    if (det_C0_C1 == 0.0)
		  det_C0_C1 = (C[0][0] * C[1][1]) * 10e-12;

    alpha_l = det_X_C1 / det_C0_C1;
    alpha_r = det_C0_X / det_C0_C1;


    /*  If alpha negative, use the Wu/Barsky heuristic (see text) */
	/* (if alpha is 0, you get coincident control points that lead to
	 * divide by zero in any subsequent NewtonRaphsonRootFind() call. */
    if (alpha_l < 1.0e-6 || alpha_r < 1.0e-6)
      {
		    double	dist = geom::distance (d[last], d[first]) / 3.0;

		    bezCurve[0] = d[first];
		    bezCurve[3] = d[last];
        bezCurve[1] = bezCurve[0] + tHat1 * dist;
        bezCurve[2] = bezCurve[3] + tHat2 * dist;
      }

    /*  First and last control points of the Bezier curve are */
    /*  positioned exactly at the first and last data points */
    /*  Control points 1 and 2 are positioned an alpha distance out */
    /*  on the tangent vectors, left and right, respectively */
    bezCurve[0] = d[first];
    bezCurve[3] = d[last];
    bezCurve[1] = bezCurve[0] + tHat1 * alpha_l;
    bezCurve[2] = bezCurve[3] + tHat2 * alpha_r;
}



/*
 *  FitCurve :
 *  	Fit a Bezier curve to a set of digitized points 
 */



/*
 * ComputeLeftTangent, ComputeRightTangent, ComputeCenterTangent :
 *Approximate unit tangents at endpoints and "center" of digitized curve
 */
static QPointF ComputeLeftTangent(QPointF *d, int end)
{
    QPointF	tHat1;
    tHat1 = d[end+1] - d[end];
    tHat1 /= geom::norm (tHat1);
    return tHat1;
}

static QPointF ComputeRightTangent(QPointF *d, int end)
{
    QPointF	tHat2;
    tHat2 = d[end-1] - d[end];
    tHat2 /= geom::norm (tHat2);
    return tHat2;
}


static QPointF ComputeCenterTangent(QPointF *d, int center)
{
    QPointF	V1, V2, tHatCenter;

    V1 = d[center-1] - d[center]  ;
    V2 = d[center]   - d[center+1];
    tHatCenter = V1 + V2;
    tHatCenter  /= geom::norm (tHatCenter);
    return tHatCenter;
}

/*
 *  Bezier :
 *  	Evaluate a Bezier curve at a particular parameter value
 * 
 */
static QPointF BezierII (int degree, QPointF *V, double t)
{
    int 	i, j;		
    QPointF 	Q;	        /* Point on curve at parameter t	*/
    QPointF 	Vtemp[4];		/* Local copy of control points		*/

    /* Copy array	*/
    for (i = 0; i <= degree; i++)
		  Vtemp[i] = V[i];

    /* Triangle computation	*/
    for (i = 1; i <= degree; i++)
		  for (j = 0; j <= degree-i; j++)
	    	Vtemp[j] = (1.0 - t) * Vtemp[j] + t * Vtemp[j+1];

    Q = Vtemp[0];
    return Q;
}

/*
 *  ComputeMaxError :
 *	Find the maximum squared distance of digitized points
 *	to fitted curve.
*/
static double ComputeMaxError (QPointF *d, int first, int last, BezierCurve bezCurve, double *u, int *splitPoint)
{
    int		i;
    double	maxDist;		/*  Maximum error		*/
    double	dist;		/*  Current error		*/
    QPointF	P;			/*  Point on curve		*/
    QPointF	v;			/*  Vector from point to curve	*/

    *splitPoint = (last - first + 1)/2;
    maxDist = 0.0;
    for (i = first + 1; i < last; i++)
      {
		    P = BezierII(3, bezCurve, u[i-first]);
		    v = P - d[i];
		    dist = geom::norm2 (v);
		    if (dist >= maxDist)
          {
	    	      maxDist = dist;
	    	      *splitPoint = i;
		      }
      }
    return (maxDist);
}

/*
 *  NewtonRaphsonRootFind :
 *	Use Newton-Raphson iteration to find better root.
 */
static double NewtonRaphsonRootFind(BezierCurve Q, QPointF P, double u)
{
    double 		numerator, denominator;
    QPointF 		Q1[3], Q2[2];	/*  Q' and Q''			*/
    QPointF		Q_u, Q1_u, Q2_u; /*u evaluated at Q, Q', & Q''	*/
    double 		uPrime;		/*  Improved u			*/
    int 		i;
    
    /* Compute Q(u)	*/
    Q_u = BezierII(3, Q, u);
    
    /* Generate control vertices for Q'	*/
    for (i = 0; i <= 2; i++)
		  Q1[i] = (Q[i+1] - Q[i]) * 3.0;
    
    /* Generate control vertices for Q'' */
    for (i = 0; i <= 1; i++)
		  Q2[i] = (Q1[i+1] - Q1[i]) * 2.0;
    
    /* Compute Q'(u) and Q''(u)	*/
    Q1_u = BezierII(2, Q1, u);
    Q2_u = BezierII(1, Q2, u);
    
    /* Compute f(u)/f'(u) */
    numerator = geom::dot (Q_u - P, Q1_u);
    denominator = geom::norm2 (Q1_u) + geom::norm2 (Q_u - P);
    
    /* u = u - f(u)/f'(u) */
    uPrime = u - (numerator/denominator);
    return (uPrime);
}

/*
 *  Reparameterize:
 *	Given set of points and their parameterization, try to find
 *   a better parameterization.
 *
 */
static double *Reparameterize (QPointF *d, int first, int last, double *u, BezierCurve bezCurve)
{
    int 	nPts = last-first+1;	
    int 	i;
    double	*uPrime;		/*  New parameter values	*/

    uPrime = (double *)malloc(nPts * sizeof(double));
    for (i = first; i <= last; i++)
		  uPrime[i-first] = NewtonRaphsonRootFind(bezCurve, d[i], u[i-first]);

    return (uPrime);
}

/*
 *  ChordLengthParameterize :
 *	Assign parameter values to digitized points 
 *	using relative distances between points.
 */
static double *ChordLengthParameterize (QPointF *d, int first, int last)
{
    int		i;	
    double	*u;			/*  Parameterization		*/

    u = (double *)malloc((unsigned)(last-first+1) * sizeof(double));

    u[0] = 0.0;
    for (i = first+1; i <= last; i++)
		  u[i-first] = u[i-first-1] + geom::distance (d[i], d[i-1]);

    for (i = first + 1; i <= last; i++)
		  u[i-first] = u[i-first] / u[last-first];

    return(u);
}

/*
 *  FitCubic :
 *  	Fit a Bezier curve to a (sub)set of digitized points
 */
void path_approximation::FitCubic (QPointF *d, int first, int last, QPointF tHat1, QPointF tHat2, double error)
{
    QPointF	bezCurve[4]; /*Control points of fitted Bezier curve*/
    double	*u;		/*  Parameter values for point  */
    double	*uPrime;	/*  Improved parameter values */
    double	maxError;	/*  Maximum fitting error	 */
    int		splitPoint;	/*  Point to split point set at	 */
    int		nPts;		/*  Number of points in subset  */
    double	iterationError; /*Error below which you try iterating  */
    int		maxIterations = 4; /*  Max times to try iterating  */
    QPointF	tHatCenter;   	/* Unit tangent vector at splitPoint */
    int		i;		

    iterationError = error * error;
    nPts = last - first + 1;

    /*  Use heuristic if region only has two points in it */
    if (nPts == 2) 
      {
	      double dist = geom::distance (d[last], d[first]) / 3.0;
		    bezCurve[0] = d[first];
		    bezCurve[3] = d[last];
        bezCurve[1] = bezCurve[0] + tHat1 * dist;
        bezCurve[2] = bezCurve[3] + tHat2 * dist;
		    output_curve (bezCurve);
		    return;
      }

    /*  Parameterize points, and attempt to fit curve */
    u = ChordLengthParameterize(d, first, last);
    GenerateBezier(d, first, last, u, tHat1, tHat2, bezCurve);

    /*  Find max deviation of points to fitted curve */
    maxError = ComputeMaxError(d, first, last, bezCurve, u, &splitPoint);
    if (maxError < error)
      {
		    output_curve (bezCurve);
		    free((void *)u);
		    return;
      }


    /*  If error not too large, try some reparameterization  */
    /*  and iteration */
    if (maxError < iterationError)
      {
		    for (i = 0; i < maxIterations; i++)
          {
	    	    uPrime = Reparameterize(d, first, last, u, bezCurve);
	    	    GenerateBezier(d, first, last, uPrime, tHat1, tHat2, bezCurve);
	    	    maxError = ComputeMaxError(d, first, last,
				            bezCurve, uPrime, &splitPoint);
	    	    if (maxError < error)
              {
			          output_curve (bezCurve);
			          free((void *)u);
			          return;
	            }
	          free((void *)u);
	          u = uPrime;
	        }
      }

    /* Fitting failed -- split at max error point and fit recursively */
    free((void *)u);
    tHatCenter = ComputeCenterTangent(d, splitPoint);
    FitCubic(d, first, splitPoint, tHat1, tHatCenter, error);
    tHatCenter = -tHatCenter;
    FitCubic(d, splitPoint, last, tHatCenter, tHat2, error);
}


void path_approximation::approximate (std::vector<QPointF> points, double possible_error /*= 4.0*/)
{
  points.erase (std::unique (points.begin (), points.end ()), points.end ());
  QPointF	tHat1, tHat2;	/*  Unit tangent vectors at endpoints */

  tHat1 = ComputeLeftTangent (points.data (), 0);
  tHat2 = ComputeRightTangent (points.data (), (int)points.size () - 1);

  FitCubic (points.data (), 0, (int)points.size () - 1, tHat1, tHat2, possible_error);
}

void path_approximation::output_curve (const QPointF *data)
{
  if (m_builder->empty ())
    m_builder->move_to (data[0], false);

  m_builder->curve_to (data[3], data[1], data[2], false);
}

path_approximation::path_approximation (svg_path &path)
{
  put_in (m_builder, path);
}

path_approximation::~path_approximation ()
{

}
