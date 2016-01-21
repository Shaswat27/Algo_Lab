#include <stdio.h>
#include <stdlib.h>

typedef struct b
{
	double l1;
	double h1;
	double l2;
	double h2;
} building;

typedef struct r
{
	double x;
	double y;
} result;

int predict_y(result l1, result l2, result check) //return 1 if check can be inserted
{
	result predict;
	if(l1.x!=l2.x)	
	{
		predict.x = check.x;
		predict.y = ( (l2.y-l1.y)/(l2.x-l1.x) )*(predict.x-l1.x) + l1.y;
		if (predict.y>=check.y) return 0;
		else return 1;
	}
	else
	{
		double y_max = (l1.y>=l2.y)?l1.y:l2.y;
		double y_min = (l1.y<l2.y)?l1.y:l2.y;
		
		if(check.x<=l1.x) return 1;
		else if(check.y>=y_max && check.x>l1.x ) return 1;
		else return 0;
	}
}

result find_intersection(result p1, result p2, result q1, result q2)
{
	if(p1.x>p2.x)
	{
		result temp = p1;
		p1 = p2;
		p2 = temp;
	}
	
	if(q1.x>q2.x)
	{
		result temp = q1;
		q1 = q2;
		q2 = temp;
	}
	
	result r;
	//now p1,p2 and q1, q2 are x sorted
	if( (p1.x!=p2.x) && (q2.x!=q1.x) )
	{
    	double m1 = (p2.y - p1.y)/(p2.x - p1.x);
		double m2 = (q2.y - q1.y)/(q2.x - q1.x);

		
	
		if(m1 != m2)
		{
			r.x = ( (m1*p1.x - m2*q1.x)+(q1.y-p1.y) )/(m1-m2);
			r.y = m1*(r.x - p1.x) + p1.y;
		}
		else //no intersection
		{
			r.x = -1000;
			r.y = -1000;
		}
	}
	else if(p1.x==p2.x && q1.x!=q2.x)
	{
		double m2 = (q2.y - q1.y)/(q2.x - q1.x);

		//result r;
		
		r.x = p1.x;

		r.y = q1.y + m2*(r.x-q1.x);
	}
	else if(p1.x!=p2.x && q1.x==q2.x)
	{
		double m1 = (p2.y - p1.y)/(p2.x - p1.x);

		//result r;
		
		r.x = q1.x;

		r.y = p1.y + m1*(r.x-p1.x);
	}
	else {}
			
	double p_y_max = (p1.y>p2.y)?p1.y:p2.y;
	double q_y_max = (q1.y>q2.y)?q1.y:q2.y;

	double p_y_min = (p1.y<p2.y)?p1.y:p2.y;
	double q_y_min = (q1.y<q2.y)?q1.y:q2.y;

	//checking for validity of the intersection
	if( !(r.x>=p1.x && r.x<=p2.x && r.x>=q1.x && r.x<=q2.x) || !(r.y>=p_y_min && r.y<=p_y_max && r.y>=q_y_min && r.y<=q_y_max) ) 
	{
		r.x = -1000;
		r.y = -1000;
	}
 	return r;
}

