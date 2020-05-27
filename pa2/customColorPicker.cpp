#include "customColorPicker.h"

customColorPicker::customColorPicker(int hueMin ,int hueMax)
{
    max = hueMax;
    min = hueMin;
}

HSLAPixel customColorPicker::operator()(point p)
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

    int hueDiff = max - min;

    HSLAPixel ans = p.c.color;
    if(distSq != 0){
        if((hueDiff + ans.h <= max)){
            ans.h = hueDiff + ans.h;
        }else{
            ans.h = max;
        }
        if(distSq > 10){
            ans.l = (int) ans.l/2;
        }
    }
    return ans;
}
