/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
  if( n < 1 ) return 0;
  int v = 1;
  while( v <= n ) v <<= 1;
  return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  :upLeft(ul),size(sz),parent(par),nw(NULL),ne(NULL),sw(NULL),se(NULL)
{
  var = varAndAvg(im,ul,size,avg);
}

// QTree::Node::~Node(){
//   clearNode();
// }

// QTree::Node::Node(const Node & other){
//   copyNode(other);
// }

// // Node QTree::Node::operator=(const Node & rhs){
// //   if (this != &rhs) {
// //     clearNode();
// //     copyNode(rhs);
// //   }
// //   return *this;
// // }


// void QTree::Node::clearNode(){
//    cout << "clearing Node\n";
//   if(this->nw == NULL){
//   }else{
//     delete this->nw;
//     delete this->ne;
//     delete this->se;
//     delete this->sw;
//   }
// }


// void QTree::Node::copyNode(const Node & orig){
//   size = orig.size;
//   var = orig.var;
//   avg = orig.avg;
//   upLeft = orig.upLeft;

//   if(orig.parent == NULL){
//     parent = NULL;
//   }else{
//     parent = orig.parent;
//   }

//   if(orig.nw == NULL){
//     nw = NULL;
//   }else{
//     nw = orig.nw;
//   }

//   if(orig.ne == NULL){
//     ne = NULL;
//   }else{
//     ne = orig.ne;
//   }

//   if(orig.sw == NULL){
//     sw = NULL;
//   }else{
//     sw = orig.sw;
//   }

//   if(orig.se == NULL){
//     se = NULL;
//   }else{
//     se = orig.se;
//   }
// }


QTree::~QTree(){
  clear();
}

QTree::QTree(const QTree & other) {
  copy(other);
}


QTree & QTree::operator=(const QTree & rhs){
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}


// Need subimage from 0,0 to 2^k,2^k
// Node has upper left conner pixel (x,y)
// and size of square 

//number of leaves needs to be less than leafBound 
//spliting of a leaf means that a leaf is lost and 4 is gained 
//net increase of 3. For leafbound

//splits leaf with highest variance

QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{
  int minSize  = (imIn.width() < imIn.height()) ? imIn.width() : imIn.height();
  int subImageBound = biggestPow2(minSize);
  auto pairCord = make_pair(0,0);
  //PNG * newImage = new PNG(imIn);
  this->im = imIn;
  this->root = new Node(im, pairCord, subImageBound, NULL);
  this->numLeaf = 1;
  Q.push(root);
  split(root, false);

  // while(leafBound > numOfLeaf(root)){
  //   Node * next = Q.top();
  //   Q.pop();
  //   split(next);
  // }
  //this->numLeaf = numOfLeaf(root);
}


QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{
  
  int minSize  = (imIn.width() < imIn.height()) ? imIn.width() : imIn.height();
  int subImageBound = biggestPow2(minSize);
  auto pairCord = make_pair(0,0);
  this->root = new Node(imIn, pairCord, subImageBound, NULL);
  //PNG * newImage = new PNG(imIn);
  this->im = imIn;
  this->numLeaf = 1;
  Q.push(root);

  split(root, false);
  // while(leafBound > numOfLeaf(root) && !Q.empty()){
  //   Node * next = Q.top();
  
  //   Q.pop();
  //   split(next);
  // }
  //this->numLeaf = numOfLeaf(root);
}


bool QTree::isLeaf( Node *t ) {
  if(t == NULL){
    return false;
  }else if(t->nw){
    return false;
  }else{
    return true;
  }
}


//loop through nodes if node is leaf then increment one to return value 
int QTree::numOfLeaf(Node *t){
  if(t== NULL){
    return 0;
  }if(isLeaf(t)){
    return 1;
  }else{
    return numOfLeaf(t->ne) + numOfLeaf(t->nw) + numOfLeaf(t->se) + numOfLeaf(t->sw);
  }
}

  // too small to split 
  // empty queue 
  // leaf bound check 

