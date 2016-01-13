#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct p
{
  double x;
  double y;
} point;

typedef struct q
{
  point p1;
  point p2;
  double d;
} pair;


void merge(point *p, int l, int m, int r, char dim)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    point *L = (point *)malloc(n1*sizeof(point));
    point *R = (point *)malloc(n2*sizeof(point));
  
    for(i = 0; i < n1; i++)
    {
      L[i] = p[l+i];
    }
    for(j = 0; j < n2; j++)
    {
      R[j] = p[m+1+j];
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
            p[k]=L[i];
            i++;
          }
          else
          {
            p[k]=R[j];
            j++;
          }
        }

        if(dim == 'y')
        {        
	  if (L[i].y <= R[j].y)
          {
            p[k]=L[i];
            i++;
          }
          else
          {
            p[k]=R[j];
            j++;
          }
        }   
       k++;
    }
 
    //copy the remaining elements of L
    while (i < n1)
    {
        p[k]=L[i];
        i++;
        k++;
    }
 
    //copy the remaining elements of R
    while (j < n2)
    {
        p[k]=R[j];
        j++;
        k++;
    }
}

void mergesort(point *p, int l, int r, char dim)
{
    if (l < r)
    {
        int m = l+(r-l)/2; 
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

pair min_strip(point *strip, int size, double dist)
{
  int i,j;
  pair d_strip;
  d_strip.d = dist;
  
  for(i=0;i<size;i++)
  {
    for(j=i+1; j<size && (strip[j].y - strip[i].y)<d_strip.d; j++)
    {
      if(norm(strip[i],strip[j])<d_strip.d)
      {
        d_strip.d = norm(strip[i],strip[j]);
        d_strip.p1 = strip[i];
        d_strip.p2 = strip[j];
      }
    }
  }
  return d_strip;
}

pair find_closest(point *px, point *py, int n)
{
  int i, j, k=0, l=0; //iterators
  pair dist;
  dist.d = 10000; //change to max double
  pair m;
  m.d = 10000; 
  if(n<=3)
  {
    for(i=0;i<n;i++)
    {
      for(j=i+1;j<n;j++)
      {
	double temp = norm(px[i], px[j]);
	if(temp<m.d) 
        {
          m.d = temp;
	  m.p1 = px[i];
	  m.p2 = px[j];
        }
      }
    }
    return m;
  }

  //divide step
  int mid = n/2;
  point median = px[mid];
   
  point *py_left=(point *)malloc((mid+1)*sizeof(point));
  point *py_right = (point *)malloc((n-mid-1)*sizeof(point));
  
  for(i=0;i<n;i++)
  {
    if(py[i].x <= median.x)
	py_left[k++] = py[i];
    else
        py_right[l++] = py[i];
  }

  //recursive calls:
  pair dl = find_closest(px, py_left, mid);
  pair dr = find_closest(px+mid, py_right, n-mid);
  
  //store min of these two distances
  dist = (dl.d<dr.d)?(dl):(dr);
  
  //create a strip of size d on either side of median, max size can be n at max
  j=0;
  point *vstrip = (point *)malloc(n*sizeof(point));
  
  for(i=0;i<n;i++)
  {
    if( abs(py[i].x - median.x)<dist.d )
      {vstrip[j] = py[i]; j++;}
  }

  pair d_strip = min_strip(vstrip, j, dist.d);

  dist = (d_strip.d<dist.d)?(d_strip):(dist);

  return dist;
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
  int j=0, k=0;
  point *px=(point *)malloc(n*sizeof(point));
  for(i=0;i<n;i++)
  {
    px[i] = p[i];
  }
  //sort the points according to y coordinates using merge sort - > nlogn time guranteed
  mergesort(p, 0, n-1, 'y');
  point *py=(point *)malloc(n*sizeof(point));
  for(i=0;i<n;i++)
  {
    py[i] = p[i];
  } 
  //arrays px, py created
  
  //find the closest pair
  pair pr = find_closest(px,py,n);
  printf("\nClosest distance = %lf, between (%lf,%lf) and (%lf, %lf)\n",pr.d, pr.p1.x, pr.p1.y, pr.p2.x, pr.p2.y);
  
  return 0;
}
