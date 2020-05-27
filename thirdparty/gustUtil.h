#ifndef util_h
#define util_h

#define C_PI 3.14159265358979323846264338327950288419716939937511
#define C_E 2.71828182845904523536028747135266249775724709369995
#define C_SOL 299792458.0 //speed of light in m/s
#define C_DEG2RAD 0.017453292519943295
#define C_RAD2DEG 57.295779513082323
#define C_RPM2RPS 0.10471975511966
#define C_RPS2RPM 9.54929658551372

#define C_FPS2KPH 1.09728
#define C_KPH2FPS 0.911344415281423
#define C_FPS2KT 0.592483801296
#define C_KT2FPS 1.6878098571
#define C_FPS2MPH 0.681818181818182
#define C_MPH2FPS 1.466666666666667
#define C_FT2NM 0.000164578833693
#define C_NM2FT 6076.11548556
#define C_FT2M 0.3048
#define C_FT2KM 0.0003048
#define C_KM2FT 3280.83989501312
#define C_M2FT 3.280839895
#define C_M2CM 100
#define C_CM2M 0.01
#define C_M2MM 1000
#define C_MM2M 0.001
#define C_FT2SM (1 / 5280)
#define C_SM2FT 5280
#define C_USGALLON2OUNCES 128
#define C_PA2PSI 0.000145037738
#define C_PSI2PA 6894.75729
#define C_BAR2PA 100000

#define C_KG2SLUG 0.0685217659
#define C_SLUG2KG 14.5939029
#define C_KGM2_TO_SLUGFT2 0.7375615694452221

#define C_KT2KPH (C_KT2FPS * C_FPS2KPH)
#define C_KT2MPH (C_KT2FPS * C_FPS2MPH)
#define C_KT2MPS (C_KT2FPS * C_FT2M)

#define CF_PI 3.14159265359f
#define CF_E 2.71828182846f
#define CF_DEG2RAD 0.017453292519943295f
#define CF_RAD2DEG 57.295779513082323f

#define CF_FPS2KT 0.592483801296f
#define CF_KT2FPS 1.6878098571f
#define CF_FT2NM 0.000164578833693f
#define CF_NM2FT 6076.11548556f
#define CF_FT2M 0.3048f
#define CF_M2FT 3.280839895f

#ifndef MIN
#define MIN(x1, x2) ((x1) < (x2) ? (x1) : (x2))
#endif
#ifndef MAX
#define MAX(x1, x2) ((x1) > (x2) ? (x1) : (x2))
#endif
#define LIMIT(x, xl, xu) ((x) >= (xu) ? (xu) : ((x) < (xl) ? (xl) : (x)))
#define ABS_GUST(x) ((x) < 0.0 ? (-(x)) : (x))
#define SQ(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define SIGN(x) ((x) < (0) ? (-1) : ((x) > (0) ? (1) : (0)))
#define INRANGE(x, x1, x2) ((x) <= (x2) && (x) >= (x1) ? 1 : 0)

// treats given pointer to array as an n-dimensional array and gets the (i,j,k,l) element
// given macros are for 1,2,3,4 arrays
#define GET1D(MAT, N0, I) *((double *)(MAT) + (I))
#define GET2D(MAT, N0, N1, I, J) *((double *)(MAT) + (I) * (N1) + (J))
#define GET3D(MAT, N0, N1, N2, I, J, K) *((double *)(MAT) + (I) * (N1) * (N2) + (J) * (N2) + (K))
#define GET4D(MAT, N0, N1, N2, N3, I, J, K, L) *((double *)(MAT) + (I) * (N1) * (N2) * (N3) + (J) * (N2) * (N3) + (K) * (N3) + (L))

