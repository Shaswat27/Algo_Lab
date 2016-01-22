#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct b
{
	double l;
	double r;
	double h;
} building;

typedef struct r
{
	double x;
	double y;
} result;

result *merge(result *r1, result *r2, int n1, int n2, int *r_size)
{
	double h1 = 0.0, h2 = 0.0;
	
	result *r_final = (result *)malloc(2*(n1+n2)*sizeof(result));
		
	int i=0, j=0, k=0;
	
	while (i<n1 && j<n2)
    {
        //Compare x coordinates of left sides
        if (r1[i].x < r2[j].x)
        {
            h1 = r1[i].y;
            //Choose height as max of two heights
            double h = (h1>h2)?(h1):(h2);
            if(k>0) if(r_final[k-1].y == h) {i++;continue;}
            r_final[k].x = r1[i].x;
 	    r_final[k++].y = h;          
            i++;
        }
        else
        {
            h2 = r2[j].y;
            //Choose height as max of two heights
            double h = (h1>h2)?(h1):(h2);
	    if(k>0) if(r_final[k-1].y == h) {j++;continue;}
            r_final[k].x = r2[j].x;
            r_final[k++].y = h;
            j++;
        }
    }
 
    //If there are points left
    while (i < n1)
    {
        r_final[k++] = r1[i];
        i++;
    }
    while (j < n2)
    {
        r_final[k++] = r2[j];
        j++;
    }

    *r_size = k;    

    return r_final;
}
	
//maximum size of r can be 2n (all buildings are non-overlapping)
result *skyline(building *b, int start, int end, int *r_size) //parameters - array of buildings, start index of b, end index of b, array to store result, size of result
{
	if(start>=end) //stopping condition
	{
		result *r = (result *)malloc(2*sizeof(result));
		r[0].x = b[start].l;
		r[0].y = b[start].h;
		r[1].x = b[start].r;
		r[1].y = 0.0;
		*r_size = 2.0;
		
		/*printf("\nStopping:\n");
		int i=0;
		for(;i<(*r_size);i++)
		{
			printf("(%lf, %lf)\n",r[i].x, r[i].y);
		}*/
		
		return r;
	}
	
	int n = (end-start); //number of buildings - 1
	//printf("\nn=%d\n", n);
	result *r1 = (result *)malloc(2*n*sizeof(result));
	result *r2 = (result *)malloc(2*n*sizeof(result));
	 
	r1 = skyline(b, start, start + n/2, r_size);
	int n_r1 = *r_size;
	r2 = skyline(b, start + n/2 + 1, end, r_size);
	int n_r2 = *r_size; 
	result *r_final = (result *)malloc(2*n*sizeof(result));
	
	/*printf("\nMerge:\n");
	int i=0;
	for(;i<n_r1;i++)
	{
		printf("(%lf, %lf)\n",r1[i].x, r1[i].y);
	}
	printf("--------------------------------------------------------\n");
	for(i=0;i<n_r2;i++)
	{
		printf("(%lf, %lf)\n",r2[i].x, r2[i].y);
	}*/

        r_final = merge(r1,r2, n_r1, n_r2, r_size);
	
	return r_final;
}	

int main()
{
	srand(time(NULL));	
	double x_min, x_max, y_max;
	int n;
	printf("\nEnter the minimum x coordinate: \n");
	scanf("%lf", &x_min);
    printf("\nEnter the maximum x coordinate: \n");
	scanf("%lf", &x_max);
    printf("\nEnter the maximum y coordinate: \n");
	scanf("%lf", &y_max);
	printf("\nEnter the number of buildings: \n");
	scanf("%d", &n);
	
	building *b = (building *)malloc(n*sizeof(building));
	int r_size = 0;

	int i=0;
	for(;i<n;i++)
	{
		b[i].l = 1 + x_min + (rand())%((int)(x_max-x_min-1));
		b[i].r = b[i].l + 1 + (rand())%((int)(x_max-b[i].l-1));
		b[i].h = 1 + (rand())%((int)y_max);
	} 

	/*b[1].l = 1.0;
	b[1].r = 5.0;
	b[1].h = 11.0;
	
	b[0].l = 2.0;
	b[0].r = 7.0;
	b[0].h = 6.0;

	b[3].l = 3.0;
	b[3].r = 9.0;
	b[3].h = 13.0;
	
	b[2].l = 12.0;
	b[2].r = 16.0;
	b[2].h = 7.0;

    b[5].l = 14.0;
	b[5].r = 25.0;
	b[5].h = 3.0;
	
	b[4].l = 19.0;
	b[4].r = 22.0;
	b[4].h = 18.0;

	b[7].l = 23.0;
	b[7].r = 29.0;
	b[7].h = 13.0;
	
	b[6].l = 24.0;
	b[6].r = 28.0;
	b[6].h = 4.0;*/
	
	i=0;
	printf("\nThe buildings are (left x, right x, height): ");
	for(;i<n;i++)
	{
		printf("(%lf, %lf, %lf) ",b[i].l, b[i].r, b[i].h);
	}	
	
	result *r = skyline(b, 0, n-1, &r_size);
	
	i=0;
	printf("\nThe contour is (x,y): ");
	for(;i<r_size;i++)
	{
		printf("(%lf, %lf) ",r[i].x, r[i].y);
	}	
	printf("\n");
	return 0;
}

