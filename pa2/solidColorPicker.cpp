#include "solidColorPicker.h"

solidColorPicker::solidColorPicker()
{
    
}

HSLAPixel solidColorPicker::operator()(point p)
{
    HSLAPixel ans = p.c.color;
    return ans;
}
