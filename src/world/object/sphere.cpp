#include "sphere.h"

#include "math/quadratic.h"

#include <algorithm>

Sphere::Sphere(Vector3d position, double radius, const Material* material)
: Traceable(material)
{
    this->position = position;
    this->radius = radius;
}

Sphere::~Sphere()
{
    
}

bool Sphere::trace(const Ray& r, Intersection& intersection, double min_distance)
{
    /*
     * Given the following:
     * - O = origin
     * - D = direction
     * 
     * A point along the ray at distance x is:
     * O + xD
     *
     * A sphere at origin with radius r is:
     * |V|^2 - r^2 = 0
     * for all matching points V.
     *
     * The intersection between a ray and a sphere
     * at position C is:
     * |O - C + xD|^2 - r^2 = 0
     *
     * This gives:
     * |O - C|^2 + 2(O - C)xD + x^2D^2 - r^2 = 0
     *
     * Rearranging into the for ax^2 + bx + c = 0,
     * a = D^2
     * b = 2(0 - C)D
     * c = |O - C|^2 - r^2
     *
     * This is solved using the quadratic formula.
     */

    double x1 = 0.0;
    double x2 = 0.0;

    const Vector3d& orig = r.origin();
    const Vector3d& direc = r.direction();

    Vector3d offset = orig - position; 
    float a = direc.dot(direc); 
    float b = 2 * direc.dot(offset); 
    float c = offset.dot(offset) - radius * radius;

    if (!Quadratic::solve(a, b, c, x1, x2))
    {
        return false;
    }

    if (x1 > x2 && x2 >= min_distance)
    {
        std::swap(x1, x2);
    }
 
    if (x1 < min_distance)
    { 
        return false;
    }

    Vector3d normal = r.get_position(x1) - position;
    normal.normalise();

    intersection.distance = x1;
    intersection.material = material;
    intersection.normal = normal;

    return true;
}
