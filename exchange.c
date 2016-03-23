#include <stdio.h>
#include <math.h>

#define MAX 1000

double table[30][30];

int bellmanFord(double w[][30], int n)
{
	double d[30];
	int i = 0, j = 0, itr = 0;
	for(i=0; i<n; i++)
		d[i] = MAX;
	d[0] = 0;

	for(itr=0; itr<n; itr++) //for itr=1 -> itr=|v|-1
	{
		for(i = 0; i<n; i++)
		{
			for(j = 0; j<n; j++)
			{
				if(d[j] > d[i] - log(w[i][j]/100.0))
					d[j] = d[i] - log(w[i][j]/100.0);
			}
		}
	}

	for(i = 0; i<n; i++)
	{
		for(j = 0; j<n; j++)
		{
			if(d[j] > d[i] - log(w[i][j]/100.0))
				return 1;
		}
	}

	return 0;
}

void main()
{
	int n;
	printf("Enter the number of currrencies: ");
	scanf("%d", &n);

	int i = 0, j = 0;
	printf("\nEnter conversion rates: \n");
	for(i = 0; i<n; i++)
	{
		for(j = 0; j<n; j++)
		{
			printf("Conversion rate from currency %d to %d -> ", i+1, j+1);
			scanf("%lf", &table[i][j]);
		}
	}

	int check = bellmanFord(table, n);

	(check==1)?(printf("\nOpportunity exists!\n")):(printf("\nOpportunity doesn't exist.\n"));

}