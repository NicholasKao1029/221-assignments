/**
 * @file fadeColorPicker.h
 * Definition of a fade color picker.
 *
 */
#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include "colorPicker.h"
#include "cs221util/HSLAPixel.h"
#include "point.h"
#include "center.h"
#include <cmath>
using namespace std;

/**
 * CustomColorPicker 
 * will do something creative or something
 * probably a mix of both dot and fade
 *
 */
class customColorPicker : public colorPicker
{
  public:
    /**
     * Constructs a new customColorPicker.
     *
     * @param int hueMin
     * @param int hueMax
     */
    customColorPicker(int hueMin ,int hueMax);

    /**
     * Picks the color for pixel (x, y), which is a member of point p. 
     * The new color consists of components h, s, and l.
     * The hue h, and saturation s, are unchanged by 
     * the function, but the luminance decays as the 
     * distance from the current point to the center,
     * increases.
     *
    *  The new luminance is simply newL = centerL * (fadeFactor)^(distSq).
    *  centerL, and all the information you need to compute distSq is
    * given via the input parameter p.
    *  Picks the color for pixel (x, y). 
    * If the distance from point (x,y) to a grid point is less than 
    * dotSize, then return fillColor, otherwise, it return white.
    *
    * Grid points are the points where x == y == 0 mod dotGrid. 
    * Distances between pixel locations are calculated using a 
    * Euclidian metric, but because we don't like computing square
    * roots, we compare squared distances. Based on these 
    * observations you should be able to devise a fairly simple
    * computation to use to determine if an (x,y) point is within
    * a dot. Note that if dotSize is a little more than dotGrid, the
    * entire fill region will be colored with the dot color.
    * 
     * @param p the point for which you're picking a color
     * @return The color chosen for (p).
     */
    virtual HSLAPixel operator()(point p);

  private:

    int max;
    int min;
};

#endif
