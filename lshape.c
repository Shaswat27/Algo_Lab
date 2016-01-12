#include <stdio.h>
#include <stdlib.h>


void fill(int **A, int r, int c, int r1, int c1, int n, int k) //board, row of missing, column of missing, start row, start col, size, k
{
	
	//printf("\n%d, %d, %d, %d\n", r, c, r1, c1);
	int i, j;
	//stopping condition
	if(n==2)
	{
		//printf("%d,%d,%d,%d\n",r1,c1,r,c);		
		for(i=0;i<2;i++)
		{
			for(j=0;j<2;j++)
			{
			if( (r1+i!=r || c1+j!=c)&&(A[r1+i][c1+j])<1) A[r1+i][c1+j]=k;
			}
		}
		return;
	}
	
	int flag = 0;
	//fill the middle block
	if(r-r1>=n/2 && c-c1>=n/2)
	{
	if(A[n/2+r1][n/2+c1]!=-1)
	{
	  A[n/2+r1][n/2+c1]=-1;
	  flag = 1;
	}	
	//r = n/2;
	//c = n/2;	
	fill(A, r1+n/2, c1+n/2, r1+n/2-1, c1+n/2-1, 2, k); 
	if(flag) A[n/2+r1][n/2+c1]=0;

	//now divide into four blocks and do the same
	fill(A, r, c, r1+n/2, c1+n/2, n/2, ++k);
	fill(A, r1+n/2, c1+n/2-1, r1+n/2, c1, n/2, ++k);
	fill(A, r1+n/2-1, c1+n/2-1, r1, c1, n/2, ++k);
	fill(A, r1+n/2-1, c1+n/2, r1, c1+n/2, n/2, ++k);
	}

	else if(r-r1>=n/2 && c-c1<n/2)
	{
	if(A[n/2+r1][n/2-1+c1]!=-1)
	{  
  	  A[n/2+r1][n/2-1+c1]=-1;
	  flag = 1;
	}
	//r = n/2;
	//c = n/2-1;	
	k+=5;
	fill(A, r1+n/2, c1+n/2-1, r1+n/2-1, c1+n/2-1, 2, k); 
	if(flag) A[n/2+r1][n/2-1+c1]=0;

	//now divide into four blocks and do the same
	fill(A, r1+n/2, c1+n/2, r1+n/2, c1+n/2, n/2, ++k);
	fill(A, r, c, r1+n/2, c1, n/2, ++k);
	fill(A, r1+n/2-1, c1+n/2-1, r1, c1, n/2, ++k);
	fill(A, r1+n/2-1, c1+n/2, r1, c1+n/2, n/2, ++k);
	}
		
	else if(r-r1<n/2 && c-c1<n/2)
	{
	if(A[n/2-1+r1][n/2-1+c1]!=-1)
	{
	  A[n/2-1+r1][n/2-1+c1]=-1;
   	  flag = 1;
	}
	//r = n/2-1;
	//c = n/2-1;	
	k+=10;
	fill(A, r1+n/2-1, c1+n/2-1, r1+n/2-1, c1+n/2-1, 2, k);
	if(flag) A[n/2-1+r1][n/2-1+c1]=0; 

	//now divide into four blocks and do the same
	fill(A, r1+n/2, c1+n/2, r1+n/2, c1+n/2, n/2, ++k);
	fill(A, r1+n/2, c1+n/2-1, r1+n/2, c1, n/2, ++k);
	fill(A, r, c, r1, c1, n/2, ++k);
	fill(A, r1+n/2-1, c1+n/2, r1, c1+n/2, n/2, ++k);
	}

	else
	{
	if(A[n/2-1+r1][n/2+c1]!=-1)
	{
 	  A[n/2-1+r1][n/2+c1]=-1;
	  flag = 1;
	}
	//r = n/2-1;
	//c = n/2-1;
	k+=15;
	fill(A, r1+n/2-1, c1+n/2, r1+n/2-1, c1+n/2-1, 2, k);
	if(flag) A[n/2-1+r1][n/2+c1]=0; 

	//now divide into four blocks and do the same
	fill(A, r1+n/2, c1+n/2, r1+n/2, c1+n/2, n/2, ++k);
	fill(A, r1+n/2, c1+n/2-1, r1+n/2, c1, n/2, ++k);
	fill(A, r1+n/2-1, c1+n/2-1, r1, c1, n/2, ++k);
	fill(A, r, c, r1, c1+n/2, n/2, ++k);
	}

	return;
}


int main()
{
	int n, r, c; //size of the board, row of the blank tile, column of the blank tile
	
	//enter data	
	printf("Enter the size of the board (must be a power of 2): ");
	scanf("%d", &n);
	printf("\nEnter the row number of missing tile (1,2,...,n): ");
	scanf("%d", &r);
	r--;
	printf("\nEnter the column number of missing tile (1,2,...,n): ");
	scanf("%d", &c);
	c--;

	//create the board: missing location->(-1), rest->0
	int **A = (int **)malloc(n*sizeof(int *));
	int i=0, j=0;
	for (;i<n;i++)
		*(A+i) = (int *)malloc(n*sizeof(int));
	for (i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(i==r && j==c) A[i][j]=-1;
			else A[i][j]=0;
		}
	}

for (i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%d\t",A[i][j]);
		}
	printf("\n");
	}
	
	//recursively apply the D&C algorithm
	fill(A, r, c, 0, 0, n, 1);
	
	printf("\n");
	
	for (i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%d\t",A[i][j]);
		}
printf("\n");
	}
	
	return 0;
}
	

	