void QTree::split(Node *t, bool bal) {
  
  /* YOUR CODE HERE */
  //check conditions to pop off the priority queue. 
  int origSize = t->size;
  // check if split can occur
  if(bal && (!isLeaf(t) || origSize <= 1)){    
    cout << "balance already done";
    return;
  }
  if(!bal &&((origSize <= 1) || (!isLeaf(t)))){
    if(!Q.empty()){
      Node * next = Q.top();
      Q.pop();
      split(next, false);
    }
    return;
  }else{   
    if(t == this->root){
      Q.pop();
    }
    //removes the parameter t node from the Q
    pair<int,int> origUL = t->upLeft;
    int x_cord[4] = {origUL.first,
                     origUL.first + origSize/2,
                     origUL.first,
                     origUL.first + origSize/2};
    int y_cord[4] = {origUL.second,
                     origUL.second,
                     origUL.second + origSize/2,
                     origUL.second + origSize/2};

    pair<int,int> listNewUL[4] = {};

    for(int i = 0; i < 4; i++){
      pair<int,int> temp = make_pair(x_cord[i], y_cord[i]);
      listNewUL[i] = temp; 
    }
    t->nw = new Node(this->im, listNewUL[0], origSize/2, t);   
    t->ne = new Node(this->im, listNewUL[1], origSize/2, t);
    t->sw = new Node(this->im, listNewUL[2], origSize/2, t);
    t->se = new Node(this->im, listNewUL[3], origSize/2, t);     

    //this->numLeaf = this->numLeaf + 3;
    this->Q.push(t->nw);
    this->Q.push(t->ne);
    this->Q.push(t->sw);
    this->Q.push(t->se); 
    this->numLeaf = this->numLeaf + 3;

    // cout << "There are " << numOfLeaf(this->root) << "leaves and " << Q.size() << " nodes in priority queue\n";
    if(this->balanced){
      if(t == root){
      }
      else{
      
      int id = determineSelf(t);

      Node * north = NNbr(t->parent);
      Node * south = SNbr(t->parent);
      Node * west = WNbr(t->parent);
      Node * east = ENbr(t->parent);

      bool nLeaf = isLeaf(north);
      bool sLeaf = isLeaf(south);
      bool wLeaf = isLeaf(west);
      bool eLeaf = isLeaf(east);


      if(id == 1){
        cout << "NW\n";
        if(nLeaf){
          cout << "balancing NW NORTH\n";
          split(north, true);
        }
        
        if(wLeaf){
          cout << "balancing NW WEST\n";
          split(west, true);
        }
      }
      else if(id == 2){
        cout << "NE\n";
        if(nLeaf){
          cout << "balancing NE NORTH \n";
          split(north, true);
        }
        if(eLeaf){
          cout << "balancing NE EAST\n";
          split(east, true);
        }
      }
      else if(id == 3){
        cout << "SW\n";
        if(sLeaf){
          cout << "balancing SW SOUTH\n";
          split(south, true);
        }
        if(wLeaf){
          cout << "balancing SW WEST\n";
          split(west, true);
        }
      }
      else if(id == 4){
        cout << "SE\n";
        if(sLeaf){
          cout << "balancing SE SOUTH\n";
          split(south, true);
        }
        if(eLeaf){
          cout << "balancing SE EAST\n";
          split(east, true);
        }
      }
      }
    }

    if(this->leafBound > this->numLeaf){
      Node * next = Q.top();
      if(next != NULL){
        Q.pop();
        split(next, false);
      }
    }
    //cout << "There are " << numOfLeaf(this->root) << "leaves and " << Q.size() << " nodes in priority queue\n";
}

  // FOR BALANCED QTREES-------------------------------------------------
  // A split might cause one or two nbrs of the parent of t to split
  // to maintain balance.  Note that these two nbrs exist (unless they're
  // not in the image region) because the current set of leaves are
  // balanced.

  // if( t is a NW (or NE or SW or SE) child ) then we need to check that
  // the North and West (or North and East or South and West or
  // South and East) nbrs of t->parent have children. If they don't
  // we need to split them.
  
  // If we split the node x, then we need to split any same-sized 
  // neighbor of the parent t of x that shares a side with x and is a leaf.
  
}

