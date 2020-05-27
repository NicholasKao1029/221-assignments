#include "grid.h"
#include "grid_given.cpp"


// PA1 functions


/**
 * Destroys the current Grid. This function should ensure that
 * memory does not leak on destruction of a grid.
 */
Grid::~Grid(){ /*your code here*/
    clear();
}

/**
 * Rotate row r (row 0 is the first) by count positions.
 * If row r is ABCDE (left to right) in grid g then row r
 * in grid g should be DEABC after the call g.rotateR(r, 2).
 * Rotate headOfCol_ if necessary.
 */
void Grid::rotateR(int r, int count) { /* your code here */
    r = r % numRows();
    int col = numCols(); //gives amount of columns thus how many nodes in a row
    count = count % col; //reduces count to a maximum of col - 1

    if(count == 0) return;
    //making two temporary vectors that hold the pointers of the top pointesr
    //as well as the bottom pointers 
    vector<Node *> tempUp; //holds temporary pointers each of the nodes UP pointer in order from the orig head
    tempUp.reserve(col);
    vector<Node *> tempDown; //holds temporary pointers each of the nodes DOWN poitner in order from the original head
    tempDown.reserve(col);

    Node* ptHead = headOfRow_[r];
    
    for(int i = 0; i < col; i++){
        tempUp[i] = ptHead->up;
        tempDown[i] = ptHead->down;
        ptHead = ptHead->right;
    }

    ptHead = headOfRow_[r];

    // for(int i = 0; i < col; i++){
    //     tempDown[i] = ptHead->down;
    //     ptHead = ptHead->right
    // }
    
    //need to figure out the new head of Row based on count
    for(int i = 0; i < count; i++){
        ptHead = ptHead->left;
    }

    headOfRow_[r] = ptHead;

    //reassemble pointers

    for(int i = 0; i < col; i++){
        Node* reUp =tempUp[i];
        Node* reDown = tempDown[i];
        
        ptHead->up = reUp;
        ptHead->down = reDown;

        reUp->down = ptHead;
        reDown->up = ptHead;
        

        ptHead = ptHead->right;
    }

    

    // TODO case where headofCOl changes 
    if(r == 0){
        ptHead = headOfRow_[r];
        for(int i = 0; i < col; i++){
            headOfCol_[i] = ptHead;
            ptHead = ptHead->right;

        }

    }

    ptHead = NULL;

}

/**
 * Rotate column c (column 0 is the first) by count positions.
 * If column c is ABCDE (top to bottom) in grid g then column c
 * in grid g should be DEABC after the call g.rotateC(c, 2). 
 * Rotate headOfRow_ if necessary.
 */
void Grid::rotateC(int c, int count) { /* your code here */
    c = c % numCols();
    int row = numRows(); //gives amount of rows thus how many nodes in a column
    count = count % row; //reduces count to a maximum of row - 1

    if(count == 0) return; 
    //making two temporary vectors that hold the pointers of the top pointesr
    //as well as the bottom pointers 
    vector<Node *> tempRight; 
    tempRight.reserve(row);
    vector<Node *> tempLeft; 
    tempLeft.reserve(row);

    Node* ptHead = headOfCol_[c];
    
    for(int i = 0; i < row; i++){
        tempRight[i] = ptHead->right;
        tempLeft[i] = ptHead->left;
        ptHead = ptHead->down;
    }

    ptHead = headOfCol_[c];

    // for(int i = 0; i < row; i++){
    //     tempLeft[i] = ptHead->left;
    //     ptHead = ptHead->down
    // }
    
    //need to figure out the new head of Col based on count
    for(int i = 0; i < count; i++){
        ptHead = ptHead->up;
    }

    headOfCol_[c] = ptHead;

    //reassemble pointers

    for(int i = 0; i < row; i++){
        Node* reRight =tempRight[i];
        Node* reLeft = tempLeft[i];
        
        ptHead->right = reRight;
        ptHead->left = reLeft;

        reRight->left = ptHead;
        reLeft->right = ptHead;

        ptHead = ptHead->down;
    }

    
    // TODO case where headofRow changes 
    if(c == 0){
        ptHead = headOfCol_[c];
        for(int i = 0; i < row; i++){
            headOfRow_[i] = ptHead;
            ptHead = ptHead->down;

        }

    }

    ptHead = NULL;

}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Grid class. Clears headOfRow_ and headOfCol_ vectors.
 * Sets bwidth_, bheight_ to zero.
 * After clear() the grid represents an empty grid.
 */
void Grid::clear() { /*your code here*/
int row = numRows();
int col = numCols();
for(int i = 0; i < row; i++){
    Node* t= headOfRow_[i];
    Node* temp;
    for(int j = 0; j < col; j++){
        temp = t->right;
        delete t;
        t = temp;
    }
}
// for(int i = 0; i < row; i++){
//     headOfRow_[i] = NULL;
// }
headOfRow_.clear();
// for(int i = 0; i < col; i++){
//     headOfCol_[i] = NULL;
// }
headOfCol_.clear();
bwidth_ = 0;
bheight_ = 0;


}


/**
 * Makes the current Grid a copy of the "other" Grid.
 * The dimensions should be the same.  The Nodes should
 * contain the same blocks, but the Nodes should be newly
 * allocated. This function is used in both the copy
 * constructor and the assignment operator for Grids.
 */
void Grid::copy(Grid const& other) { /*your code here*/
    clear();
    bheight_ = other.bheight_;
    bwidth_ = other.bwidth_;
    int nRows = other.numRows();
    int nCols = other.numCols();

    vector < vector < Node * > > A;
    for(int i = 0; i<nRows; i++) {
      vector < Node * > temp;
      Node* t = other.headOfRow_[i];
      for(int j = 0; j<nCols; j++){  
          temp.push_back(new Node(t->block));
          t = t->right;
      }  
      A.push_back(temp);
    }
    

  for( int j=0; j<nRows; j++ ) {
    for( int i=0; i<nCols; i++ ) {
      Node *p = A[j][i];
      p->up    = A[(j==0) ? nRows-1 : j-1] [i]; 
      p->down  = A[(j==nRows-1) ? 0 : j+1][i];
      p->left  = A[j] [(i==0) ? nCols-1 : i-1];
      p->right = A[j] [(i==nCols-1) ? 0 : i+1];
    }
  }
  for( int j=0; j<nRows; j++ ) {
    headOfRow_.push_back(A[j][0]);
  }
  for( int i=0; i<nCols; i++ ) {
    headOfCol_.push_back(A[0][i]);
  }
}
