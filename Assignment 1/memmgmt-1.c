#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct point_struct
{
    double xval;
    double yval;
    double zval;
    float alpha;
    char *name;
};

/* Here we use typedef to define "point" as an alias to "struct point_struct" so we don't
have to use the struct keyword every time we want a point.  C is the only language that
has this problem. */

typedef struct point_struct point;

/* Constructor for point. */

point *new_point(double xval, double yval, double zval, float alpha, char *name)
{
    point *p = malloc(sizeof(point));

    p->xval = xval;
    p->yval = yval;
    p->zval = zval;
    p->alpha = alpha;
    /*
    p->name = malloc(sizeof(char) * (strlen(name) + 1));
    OR
    p->name = calloc(sizeof(char), strlen(name) + 1);
    strcpy(p->name, name);
    ...OR JUST...
    */
    p->name = strdup(name); // strdup internally includes the malloc

    return p;
}

/* Destructor for point. */

void dispose_point(point *p)
{
    free(p->name);
    free(p);
}

void print_point(point *p)
{
    printf("Point %s:\n", p->name);
    printf("  x: %.2lf\n", p->xval);
    printf("  y: %.2lf\n", p->yval);
    printf("  z: %.2lf\n", p->zval);
    printf("  a: %.2f\n", p->alpha);
}

int main()
{
    point *p = new_point(3.0, 4.0, 8.0, 2.0, "Bob");

    printf("Hello world!\n");
    print_point(p);

    dispose_point(p);

    /* p is no longer a valid pointer after being freed.  Unless you malloc it again, DON'T USE IT
    AFTER THIS! */

    return 0;
}