result *merge(result *r1, result *r2, int n1, int n2, int *r_size)
{
	result *r_final = (result *)malloc(6*(n1+n2)*sizeof(result));
	
	int i=0, j=0, k=0;
	
	/*int u=0, v=0;
	printf("\nMerge: ");
	for(;u<n1;u++)
	{
		printf("(%lf, %lf) , ",r1[u].x, r1[u].y);
	}  
	printf(" & ");
	printf("\nMerge: ");
	for(;v<n2;v++)
	{
		printf("(%lf, %lf) , ",r2[v].x, r2[v].y);
	}
	printf("\n");*/

	if (r1[i].x < r2[j].x)
	{
		r_final[k++]=r1[i++];
	}
	else 
	{
		r_final[k++]=r2[j++];
	}
	while (i<n1 && j<n2)
    {
        
		result p1,p2,q1,q2,r;

		p1 = r1[i];
		q1 = r2[j];
			
		if(i==0) { p2.x = r1[i].x;
				   p2.y = 0.0; }
		else p2 = r1[i-1];
		if(j==0) {  q2.x = r2[j].x;
				    q2.y = 0.0;}
		else q2 = r2[j-1];


		r = find_intersection(p1,p2,q1,q2);	

		
		/*printf("\n r1[i] -> (%lf, %lf) \n", p1.x, p1.y);
		printf("\n r1[i-1] -> (%lf, %lf) \n", p2.x, p2.y);
        printf("\n r2[i] -> (%lf, %lf) \n", q1.x, q1.y);
		printf("\n r2[i-1] -> (%lf, %lf) \n", q2.x, q2.y);
		printf("\n%lf,%lf\n",r.x,r.y);*/	
		
		//If we don't find an intersection
        if (r.y == -1000)
        {
			if (p1.x > q1.x)
			{
				if(predict_y(p1,p2,q1))				
					{r_final[k++]=q1;
					j++;}
				else
					j++;
			}
			else 
			{
				if(predict_y(q1,q2,p1))				
				{r_final[k++]=p1;
				 i++;}
				else
					i++;
			}				            
        }

		//If we find an intersection
        else
        {
            r_final[k++]=r;

			double slope1, slope2;			
			
			if(r.x!=p1.x && r.x!=q1.x)			
			{
				slope1 = (r.y-p1.y)/(r.x-p1.x);
				slope2 = (r.y-q1.y)/(r.x-q1.x);
			}
			else if(r.x==p1.x && r.x!=q1.x)		
			{
				slope2 = (r.y-q1.y)/(r.x-q1.x);
				slope1 = slope2-1;
			}
			else if(r.x!=p1.x && r.x==q1.x)		
			{
				slope1 = (r.y-p1.y)/(r.x-p1.x);
				slope2 = slope1-1;
			}	
						
			if (p1.x < q1.x)
			{
				if(slope1>slope2)				
					{r_final[k++]=p1;
					 i++;}
				else
					i++;
			}
			else 
			{
				if(slope2>slope1)				
					{r_final[k++]=q1;
					 j++;}
				else
					j++;
			}			
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

	/*printf("Result: ");
	i=0;
	for(;i<(*r_size);i++)
	{
		printf("(%lf, %lf) , ",r_final[i].x, r_final[i].y);
	}  
	printf("\n");*/
    return r_final;
}
	
int flag = 0;
double error = 0;

result *skyline(building *b, int start, int end, int *r_size) //parameters - array of buildings, start index of b, end index of b, array to store result, size of result
{
	//printf ("\n%d, b[2].h1 = %lf\n, error= %lf",flag, b[2].h1, error);
	if(flag == 0) {error = b[2].h1; flag=1;}
    if(start>=end) //stopping condition
	{
		result *r = (result *)malloc(3*sizeof(result));
		r[0].x = b[start].l1;
		if (start == 2) r[0].y = error; 
		else r[0].y = b[start].h1;
		r[1].x = b[start].l2;
		r[1].y = b[start].h2;
		r[2].x = b[start].l2;
		r[2].y = 0.0;
		*r_size = 3.0;
		
		/*printf("\nStopping:\n");
		int i=0;
		for(;i<(*r_size);i++)
		{
			printf("\n(%lf,%lf)\n", r[i].x,r[i].y);
		}*/
		
		return r;
	}
	
	int n = (end-start); //number of buildings - 1
	
	result *r1 = (result *)malloc(6*n*sizeof(result));
	result *r2 = (result *)malloc(6*n*sizeof(result));
	 
	r1 = skyline(b, start, start + n/2, r_size);
	int n_r1 = *r_size;
	r2 = skyline(b, start + n/2 + 1, end, r_size);
	int n_r2 = *r_size; 
	result *r_final = (result *)malloc(6*n*sizeof(result));
	
    r_final = merge(r1,r2, n_r1, n_r2, r_size);
	
	return r_final;
}	

int main()
{
	building *b = (building *)malloc(2*sizeof(building));
	int r_size = 0;
	
	b[0].l1 = 0.0;
	b[0].h1 = 10.0;
	b[0].l2 = 2.0;
	b[0].h2 = 8.0;
	
	b[1].l1 = 1.0;
	b[1].h1 = 6.0;
	b[1].l2 = 4.0;
	b[1].h2 = 15.0;

	b[2].l1 = 3.0;
	b[2].h1 = 7.5;
	b[2].l2 = 6.0;
	b[2].h2 = 9.0;

	b[3].l1 = 5.0;
	b[3].h1 = 4.0;
	b[3].l2 = 9.0;
	b[3].h2 = 4.0;


	b[4].l1 = 0.0+10.0;
	b[4].h1 = 10.0;
	b[4].l2 = 2.0+10.0;
	b[4].h2 = 8.0;
	
	b[5].l1 = 1.0+10.0;
	b[5].h1 = 6.0;
	b[5].l2 = 4.0+10.0;
	b[5].h2 = 15.0;

	b[6].l1 = 3.0+10.0;
	b[6].h1 = 7.5;
	b[6].l2 = 6.0+10.0;
	b[6].h2 = 9.0;

	b[7].l1 = 5.0+10.0;
	b[7].h1 = 4.0;
	b[7].l2 = 9.0+10.0;
	b[7].h2 = 4.0;
	result *r = skyline(b, 0, 7, &r_size);
	
	int i=0;
	//printf("\nr_size = %d\n", r_size);
	for(;i<r_size;i++)
	{
		printf("(%lf, %lf)\n",r[i].x, r[i].y);
	}

	
	/*result p1 = {0.0,0.0};
	result p2 = {1.0,0.0};
	result q1 = {0.0,0.5};
	result q2 = {0.5,1.0};

	result r = find_intersection(p1,p2,q1,q2);*/
	//printf("\nIntersection: (%lf,%lf)\n",r.x,r.y);
	return 0;
}