// same macro just different name
#define SET1D(MAT, N0, I) *((double *)(MAT) + (I))
#define SET2D(MAT, N0, N1, I, J) *((double *)(MAT) + (I) * (N1) + (J))
#define SET3D(MAT, N0, N1, N2, I, J, K) *((double *)(MAT) + (I) * (N1) * (N2) + (J) * (N2) + (K))
#define SET4D(MAT, N0, N1, N2, N3, I, J, K, L) *((double *)(MAT) + (I) * (N1) * (N2) * (N3) + (J) * (N2) * (N3) + (K) * (N3) + (L))

#define FILE_READ_INT(FILE, VAR, NMEMB) fread((VAR), 4, (NMEMB), (FILE))
#define FILE_READ_DOUBLE(FILE, VAR, NMEMB) fread((VAR), 8, (NMEMB), (FILE))

// cross product
#define VCROSS0(A0, A1, A2, B0, B1, B2) ((A1) * (B2) - (A2) * (B1))
#define VCROSS1(A0, A1, A2, B0, B1, B2) ((A2) * (B0) - (A0) * (B2))
#define VCROSS2(A0, A1, A2, B0, B1, B2) ((A0) * (B1) - (A1) * (B0))

#include <math.h>

#if defined(__cplusplus)
extern "C"
{
#endif

	double hmodDeg(double h);
	double hmod360(double h);
	double hmodRad(double h);
	void quat2euler(double *q, double *phi, double *theta, double *psi);
	void build_rotmatrix(double m[3][3], double q[4]);
	void dcm2euler(double m[3][3],
				   double *phi, double *theta, double *psi);
				   
	/** bisection location */
	int locateBisect(double *xx, int n, double x);

	// table lookup routines
	double lookup1d(double *yy, double *xx, int n, double x);
	double lookup2d(double *yy,
					double *xx0, double *xx1,
					int n0, int n1,
					double x0, double x1);

	// Allows for a larger stored table, even though the breakpoints and actual
	//  data are smaller.
	double lookup2dFlexSize(double *yy,
							double *xx0, double *xx1,
							int n0, int n1,
							double x0, double x1,
							int n0Stored, int n1Stored);

	double lookup3d(double *yy,
					double *xx0, double *xx1, double *xx2,
					int n0, int n1, int n2,
					double x0, double x1, double x2);

	double lookup4d(double *yy,
					double *xx0, double *xx1, double *xx2, double *xx3,
					int n0, int n1, int n2, int n3,
					double x0, double x1, double x2, double x3);

	// vector version of lookup1d (useful for resampling data)
	// given a table xx, yy; samples points at x (of length nx)
	// and stores it in y
	void lookup1dv(double *yy, double *xx, int n, double *y, double *x, int nx);

	unsigned short endian_swap_ushort(unsigned short x);
	unsigned int endian_swap_uint(unsigned int x);

	// calculates the Hamming distance between two vectors of lenght given by bytes
	unsigned int calcHamming(const char vec1[], const char vec2[], int bytes);

#if defined(CUSTOM_ROUND_FUNCTION)
	double round(double);
#endif

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)

#endif // c++

#endif


double hmodDeg( double h ) {
		double dh;
		int i;

		if( h > 0 )
			i = (int)( h/360.0 + 0.5 );
		else
			i = (int)( h/360.0 - 0.5 );
		dh = h - 360.0*i;

		return dh;
}

void quat2euler(double *q, double *phi, double *theta, double *psi) {
	double dcm[3][3];	/* this is expensive, avoid using this fcn */

	build_rotmatrix(dcm,q);
	dcm2euler(dcm,phi,theta,psi);
}

/*
 * Build a rotation matrix, given a quaternion rotation.
 */