int QTree::determineSelf(Node *t){
  if(t->parent == NULL){
    return 0;
  }else{
    if(t->parent->nw == t){
      return 1;
    }
    else if(t->parent->ne == t){
      return 2;
    }
    else if(t->parent->sw == t){
      return 3;
    }
    else if(t->parent->se == t){
      return 4;
    }
  }
}


//find node with the cords and size given

QTree::Node* QTree::getNode(Node * ancestor, pair<int,int> nCord, int size){

  int currSize = ancestor->size;
  int currXCord = ancestor->upLeft.first;
  int currYCord = ancestor->upLeft.second;

  if(currSize > size && isLeaf(ancestor)){
    return NULL;
  }
  if(currSize == size && ancestor->upLeft == nCord){
    return ancestor;
  }else{
    // if(isLeaf(ancestor)){
    //   return NULL;
    // }
    bool isWest = nCord.first < (currXCord + currSize/2);
    bool isNorth = nCord.second < (currYCord + currSize/2);
    
    if(isWest){
      if(isNorth){
        return getNode(ancestor->nw, nCord, size);
      }else{
        return getNode(ancestor->sw, nCord, size);
      }
    }else{
      if(isNorth){
        return getNode(ancestor->ne, nCord, size);
      }else{
        return getNode(ancestor->se, nCord, size);
      }
    }
  }
}

/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */

