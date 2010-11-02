typedef struct
{
  int x;
  int y;
} Foo;

void allocate_node (Foo ** f)
{
  (*f) = (Foo *) malloc ( sizeof(Foo) );
}

void main()
{
  Foo * p;
  allocate_node (&p);
  p->x = 2;
  p->y = 3;
  free(p);
}

