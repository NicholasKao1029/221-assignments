#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
    int xcord = p.x;
    int ycord = p.y;
    int xcent = p.c.x;
    int ycent = p.c.y;
    int diffx = xcord - xcent;
    int diffy = ycord - ycent;

    diffx = pow(diffx, 2);
    diffy = pow(diffy, 2);

    int distSq = diffx + diffy;
    distSq = pow(distSq, 0.5);

    HSLAPixel ans = p.c.color;
    ans.l = ans.l * pow(fadeFactor, distSq);
    return ans;
}