void build_rotmatrix( double m[3][3], double q[4] ) {

  // Products used more than once are broken out to (hopefully)
  // speed up the code and, most importantly, to work around a
  // bug in the TI CodeComposer C6000 compiler, v7.2)
  double q0_x_q1 = q[0] * q[1];
  double q0_x_q2 = q[0] * q[2];
  double q0_x_q3 = q[0] * q[3];
  double q1_x_q1 = q[1] * q[1];
  double q1_x_q2 = q[1] * q[2];
  double q1_x_q3 = q[1] * q[3];
  double q2_x_q2 = q[2] * q[2];
  double q2_x_q3 = q[2] * q[3];
  double q3_x_q3 = q[3] * q[3];
  
  m[0][0] = 1.0 - 2.0*( q2_x_q2 + q3_x_q3 );
  m[0][1] =       2.0*( q1_x_q2 - q0_x_q3 );
  m[0][2] =       2.0*( q1_x_q3 + q0_x_q2 );

  m[1][0] =       2.0*( q1_x_q2 + q0_x_q3 );
  m[1][1] = 1.0 - 2.0*( q1_x_q1 + q3_x_q3 );
  m[1][2] =       2.0*( q2_x_q3 - q0_x_q1 );

  m[2][0] =       2.0*( q1_x_q3 - q0_x_q2 );
  m[2][1] =       2.0*( q2_x_q3 + q0_x_q1 );
  m[2][2] = 1.0 - 2.0*( q1_x_q1 + q2_x_q2 );

}

void dcm2euler( double m[3][3],
		double *phi, double *theta, double *psi ) {

  /* this needs to be checked */

  if( ABS_GUST( m[2][0] ) < 0.9999 ) {
    *phi   = atan2( m[2][1], m[2][2] );
    *theta = -asin( LIMIT( m[2][0], -1.0, 1.0 ) );
    *psi   = atan2( m[1][0], m[0][0] );
  } else {
    *phi   = 0.0;
    if( m[2][0] < 0.0 ) {
      *theta =  C_PI*0.5;
      *psi   = atan2( m[1][2], m[0][2] );
    } else {
      *theta = -C_PI*0.5;
      *psi   = atan2( -m[1][2], -m[0][2] );
    }
  }

}

// see numerical recipes section 3.4 for details
// it has been modified here to use 0 indexing, and use signed integers
// so that it returns -1 if under-range or n if above range
int locateBisect(double *xx, int n, double x) {
	int j,ju,jm,jl;
	int ascnd;

	jl = -1;
	ju = n;

	// check if it is ascending or descending
	ascnd =  xx[n-1] >= xx[0];

	while ( ju - jl > 1 ) {
		jm = (ju + jl) >> 1;
		if( (x > xx[jm]) == ascnd ) {
			jl = jm;
		}
		else {
			ju = jm;
		}
	}

	if (x == xx[0]) {
		j = 0;
	}
	else if(x == xx[n-1]) {
		j = n-2;
	}
	else {
		j = jl;
	}
	return j;

}

// yy is the table, xx is list of breakpoints, n is nubmer of breakpoints, x is independent var
double lookup1d(double *yy, double *xx, int n, double x) {
	int i;
	double p;

	i = MIN( MAX( locateBisect(xx, n, x), 0) , n-2);

	p = ( x - xx[i] ) / ( xx[i+1] - xx[i] );

	return (1-p)*yy[i] + p*yy[i+1];

}

// yy is the table, xx0,xx1 is list of breakpoints, n0,n1 is nubmer of breakpoints, x0,x1 is independent var
double lookup2d(double *yy,
				double *xx0, double *xx1,
				int    n0,   int    n1,
				double x0,   double x1) {
	int    i0,i1;
	double p0,p1;

	i0 = MIN( MAX( locateBisect(xx0, n0, x0), 0) , n0-2);
	i1 = MIN( MAX( locateBisect(xx1, n1, x1), 0) , n1-2);

	p0 = (x0 - xx0[i0]) / (xx0[i0+1] - xx0[i0]);
	p1 = (x1 - xx1[i1]) / (xx1[i1+1] - xx1[i1]);

	return  (1-p0)*(1-p1)*GET2D(yy, n0, n1, i0    , i1    )
		  + (1-p0)*(  p1)*GET2D(yy, n0, n1, i0    , i1 + 1)
		  + (  p0)*(1-p1)*GET2D(yy, n0, n1, i0 + 1, i1    )
		  + (  p0)*(  p1)*GET2D(yy, n0, n1, i0 + 1, i1 + 1);
}