QTree::Node * QTree::NNbr(Node *t) {
  int currSize = t->size;
  int currXCord = t->upLeft.first;
  int currYCord = t->upLeft.second;
  //Part that changes with each neighbour 
  pair<int,int> nBrCord = make_pair(currXCord, currYCord - currSize);
  if(t == NULL){
    return NULL;
  }
  if(!validNbr(t, nBrCord)){
    return NULL;
  }else{
    // might share a parent
    // if exists shares some ancestor might even be root
    // from root traverse downwards into correct subchild based on
    // nBrCords
    return getNode(root, nBrCord, currSize);
  }
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {

  int currSize = t->size;
  int currXCord = t->upLeft.first;
  int currYCord = t->upLeft.second;
  //Part that changes with each neighbour 
  pair<int,int> nBrCord = make_pair(currXCord, currYCord + currSize);
  if(t == NULL){
    return NULL;
  }
  if(!validNbr(t, nBrCord)){
    return NULL;
  }else{
    return getNode(root, nBrCord, currSize);
  }  
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */

QTree::Node * QTree::ENbr(Node *t) {

  int currSize = t->size;
  int currXCord = t->upLeft.first;
  int currYCord = t->upLeft.second;
  //Part that changes with each neighbour 
  pair<int,int> nBrCord = make_pair(currXCord + currSize, currYCord);
  if(t == NULL){
    return NULL;
  }
  if(!validNbr(t, nBrCord)){
    return NULL;
  }else{
    return getNode(root, nBrCord, currSize);
  }
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {

  int currSize = t->size;
  int currXCord = t->upLeft.first;
  int currYCord = t->upLeft.second;
  //Part that changes with each neighbour 
  pair<int,int> nBrCord = make_pair(currXCord - currSize, currYCord);
  if(t == NULL){
    return NULL;
  }
  if(!validNbr(t, nBrCord)){
    return NULL;
  }else{
    return getNode(root, nBrCord, currSize);
  }
}

bool QTree::validNbr(Node * t, pair<int,int> nCord){

  if(t->parent == NULL){
    return false;
  }

  int maxCord = root->size -1;
  int minCord = 0; 

  int nxCord = nCord.first;
  int nyCord = nCord.second;

  bool insideX = nxCord <= maxCord && nxCord >= minCord;
  bool insideY = nyCord <= maxCord && nyCord >= minCord;

  return insideX && insideY; 
}


// change the Qtree's im field 
// only leaves will have a color
// if drawFrame is true then 1 pixel border in each leaf square 



bool QTree::write(string const & fileName){

  unsigned int l = (unsigned int) root->size;
  //make new image
  PNG printIm(l,l);
  helpWrite(root, printIm);

  //im is not private field 
  // include the following line to write the image to file.
  return(printIm.writeToFile(fileName));
}

void QTree::helpWrite(Node *t, PNG & newImage){

  if(isLeaf(t)){
    unsigned int xcord = t->upLeft.first;
    unsigned int ycord = t->upLeft.second;
    //fill the appropriate pixels with the avg color of t
    for(unsigned int i = xcord; i < xcord + t->size; i++){
      for(unsigned int j = ycord; j < ycord + t->size; j++){
        RGBAPixel * temp = newImage.getPixel(i,j);
        *temp = t->avg;
      }
    } 
    // t->size - 1 because the cord var holds the first pixel value
    // which the - 1 takes into account
    if(drawFrame){
      for(unsigned int i = xcord; i < xcord + t->size; i++){
        RGBAPixel * l1 = newImage.getPixel(i,ycord);
        RGBAPixel * l2 = newImage.getPixel(i, ycord + t->size -1);
        *l1 = frameColor;
        *l2 = frameColor; 
      }
      for(unsigned int i = ycord; i < ycord + t->size; i++){
        RGBAPixel * l1 = newImage.getPixel(xcord,i);
        RGBAPixel * l2 = newImage.getPixel(xcord + t->size -1, i);
        *l1 = frameColor;
        *l2 = frameColor;
      }
    }
  } else{
    helpWrite(t->nw, newImage);
    helpWrite(t->ne, newImage);
    helpWrite(t->sw, newImage);
    helpWrite(t->se, newImage);
  }
}

//traverse the root node all the way down to it's leaves
//only delete if node is a leaf
//make sure that the node's parent who has a pointer to it is set to NULL 
//so that next iterations/calls can accurately determine that it is a leaf

void QTree::clear() {
  clearLeaves(this->root);
}

// check if t is NULL if it is return 
// if it is a leaf then delete 
// if not call clearleaves on t->children 

void QTree::clearLeaves(Node * t){
  //NW
  if(t== NULL){
    return;
  }else if(isLeaf(t)){
    delete t;
  }else{
    clearLeaves(t->nw);
    clearLeaves(t->ne);
    clearLeaves(t->sw);
    clearLeaves(t->se);
    delete t;
  }

  // if(isLeaf(t->nw)){
  //   delete t->nw;
  //   t->nw = NULL;
  // }else{
  //   clearLeaves(t->nw);
  //   delete t->nw;
  //   t->nw = NULL;
  // }

  // //NE
  // if(isLeaf(t->ne)){
  //   delete t->ne;
  //   t->ne = NULL;
  // }else{
  //   clearLeaves(t->ne);
  //   delete t->ne;
  //   t->ne = NULL;
  // }

  // //SW
  // if(isLeaf(t->sw)){
  //   delete t->sw;
  //   t->sw = NULL;
  // }else{
  //   clearLeaves(t->sw);
  //   delete t->sw;
  //   t->sw = NULL;
  // }

  // //SE
  // if(isLeaf(t->se)){
  //   delete t->se;
  //   t->se = NULL;
  // }else{
  //   clearLeaves(t->se);
  //   delete t->se;
  //   t->se = NULL;
  // }
}




void QTree::copy(const QTree & orig) {
  Node * origR = orig.root;
  this->im = orig.im;
  root = new Node(this->im, origR->upLeft, origR->size, NULL);
  //copy helper 
  leafBound = orig.leafBound;
  balanced = orig.balanced; 
  drawFrame = orig.drawFrame;
  if(drawFrame){
    frameColor =  orig.frameColor;
  }  createLeaves(origR, root);
}


void QTree::createLeaves(Node * o, Node *root){
  if(isLeaf(o)){
    return;
  }else{
    root->nw = new Node(this->im, o->nw->upLeft, o->nw->size, root);
    root->ne = new Node(this->im, o->ne->upLeft, o->ne->size, root);
    root->sw = new Node(this->im, o->sw->upLeft, o->sw->size, root);
    root->se = new Node(this->im, o->se->upLeft, o->se->size, root);

    createLeaves(o->nw, root->nw);
    createLeaves(o->ne, root->ne);
    createLeaves(o->sw, root->sw);
    createLeaves(o->se, root->se); 
  }
}

