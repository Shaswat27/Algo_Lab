#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.141592

//defining a complex number
typedef struct c
{
	double re;
	double im;
} complex;

//function to calculate fast fourier transform, n = 2^d
void fft(int n, complex *A, complex *F)
{
	int k, j;
	complex omegak;
	if (n==1) { F[0].re = A[0].re; 
		        F[0].im = A[0].im;
		        return ;
		      }

	complex *E = (complex *)(malloc((n/2)*sizeof(complex)));
	complex *O = (complex *)(malloc((n/2)*sizeof(complex)));
	complex *EF = (complex *)(malloc((n/2)*sizeof(complex)));
	complex *OF = (complex *)(malloc((n/2)*sizeof(complex)));
	
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

//function to calculate inverse fast fourier tranform, n = 2^d 
void ifft(int n, complex *F, complex *B)
{
	int k, j;
	complex omegak;
	if (n==1) { B[0].re = F[0].re; 
		        B[0].im = F[0].im;
		        return ;
		      }

	complex *E = (complex *)(malloc((n/2)*sizeof(complex)));
	complex *O = (complex *)(malloc((n/2)*sizeof(complex)));
	complex *EF = (complex *)(malloc((n/2)*sizeof(complex)));
	complex *OF = (complex *)(malloc((n/2)*sizeof(complex)));
		
	for (j=k=0; k<n; j++,k=k+2)
	{
		E[j].re = F[k].re;
		E[j].im = F[k].im;

		O[j].re = F[k+1].re;
		O[j].im = F[k+1].im;

	}
	
	
	
	ifft(n/2, E, EF);
	ifft(n/2, O, OF);
	
	complex omega;
	omega.re = cos(-2*PI/n);
	omega.im = sin(-2*PI/n);
	omegak.re = 1.0;//omega.re;
	omegak.im = 0.0;//omega.im;
	
	int sft = n/2;
	
	for (k=0; k<n/2; k++)
	{
		B[k].re = ( EF[k].re + (omegak.re)*(OF[k].re) - (omegak.im)*(OF[k].im) );
		B[k].im = ( EF[k].im + (omegak.re)*(OF[k].im) + (omegak.im)*(OF[k].re) );
		
		//printf("F[%d] = %lf + %lf i \n",k, F[k].re,F[k].im);
		
		B[k+sft].re = ( EF[k].re - (omegak.re)*(OF[k].re) + (omegak.im)*(OF[k].im) );
		B[k+sft].im = ( EF[k].im - (omegak.re)*(OF[k].im) - (omegak.im)*(OF[k].re) );

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
	int n=3, m=3, i=0; //degree 2, degree 2
	
	double temp = m+n; // final degree = m+n-2
	double d = log(temp)/log(2.0);
	int tmp =  (int)(d)+1;
	double p = pow(2.0,tmp);
	int size = (int)(p); // size s.t. size = 2^d and size>m+n

	complex *N = (complex *)(malloc(size*sizeof(complex)));
	complex *M = (complex *)(malloc(size*sizeof(complex)));
	complex *C = (complex *)(malloc(size*sizeof(complex)));

	for(i=0; i<size; i++)
	{
		N[i].re = 0.0;
		N[i].im = 0.0;

		M[i].re = 0.0;
		M[i].im = 0.0;
	}

	N[0].re = 9.0;
	N[1].re = 9.0;
	N[2].re = 9.0;

	M[0].re = 9.0;
	M[1].re = 9.0;
	M[2].re = 9.0;

	complex *FN = (complex *)(malloc(size*sizeof(complex))); //to store the fourier transform of N
	complex *FM = (complex *)(malloc(size*sizeof(complex))); //to store the fourier transform of M
	complex *FC = (complex *)(malloc(size*sizeof(complex))); //to store the fourier transform of product

	fft(size, N, FN);
	fft(size, M, FM);

	for (i=0;i<size;i++)
	{
		FC[i].re = FN[i].re*FM[i].re - FN[i].im*FM[i].im;
		FC[i].im = FN[i].re*FM[i].im + FN[i].im*FM[i].re;
	}

	ifft(size, FC, C);	
	
	for(i=0; i<size; i++)
	{
		C[i].re /= size;
		C[i].im /= size;
	}

	/*
	printf("IFFT of FC: \n");
	for (i=0; i<size; i++)
	 printf("%f + %fi \n",C[i].re,C[i].im);*/

	//Now create the number
	for(i=0; i<size-1; i++)
	{
		int temp = (int)(round(C[i].re));
		int carry = temp/10;
		int digit = temp%10;
		C[i].re = digit;
		C[i+1].re += carry;
	}

	//display the product
	int flag = 0;
	printf("Product: \n");
	for (i=size-1; i>=0; i--)
	{	
		if( (int)(C[i].re) != 0 ) flag = 1;
		if (flag)
			printf("%d",(int)(C[i].re));
	}
	printf("\n");

}