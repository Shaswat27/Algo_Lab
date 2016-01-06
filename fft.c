#include <stdio.h>
#include <math.h>

#define PI 3.141592

typedef struct c
{
	double re;
	double im;
} complex;

void fft(int n, complex A[], complex F[])
{
	int k, j;
	complex omegak;
	if (n==1) { F[0].re = A[0].re; 
		        F[0].im = A[0].im;
		        return ;
		      }
	complex E[100], O[100], EF[100], OF[100];
		
	for (j=k=0; k<n; j++,k=k+2)
	{
		E[j].re = A[k].re;
		E[j].im = A[k].im;

		O[j].re = A[k+1].re;
		O[j].im = A[k+1].im;

	}
	
	
	
	fft(n/2, E, EF);
	fft(n/2, O, OF);
	
	complex omega;
	omega.re = cos(2*PI/n);
	omega.im = sin(2*PI/n);
	omegak.re = 1.0;//omega.re;
	omegak.im = 0.0;//omega.im;
	
	int sft = n/2;
	
	for (k=0; k<n/2; k++)
	{
		F[k].re = EF[k].re + (omegak.re)*(OF[k].re) - (omegak.im)*(OF[k].im);
		F[k].im = EF[k].im + (omegak.re)*(OF[k].im) + (omegak.im)*(OF[k].re);
		
		//printf("F[%d] = %lf + %lf i \n",k, F[k].re,F[k].im);
		
		F[k+sft].re = EF[k].re - (omegak.re)*(OF[k].re) + (omegak.im)*(OF[k].im);
		F[k+sft].im = EF[k].im - (omegak.re)*(OF[k].im) - (omegak.im)*(OF[k].re);

		//printf("n=%d\nomegak[%d] = %lf + %lf i \n",n,k, omegak.re,omegak.im);
		//printf("omega = %lf + %lf i \n",omega.re,omega.im);

		complex temp;
		temp.re = omegak.re;
		temp.im = omegak.im;

		omegak.re = temp.re*omega.re - temp.im*omega.im;
		double c = (temp.re)*(omega.im) + (temp.im)*(omega.re);
		omegak.im = c;
		//if(k==0 && n==4)
		//printf("c = %lf", c);
	}
}
		
void main()
{
	complex A[4];
	int n = 4;
	
	A[0].re = 1.0;
	A[0].im = 0.0;

	A[1].re = 2.0;
	A[1].im = 0.0;
	
	A[2].re = 3.0;
	A[2].im = 0.0;
	
	A[3].re = 4.0;
	A[3].im = 0.0;
	
	complex F[4];
	
	fft(n, A, F);
	int i = 0;
	for (i=0; i<4; i++)
	 printf("%f + %fi \n",F[i].re,F[i].im);

}



