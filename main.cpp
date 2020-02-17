#include <iostream>
#include <cmath>
#include <stack>

using namespace std;

struct Point {
    float x, y;
};

Point p0;

int orientation_test(Point p, Point q, Point r)
{
    float det;
      det = (q.x * r.y) + (p.x * q.y) + (p.y * r.x) - (q.x * p.y) - (r.x * q.y) - (r.y * p.x);
    if (det == 0)
        return 0;//coliniare
    if (det > 0) return 1;
    return 2;

}

float distance_from_origin(Point p1)
{
    return (p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y);
}

int compare(const void *a,const void *b)
{
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    int sort_by_or;
    sort_by_or = orientation_test(p0, *p1, *p2);
    if (sort_by_or == 0)
    {
        if (distance_from_origin(*p2) >= distance_from_origin(*p1))
            return -1;
        else return 1;
    }
    if (sort_by_or == 2)
        return -1;
    else return 1;
}

Point find_middle(stack<Point> & conv_hull )
{
    Point p = conv_hull.top();
    conv_hull.pop();
    Point q = conv_hull.top();
    conv_hull.push(p);
    return q;
}

void set_first(Point &p1, Point &p2)
{
    Point aux;
    aux = p1;
    p1 = p2;
    p2 = aux;
}

int main()
{
    int n,i;
    Point input[100];
    float coord1, coord2;
    cout << "Introduceti numarul de puncte ale poligonului convex : ";
    cin >> n;
    for (i = 0;i < n;i++)
    {
        cout << "Introduceti coordonatele carteziene (x,y) pentru punctul " << i + 1 << ": ";
        cin >> coord1 >> coord2;
        input[i].x = coord1;
        input[i].y = coord2;
    }
    cout<<"Introduceti coordonatele punctului exterior poligonului convex : ";
    cin >> coord1 >> coord2;
    input[n].x = coord1;
    input[n].y = coord2;
    n=n+1;
    // gasim cel mai de jos punct , in caz de egaliatate il gasim pe cel mai din satnga ;
    float y_min = input[0].y;
    int poz_min = 0;
    for (i = 1;i < n;i++)
    {
        if (y_min > input[i].y)
        {
            y_min = input[i].y;
            poz_min = i;
        }
        else if (y_min == input[i].y)
        {
            if (input[poz_min].x > input[i].x)
            {
                y_min = input[i].y;
                poz_min = i;
            }
        }
    }
    //punem pe prima pozite punctul cel mai de jos;
    set_first(input[0], input[poz_min]);

    //Sortam punctele fata de primul punct ,in functie de unghiul polar pe care il formeazeaza(in sens trigonometric);
    p0 = input[0];
    qsort(&input[1], n - 1, sizeof(Point), compare);


   // daca  doua sau mai multe puncte fac acealsi unghi cu p0 , le eliminam pe toate , cu exceptia  celui mai indepartat;
    int n_el_dup=1; // initializam marimea vectorului modificat;
    i = 1;
    while(i < n)
    {
        while (i < n - 1 && orientation_test(p0, input[i], input[i + 1]) == 0)
        {
            i++;
        }
        input[n_el_dup] = input[i];
        n_el_dup++;
        i++;
    }
    // daca vectorul are mai putin de 3 puncte acoperirea convexa nu este posibila;
    if (n_el_dup < 3)
    {
        cout<<"Nu este posibila acoperirea, acoperirea convexa ar fi un segment \n";
        return 0;
    }

    // cream o stiva unde bagam primele 3 puncte
    stack<Point> conv_hull;
    conv_hull.push(input[0]);
    conv_hull.push(input[1]);
    conv_hull.push(input[2]);

    for (i = 3;i < n_el_dup;i++)
    {
        while (orientation_test(find_middle(conv_hull), conv_hull.top(), input[i]) != 2)
        {
            conv_hull.pop();
        }
        conv_hull.push(input[i]);
    }
        while (!conv_hull.empty())
        {
            Point p = conv_hull.top();
            cout << "(" << p.x << ", " << p.y << ")" << endl;
            conv_hull.pop();
        }

    return 0;
}
