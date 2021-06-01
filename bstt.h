
/*bstt.h*/
//
// Threaded binary search tree
//
#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
  private:
		struct NODE
		{
			KeyT Key;
			ValueT Value;
			NODE* Left;
			NODE* Right;
			bool isThreaded;
		};
		
    NODE* Root; // pointer to root node of tree (nullptr if empty)
	int Size = 0; // # of nodes in the tree (0 if empty)
    NODE* current; //pointer keeping track of current in begin()

  public:

    bstt() 
    { // default constructor: Creates an empty tree.
	    Root = nullptr;
	    Size = 0;
	  }

    void PostOrderDelete(NODE* cur)
  {
      if(cur == NULL)
          return; //if current node is pointing to nothing, the tree is empty
      else{ //else delete the elements of the tree in order, until it is empty.
          PostOrderDelete(cur->Left);
          if(cur->isThreaded == false){
              PostOrderDelete(cur->Right);
          }
          delete cur;
      }  
  }

   void preOrderInsert(NODE* other) {
    if (other == NULL) 
        return; 
  
    //print data of the node
    insert(other->Key,other->Value);
    
    //recur on left subtree*/
    preOrderInsert(other->Left);  

    //recur on right subtree
    if(!(other->isThreaded)){
        preOrderInsert(other->Right); 
    }
  }

  	//copy constructor
	  bstt(const bstt& other)
	  {
      Root = nullptr;
      Size = 0;
      NODE* temp = other.Root;
      preOrderInsert(temp);
	  }

	  // destructor:
	  // Called automatically by system when tree is about to be destroyed;
	  // this is our last chance to free any resources / memory used by
	  // this tree.
	  virtual ~bstt()
	  {
      PostOrderDelete(Root); //uses clear function to clear the tree
	  }

	  // operator=
	  // Clears "this" tree and then makes a copy of the "other" tree.
	  bstt& operator=(const bstt& other)
	  {
      PostOrderDelete(Root);
      Root = nullptr; //set root to nullptr
      Size = 0; //reset size to 0
      NODE* ptr = other.Root; //set another pointer to the others root
      preOrderInsert(ptr); //start inserting all values of the root in order
  	}

    void inOrder(NODE* cur, ostream& output) const {
      if(cur == nullptr){ //if current is null, no more branches to search
          return;
      }
      else{ // else 
          inOrder(cur->Left, output); //go to the left most node and print it out.
          
          if(cur->isThreaded == true && cur->Right != nullptr){ //if the current node is threaded and a right pointer exists, print out they key, value, and the key the thread is attached to
              output <<"(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
              cur->Right = NULL;
          }else{
            //else its not threaded, so just print out the key and value
              output <<"(" << cur->Key << "," << cur->Value << ")" << endl;
          }
          
          if(cur->isThreaded == false){
            //if its not threaded, go right and output the node
              inOrder(cur->Right, output);
          }
      }
   }

  	//Clears the contents of the tree, resetting the tree to empty.
  	void clear()
  	{
      PostOrderDelete(Root);
      Root = nullptr;
      Size = 0;
  	}

  	// size:
  	// Returns the # of nodes in the tree, 0 if empty.
  	// Time complexity: O(1)
  	int size() const
  	{
  		return Size;
  	}

  	// search:
  	//
  	// Searches the tree for the given key, returning true if found
  	// and false if not. If the key is found, the corresponding value
  	// is returned via the reference parameter.
  	//
  	// Time complexity: O(lgN) on average
  	bool search(KeyT key, ValueT& value) const
  	{
    NODE* cur = Root;
    
    while (cur != nullptr) //while the current pointer is pointing to something
    {
      if (key == cur->Key)  //check if it is pointing the the key in the search parameters
      {
        value = cur->Value; //set the value to the current value and return true
        return true;
      } else if (key < cur->Key)  //else search left:
      {
        cur = cur->Left;
      }
      else if (key > cur->Key){ //else search right
        if(cur->isThreaded ){ 
          cur = nullptr;
        }else{
          cur = cur->Right;
        }
      }
    }  
    //if get here, not found
    return false;
    }

  	// insert
  	// Inserts the given key into the tree; if the key has already been insert then
  	// the function returns without changing the tree.
  	// Time complexity: O(lgN) on average
  void insert(KeyT key, ValueT value)
  {
    NODE* prev = nullptr; //previous node
    NODE* cur = Root; //current node

    // 1. Search to see if tree already contains key:
    while (cur != nullptr) //while current is pointing to something
    {
      if (key == cur->Key)  // if its already pointing to the key, return because its already in the tree
        return;

      if (key < cur->Key)  //else readjust the pointers to check if its already in the trees
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        if(cur->isThreaded){
          cur = nullptr;
        }else {
          cur = cur->Right;
        }
      }
    }
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
         NODE* ptr= new NODE;
         ptr->Key = key;
         ptr->Value = value;
         ptr->Left = nullptr;
         ptr->Right = nullptr;
         ptr->isThreaded = true;

    // 3. link in the new node:
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs
    // to be updated.
    if(prev==nullptr){ //if the tree is empty, make the new node the root
       Root=ptr;
    }
    else if(key < prev->Key){ //else check the previous keys and insert accordingly
          ptr->Right = prev;
          prev->Left = ptr;
       }
    else if (key > prev->Key){
          ptr->Right = prev->Right;
          prev->isThreaded = false;
          prev->Right = ptr;
       }
    // 4. update size and we're done:
      Size++;
  }

  	// []
  	// Returns the value for the given key; if the key is not found,
  	// the default value ValueT{} is returned.
  	//
  	// Time complexity: O(lgN) on average
  	ValueT operator[] (KeyT key) const
  	{
    NODE* cur = Root; //set the current node to root
    
    while (cur != nullptr)
    {
      if (key == cur->Key)  //if the current node is pointing to they key in the [] operator, return the value
      {
        return cur->Value;
      }

      if (key < cur->Key)  //else search the tree until found
      {
        cur = cur->Left;
      }
      else
      {
        if(cur->isThreaded == true){
          cur = nullptr;
        }else{
          cur = cur->Right;
        }
      }
    }
    //if it got here and hasn't returned a value yet, return the default value, ValueT{}
  	  return ValueT{ };
  	}

  	// Finds the key in the tree, and returns the key to the "right".
  	// If the right is threaded, this will be the next inorder key.
  	// if the right is not threaded, it will be the key of whatever
  	// node is immediately to the right.
  	//
  	// If no such key exists, or there is no key to the "right", the
  	// default key value KeyT{} is returned.
  	// Time complexity: O(lgN) on average
  	KeyT operator()(KeyT key)const
  	{
      NODE* cur = Root;

      while (cur != nullptr) //looking for key in tree
      {
        if (key == cur->Key)  // already in tree
        {
          if(cur->Right != nullptr){ //if the right node is not null, readjust the node to go right, and return the key.
            cur = cur->Right;
            return cur->Key;
          }
        }
        if(key < cur->Key){ //else keep looking in the tree
          cur = cur->Left;
        } else {
          if(cur->isThreaded){ //if its threadedreturn the key
            break;
          } else { //else go right
            cur = cur->Right;
          }
        }
      }
  	  return KeyT{ };
  	}

  	// begin
    //-----------
  	// Resets internal state for an inorder traversal. After the
  	// call to begin(), the internal state denotes the first inorder
  	// key; this ensure that first call to next() function returns
  	// the first inorder key.
  	//
  	// Space complexity: O(1)
  	// Time complexity: O(lgN) on average
  	//
  	// Example usage:
  	// tree.begin();
  	// while (tree.next(key))
  	// cout << key << endl;
  	//
    void begin()
    {
      NODE* temp = Root;
      if(temp != nullptr){ //if the root is not empty,
        while(temp->Left != nullptr){ //keep going left, until you reach null
          temp = temp->Left;
        }
      }
      //after we've went all the way left, we set current pointer to temp
      current = temp;
      
  }

  	// -- Next --
  	// Uses the internal state to return the next inorder key, and
  	// then advances the internal state in anticipation of future
  	// calls. If a key is in fact returned (via the reference
  	// parameter), true is also returned.
  	//
  	// False is returned when the internal state has reached null,
  	// meaning no more keys are available. This is the end of the
  	// inorder traversal.
  	//
  	// Space complexity: O(1)
  	// Time complexity: O(lgN) on average
  	//
  	// Example usage:
  	// tree.begin();
  	// while (tree.next(key))
  	// cout << key << endl;
  	bool next(KeyT& key)
  	{
      NODE* cur = current; //make new node named cur, point to the node current
      
      if(cur == nullptr){ //if its pointing to null, no more keys, return false
          return false;
        } else { //else set the key to the current key and readjust the pointer to go inorder accordingly.
          key = cur->Key;

          if(cur->Left == NULL && cur->isThreaded == true){
            cur = cur->Right;
          } else if(cur->Left == NULL && cur->isThreaded == false){
            cur = cur->Right;
            while(cur->Left != NULL){
              cur = cur->Left;
            }
          } else if(cur->Left != NULL && cur->isThreaded == false){
            cur = cur->Right;
            while(cur->Left != NULL){
              cur = cur->Left;
            }
          } else if(cur->Left != NULL && cur->isThreaded == true) {
                cur = cur->Right;
          }
          current = cur;
          return true;
        }
	//if it reaches here, return false
	return false;
    }

    //
    // dump
    //
    // Dumps the contents of the tree to the output stream, using a
    // recursive inorder traversal.
    //
    void dump(ostream& output)const
    {
      output << "**************************************************" << endl; 
      output << "********************* BSTT ***********************" << endl;
      output << "** size: " << this->size() << endl;
	  inOrder(Root, output);
      // inorder traversal, with one output per line: either
      // (key,value) or (key,value,THREAD)
      // (key,value) if the node is not threaded OR thread==nullptr
      // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
      output << "**************************************************" << endl;
    }
};