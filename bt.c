#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct n
{
	int key;
	struct n* parent;
	struct n* l_child;
	struct n* r_child;
} node;

int* check; //1 if used, 0 if unused
int size = 100;

node* tree(int n, node* start)
{
	node* temp = (node*)malloc(sizeof(node));

	if(start == NULL)
		size = n;

	if(n==0) //stopping condition
	{
		temp = NULL;
		return temp;
	}

	int sign = rand()%2;
	if(sign==1)
		temp->key = rand()%size;
	else
		temp->key = - (rand()%size);
	
	//requires mapping of -ve numbers
	if( (temp->key) >= 0)
	{
		while(check[temp->key]==1)
		{
			temp->key = (temp->key+1)%size;
		}

		check[temp->key] = 1;
	}

	else
	{
		int dummy = -(temp->key);
		dummy = dummy+(size-1);

		while(check[dummy]==1)
		{
			dummy = (size) + (dummy+1)%size;
		}

		temp->key = -( dummy-(size-1) );

		check[dummy]=1;

	}	
	
	
	temp->parent = start;


	int x = 40 + rand()%21; 
	float p = x/100.0;

	int m = (int)(p*n);
	int l = n-m-1;

	temp->l_child = NULL;
	temp->r_child = NULL;

	//start = temp;
	temp->l_child = tree(m, temp);
	temp->r_child = tree(l, temp);


	/*printf("\nn = %d, temp.key = %d", n, temp->key);
	if(temp->l_child!=NULL && temp->r_child!=NULL)
		printf("\nstart = %d, l_child = %d, r_child = %d", temp->key, temp->l_child->key, temp->r_child->key);
	else if(temp->l_child==NULL && temp->r_child!=NULL)
		printf("\nstart = %d, l_child = NULL, r_child = %d", temp->key, temp->r_child->key);
	else if(temp->l_child!=NULL && temp->r_child==NULL) 
		printf("\nstart = %d, l_child = %d, r_child = NULL", temp->key, temp->l_child->key);
	else 
		printf("\nstart = %d, l_child = NULL, r_child = NULL", temp->key);*/

	return temp;
}

void inorder(node* p)
{
	if(p==NULL)
		return;

	inorder(p->l_child);
	
	printf("%d ",p->key);
	
	inorder(p->r_child);
	
}

void preorder(node* p)
{
	if(p==NULL)
		return;

	printf("%d ",p->key);

	preorder(p->l_child);
	
	preorder(p->r_child);
}


void postorder(node* p)
{
	if(p==NULL)
		return;

	postorder(p->l_child);

	postorder(p->r_child);

	printf("%d ",p->key);
}

void find_leaf(node* n, node** leaf, int* max_sum, int sum)
{
	if(n == NULL)
		return;

	sum += n->key;

	if(n->l_child == NULL && n->r_child == NULL) //if its a leaf
	{
		if(sum >= (*max_sum) )
		{
			*max_sum = sum;
			*leaf = n;
		}
	}

	find_leaf(n->l_child, leaf, max_sum, sum);
	find_leaf(n->r_child, leaf, max_sum, sum);
}

void max_root_to_leaf(node* root)
{
	if (root == NULL) return;

	node* leaf;
	int max_sum = -32767;
	int *p = &max_sum;
	int sum = 0;

	find_leaf(root, &leaf, p, sum);

	//'leaf' has the leaf with max_sum
	printf("\nleaf = %d, max_sum = %d\n", leaf->key, max_sum);

	int count=0;
	int* print = (int *)malloc(size*sizeof(int));
	while(leaf!=NULL)
	{
		print[count++] = leaf->key;
		leaf = leaf->parent;
	}

	int j=0;
	for(j=count-1; j>=0; j--)
		printf("%d ",print[j]);
	printf("\n");
}

int a[100], top=-1;

int find_max_path(node* n, int* sum, node** path, int* sum_l)
{
	if(n==NULL) return 0;


	//max paths of left and right children
	int lc = find_max_path(n->l_child, sum, path, sum_l);
	int rc = find_max_path(n->r_child, sum, path, sum_l);

	//find maximum of the two
	//printf("\n\nNode key = %d\n", n->key);

	int m = (lc>rc)?lc:rc;
	int m_child = (m+(n->key)>n->key)?(m+(n->key)):(n->key);

	//printf("m_child = %d\n", m_child);

	//sum when the node under consideration is the root of the maxsum path
	int m_root = (m_child > lc+rc+n->key)?m_child:(lc+rc+n->key);

	//printf("m_root = %d\n", m_child);

	//printf("previous sum = %d\n", *sum);


	if(*sum<=m_root) 
	{
		*path = n;
		*sum_l = *sum;
	}

    *sum = (*sum>m_root)?(*sum):m_root; 



    //printf("sum = %d\n", *sum);
 
    return m_child;	

}

int _path(node* path, int a[], int top, int sum)
{
	a[++top] = path->key;

	printf("a[top] = %d & top = %d\n", a[top], top);

	int count = 0, i;
	for(i=top; i>=0; i--)
	{
		count += a[i];
		if(count == sum)
		{
			int j;
			for(j=0; j<=top; j++)
				printf("%d ", a[j]);
			printf("\n");
			break;
		}
	}

	if(path->l_child==NULL && path->r_child==NULL && count!=sum)
	{
		printf("Decrease top to %d\n", top-1);
		top--;
	}

	if(path->l_child != NULL)
		top = _path(path->l_child, a, top, sum);
	if(path->r_child != NULL)
		top = _path(path->r_child, a, top, sum);	

	return top;
}

void print_path(node* path, int sum, int sum_l)
{
	if(path == NULL) return;

	int a[100], top = -1;

	printf("path->key = %d\n", path->key);

	top = _path(path, a, top, sum);

	int i = 0, temp = 0;

	for(i=1;i<=top;i++)
	{
		temp += a[i];
		printf("%d ", a[i]);
		if(temp == sum_l)
			printf("%d ", a[0]);
	}

	printf("\n");
}

void max_path(node* root)
{
	int s = -32767;
	int s_l = -32767;
	int* sum = &s;
	int* sum_l = &s_l;

	node* path;

	int f = find_max_path(root, sum, &path, sum_l);

	printf("\nMax sum = %d\nRoot of path = %d\nLeft sum = %d\n", *sum, path->key, *sum_l);

	print_path(path, *sum, *sum_l);
}

int main()
{
	//srand(time(NULL));
	int n;

	printf("\nEnter the number of nodes: ");
	scanf("%d", &n);

	check = (int *)malloc((2*n-1)*sizeof(int));

	int i = 0; 
	for(i=0; i<(2*n-1); i++)
		check[i] = 0;

	node* t = NULL;

	t = tree(n, t); //create the tree
	
	printf("\nInorder traversal: \n");
	inorder(t);

	printf("\nPreorder traversal: \n");
	preorder(t);

	printf("\nPostorder traversal: \n");
	postorder(t);

	max_root_to_leaf(t);

	max_path(t);

	printf("\n");
	return 0;	
}