// yy is the table, xx0,xx1 is list of breakpoints, n0,n1 is nubmer of breakpoints, x0,x1 is independent var
// Allows for a larger 2D table than the number of breakpoints, such that a larger table can be defined while
//  limiting it to flexible data available.
double lookup2dFlexSize(double *yy,
						double *xx0, double *xx1,
						int    n0,   int    n1,
						double x0,   double x1,
						__attribute__((unused)) int n0Stored, int n1Stored) {
	int    i0,i1;
	double p0,p1;

	i0 = MIN( MAX( locateBisect(xx0, n0, x0), 0) , n0-2);
	i1 = MIN( MAX( locateBisect(xx1, n1, x1), 0) , n1-2);

	p0 = (x0 - xx0[i0]) / (xx0[i0+1] - xx0[i0]);
	p1 = (x1 - xx1[i1]) / (xx1[i1+1] - xx1[i1]);

	return  (1-p0)*(1-p1)*GET2D(yy, n0Stored, n1Stored, i0    , i1    )
		  + (1-p0)*(  p1)*GET2D(yy, n0Stored, n1Stored, i0    , i1 + 1)
		  + (  p0)*(1-p1)*GET2D(yy, n0Stored, n1Stored, i0 + 1, i1    )
		  + (  p0)*(  p1)*GET2D(yy, n0Stored, n1Stored, i0 + 1, i1 + 1);
}

double lookup3d(double *yy,
				double *xx0, double *xx1, double *xx2,
				int    n0,   int    n1,   int n2,
				double x0,   double x1,   double x2) {
	int    i0,i1,i2;
	double p0,p1,p2;


	i0 = MIN( MAX( locateBisect(xx0, n0, x0), 0) , n0-2);
	i1 = MIN( MAX( locateBisect(xx1, n1, x1), 0) , n1-2);
	i2 = MIN( MAX( locateBisect(xx2, n2, x2), 0) , n2-2);

	p0 = (x0 - xx0[i0]) / (xx0[i0+1] - xx0[i0]);
	p1 = (x1 - xx1[i1]) / (xx1[i1+1] - xx1[i1]);
	p2 = (x2 - xx2[i2]) / (xx2[i2+1] - xx2[i2]);

	return (1-p0)*(1-p1)*(1-p2)*GET3D(yy, n0, n1, n2, i0 + 0, i1 + 0, i2 + 0)
		 + (1-p0)*(1-p1)*(  p2)*GET3D(yy, n0, n1, n2, i0 + 0, i1 + 0, i2 + 1)
		 + (1-p0)*(  p1)*(1-p2)*GET3D(yy, n0, n1, n2, i0 + 0, i1 + 1, i2 + 0)
		 + (1-p0)*(  p1)*(  p2)*GET3D(yy, n0, n1, n2, i0 + 0, i1 + 1, i2 + 1)

		 + (  p0)*(1-p1)*(1-p2)*GET3D(yy, n0, n1, n2, i0 + 1, i1 + 0, i2 + 0)
		 + (  p0)*(1-p1)*(  p2)*GET3D(yy, n0, n1, n2, i0 + 1, i1 + 0, i2 + 1)
		 + (  p0)*(  p1)*(1-p2)*GET3D(yy, n0, n1, n2, i0 + 1, i1 + 1, i2 + 0)
		 + (  p0)*(  p1)*(  p2)*GET3D(yy, n0, n1, n2, i0 + 1, i1 + 1, i2 + 1);

}

