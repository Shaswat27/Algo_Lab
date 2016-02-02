#include <stdio.h>

void survive(int n)
{
	static int a = 1; // index_original = a*index_new+b
	static int b = 0;
	if(n==1)
	{
		printf("\nSurvivor: %d\n",a*1+b);
		return;
	}
	
	int i=2;
	for(;i<=n;i=i+2)
	{
		printf("%d ",a*i+b);
	}
	if(n%2!=0) printf("%d ",a*1+b);	
	
	if(n%2==0) {b=-a+b ; a=a*2; }
	else { b=a+b ;a=a*2;}
	
	survive(n/2);
}

int main()
{
	int x;
	printf("Enter the number of people: ");
	scanf("%d",&x); 
	
	survive(x);
	
	//verify by formula
	int check = 1, j=0;
	for(j=0; check<=x; j++)
		check*=2;
	int m = j-1, l = x-check/2;
	printf("%d = 2^%d+%d\n",x,m,l);
	printf("Since V(2^m+l) = 2l+1, survivor (by formula) = %d\n",2*l+1);
	
	return 0;
}
