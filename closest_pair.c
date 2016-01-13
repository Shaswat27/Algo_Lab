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

double min_strip(point *strip, int size, double d)
{
  int i,j;
  double d_strip = d;
  
  for(i=0;i<size;i++)
  {
    for(j=i+1; j<size && abs(strip[j].y - strip[i].y)<d_strip; j++)
    {
      if(norm(strip[i],strip[j])<d_strip)
        d_strip = norm(strip[i],strip[j]);
    }
  }
  return d_strip;
}

double find_closest(point *px, point *py, int n)
{
  int i, j, k=0, l=0; //iterators
  double d = 10000; //change to max double
  if(n<=3)
  {
    for(i=0;i<n;i++)
    {
      for(j=i+1;j<n;j++)
      {
	double temp = norm(px[i], px[j]);
	if(temp<d) d = temp;
      }
    }
    return;
  }

  //divide step
  int mid = n/2;
  point median = px[mid];
   
  point *py_left=(point *)malloc(mid*sizeof(point));
  point *py_right = (point *)malloc((n-mid)*sizeof(point));
  
  for(i=0;i<n;i++)
  {
    if(py[i].x < median.x)
	py_left[k++] = py[i];
    else
        py_right[l++] = py[i];
  }

  //recursive calls:
  double dl = find_closest(px, py_left, mid);
  double dr = find_closest(px+mid, py_right, n-mid);

  //store min of these two distances
  d = (dl<dr)?(dl):(dr);
  
  //create a strip of size d on either side of median, max size can be n at max
  j=0;
  point *strip=(point *)malloc(mid*sizeof(point));
  for(i=0;i<n;i++)
  {
    if( abs(py[i].x - median.x)<d )
      strip[j++] = py[i];
  }

  double d_strip = min_strip(strip, j, d);

  d = (d_strip<d)?(d_strip):d;

  return d;
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
  point *px=(point *)malloc(n*sizeof(point));
  for(i=0;i<n;i++)
  {
    px[i].x = p[i].x;
    px[i].y = p[i].y;
  }
  //sort the points according to y coordinates using merge sort - > nlogn time guranteed
  mergesort(p, 0, n-1, 'y');
  point *py=(point *)malloc(n*sizeof(point));
  for(i=0;i<n;i++)
  {
    py[i].x = p[i].x;
    py[i].y = p[i].y;
  } 
  //arrays px, py created
  
  //find the closest pair
  printf("\nClosest distance = %lf",find_closest(px,py,n));
  

  /*for(i=0;i<n;i++)
  {
    printf("x: %lf\ny: %lf\n\n", p[i].x,p[i].y);
  } */
  
  return 0;
}
