#include "block.h"

/**
 * Returns the width of the block.
 */
int Block::width() const{/*your code here*/

return data[0].size();

}

/**
 * Returns the height of the block.
 */
int Block::height() const{/*your code here*/

return data.size();

}

/**
 * Default Block constructor.
 */
Block::Block() {/* nothing */}

/**
 * Useful Block constructor.
 * Makes a block from the rectangle of width by height pixels in im
 * whose upper-left corner is at position (x,y).
 */
Block::Block(PNG & im, int x, int y, int width, int height) {/*your code here*/


for(int i = 0; i < height; i++){
   
    int ycord = y + i;
    vector < HSLAPixel > temp;
    for(int j = 0; j < width; j++){
        int xcord = x + j;
        HSLAPixel * pixel = im.getPixel(xcord,ycord);
        temp.push_back(*pixel);
    }
    data.push_back(temp);
}




}

/**
 * Draws the block at position (x,y) in the image im
 */
void Block::render(PNG & im, int x, int y) const {/*your code here*/


int h = height();
int w = width();

for(int i = 0; i < h; i++){
    int ycord = y + i;
    for(int j = 0; j < w; j++){
        int xcord = x + j;

        HSLAPixel replacement = data[i][j];
        HSLAPixel * imPixel = im.getPixel(xcord,ycord);
        *imPixel = replacement;
        
    }
    
}





}

/**
 * Changes the saturation of every pixel in the block to 0,
 * which removes the color, leaving grey.
 */
void Block::greyscale() {/*your code here*/

int w = width();
int h = height();
for(int x = 0; x < w; x++){
    for(int y = 0; y < h; y++){
        data[x][y].s = 0;
    }

}


}
