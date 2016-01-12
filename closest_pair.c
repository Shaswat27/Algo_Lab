#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct p
{
  double x;
  double y;
} point;

void merge(point *p, int l, int m, int r, char dim)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    point *L = (point *)malloc(n1*sizeof(point));
    point *R = (point *)malloc(n2*sizeof(point));
  
    for(i = 0; i < n1; i++)
    {
      L[i].x = p[l+i].x;
      L[i].y = p[l+i].y;
    }
    for(j = 0; j < n2; j++)
    {
      R[j].x = p[m+1+j].x;
      R[j].y = p[m+1+j].y;
    }    
     
    //merge the temp arrays back into p
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
	if(dim == 'x')
        {        
	  if (L[i].x <= R[j].x)
          {
            p[k].x = L[i].x;
            p[k].y = L[i].y;
            i++;
          }
          else
          {
            p[k].x = R[j].x;
            p[k].y = R[j].y;
            j++;
          }
          k++;
        }

        if(dim == 'y')
        {        
	  if (L[i].y <= R[j].y)
          {
            p[k].x = L[i].x;
            p[k].y = L[i].y;
            i++;
          }
          else
          {
            p[k].x = R[j].x;
            p[k].y = R[j].y;
            j++;
          }
          k++;
        }        
    }
 
    //copy the remaining elements of L
    while (i < n1)
    {
        p[k].x = L[i].x;
        p[k].y = L[i].y;
        i++;
        k++;
    }
 
    //copy the remaining elements of R
    while (j < n2)
    {
        p[k].x = L[j].x;
        p[k].y = L[j].y;
        j++;
        k++;
    }
}

void mergesort(point *p, int l, int r, char dim)
{
    if (l < r)
    {
        int m = (l+r)/2; 
        mergesort(p, l, m, dim);
        mergesort(p, m+1, r, dim);
        merge(p, l, m, r, dim);
    }
}

double norm(point p1, point p2)
{
  double n = sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );
  return n;
}

int main()
{
  int n,i;
  printf("Enter n: ");
  scanf("%d",&n);
  
  //n points stored in n
  point *p=(point *)malloc(n*sizeof(point));
  for(i=0;i<n;i++)
  {
    printf("\nEnter the x coordinate: ");
    scanf("%lf",&p[i].x);
    printf("Enter the y coordinate: ");
    scanf("%lf",&p[i].y);
  }
   
  //sort the points according to x coordinates using merge sort -> nlogn time guaranteed
  mergesort(p, 0, n-1, 'x');

  

  for(i=0;i<n;i++)
  {
    printf("x: %lf\ny: %lf\n\n", p[i].x,p[i].y);
  } 
  
  return 0;
}