double lookup4d(double *yy,
				double *xx0, double *xx1, double *xx2, double *xx3,
				int    n0,   int    n1,   int n2, int n3,
				double x0,   double x1,   double x2, double x3) {
	int    i0,i1,i2,i3;
	double p0,p1,p2,p3;

	i0 = MIN( MAX( locateBisect(xx0, n0, x0), 0) , n0-2);
	i1 = MIN( MAX( locateBisect(xx1, n1, x1), 0) , n1-2);
	i2 = MIN( MAX( locateBisect(xx2, n2, x2), 0) , n2-2);
	i3 = MIN( MAX( locateBisect(xx3, n3, x3), 0) , n3-2);

	p0 = (x0 - xx0[i0]) / (xx0[i0+1] - xx0[i0]);
	p1 = (x1 - xx1[i1]) / (xx1[i1+1] - xx1[i1]);
	p2 = (x2 - xx2[i2]) / (xx2[i2+1] - xx2[i2]);
	p3 = (x3 - xx3[i3]) / (xx3[i3+1] - xx3[i3]);

	return
		   (1-p0)*(1-p1)*(1-p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 0, i2 + 0, i3 + 0)
         + (1-p0)*(1-p1)*(1-p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 0, i2 + 0, i3 + 1)
         + (1-p0)*(1-p1)*(  p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 0, i2 + 1, i3 + 0)
		 + (1-p0)*(1-p1)*(  p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 0, i2 + 1, i3 + 1)

		 + (1-p0)*(  p1)*(1-p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 1, i2 + 0, i3 + 0)
         + (1-p0)*(  p1)*(1-p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 1, i2 + 0, i3 + 1)
         + (1-p0)*(  p1)*(  p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 1, i2 + 1, i3 + 0)
		 + (1-p0)*(  p1)*(  p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 0, i1 + 1, i2 + 1, i3 + 1)

		 + (  p0)*(1-p1)*(1-p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 0, i2 + 0, i3 + 0)
         + (  p0)*(1-p1)*(1-p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 0, i2 + 0, i3 + 1)
         + (  p0)*(1-p1)*(  p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 0, i2 + 1, i3 + 0)
		 + (  p0)*(1-p1)*(  p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 0, i2 + 1, i3 + 1)

		 + (  p0)*(  p1)*(1-p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 1, i2 + 0, i3 + 0)
         + (  p0)*(  p1)*(1-p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 1, i2 + 0, i3 + 1)
         + (  p0)*(  p1)*(  p2)*(1-p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 1, i2 + 1, i3 + 0)
		 + (  p0)*(  p1)*(  p2)*(  p3)*GET4D(yy, n0, n1, n2, n3, i0 + 1, i1 + 1, i2 + 1, i3 + 1) ;

}



// vector version of lookup1d (useful for resampling data)
// given a table xx, yy samples points at x (of length nx)
// and stores it in y
void lookup1dv(double *yy, double *xx, int n, double *y, double *x, int nx) {
	int i;
	for(i = 0; i < nx; i++) {
		y[i] = lookup1d(yy,xx,n,x[i]);
	}
}


unsigned short endian_swap_ushort(unsigned short x)
{
	x = (x>>8) | 
		(x<<8);
	return x;
}

unsigned int endian_swap_uint(unsigned int x)
{
	x = (x>>24) | 
		((x<<8) & 0x00FF0000) |
		((x>>8) & 0x0000FF00) |
		(x<<24);

	return x;
}

unsigned int calcHamming( const char vec1[], const char vec2[], int bytes ) {
    
    unsigned int bitsum = 0;
    int k;
    
    for (k=0; k < bytes; k++){
        unsigned char xored = (vec1[k]^vec2[k]);
        for( ; xored; xored>>=1){
            bitsum += xored & 1;
        }
    }
    
    return bitsum;
}

#if defined(CUSTOM_ROUND_FUNCTION)
#include <math.h>
double round(double x) {
	return floor(x + 0.5);
}
#endif