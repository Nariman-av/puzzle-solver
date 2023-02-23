#include "chain.h"

#include <cmath>
#include <iostream>
using namespace std;
#include <limits>




/**
 * Destroys the curr Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
  // delete head_; // needed?

}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is nullptr, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is nullptr, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {

  Node *newNode = new Node(ndata);
  // head is empty
  if (head_ == nullptr) {
    head_ = newNode;
    length_++;
    return newNode;
  }
  // p is nullptr
  if (p == nullptr) {
    newNode->next = head_;
    head_->prev = newNode;
    head_ = newNode;
  } 
  // p is not nullptr and there is a node after p
  if (p != nullptr && p->next != nullptr) {
    p->next->prev = newNode;
    newNode->next = p->next;
    newNode->prev = p;
    p->next = newNode;
  }
  // p is not nullptr but no nodes after p
  if (p != nullptr && p->next == nullptr) {
    newNode->next = nullptr;
    newNode->prev = p;
    p->next = newNode;
  }  
  length_++;
  return newNode;

}


/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is nullptr or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  /* your code here */
  // not equal, not Null
  if (p == q || !p || !q) return; 
  

  Node *pNext = p->next;
  Node *qNext = q->next;
  Node *pPrev = p->prev;
  Node *qPrev = q->prev;
 

  // q is after p right beside 
  if (p->next == q) {
      if (p != head_) {
        pPrev->next = q;
      }
      if (q->next != nullptr) {
        qNext->prev = p;
      }
      q->prev = pPrev;
      q->next = p;
      p->prev = q;
      p->next = qNext;
    } else if (q->next == p) {
      if (q != head_) {
        qPrev->next = p;
      }
      if (p->next != nullptr) {
        pNext->prev = q;
      }
      q->prev = p;
      q->next = pNext;
      p->prev = qPrev;
      p->next = q;
    } else {
      if (p != head_) {
        pPrev->next = q;
      } 
      if (q != head_) {
        qPrev->next = p;
      }
      if (p->next != nullptr) {
        pNext->prev = q;
      }
      
      if (q->next != nullptr) {
        qNext->prev = p;
      }
      q->prev = pPrev;
      q->next = pNext;
      p->prev = qPrev;
      p->next = qNext;
    }
    if (q == head_) {
      head_ = p;
    } else if (p == head_) {
      head_ = q;
    }
  }
  


  
  


/**
 * Destroys all dynamically allocated memory associated with the
 * curr Chain class.
 */
void Chain::clear() {
  Node *curr = this->head_;
  while (curr != nullptr) {
    Node* temp = curr;
    curr = curr->next;
    delete temp;
  }
  head_ = nullptr;
  length_ = 0;
}

/**
 * Makes the curr object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  /* your code here */
  clear();

  if (other.length_ == 0) return;

  Node *thisCurr = new Node(other.head_->data);
  head_ = thisCurr;
  Node *otherCurr = other.head_;
  
  while (otherCurr->next != nullptr) {
    Node *node = new Node(otherCurr->next->data);
    node->prev = thisCurr;
    thisCurr->next = node;

    thisCurr = thisCurr->next;
    otherCurr = otherCurr->next;
  }
  this->length_ = other.length_;
}

/* Modifies the curr chain:
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block
 *	is big for all other blocks.
 *	For each block h, find the distanceTo h from every other block
 *	and take the minimum of these distances as h's "value".
 *	Choose the block h with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block h, find the node with
 *    the block that is the closest match to follow h (to the right)
 *    among the remaining blocks, move (swap) it to follow h's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  vector<double> arrDist;
  Node *h1 = head_;
  vector<Node*> arrNode;  


  while(h1 != nullptr) {
    double minDist = __DBL_MAX__;
    Node *h2 = head_;
    
    while(h2 != nullptr) {
      if (h2 != h1) {
        double currDist = h2->data.distanceTo(h1->data);
        if (currDist < minDist) {
          minDist = currDist;
        }
      }
      h2 = h2->next;
    }
    arrDist.push_back(minDist);
    arrNode.push_back(h1);
    h1 = h1->next;
  }

  double maxDist = __DBL_MIN__;
  unsigned long maxDex = 0;
  for (unsigned long i = 0; i < arrDist.size(); i++) {
    if (arrDist[i] > maxDist) {
      maxDist = arrDist[i];
      maxDex = i;
    }
  }

  swap(head_, arrNode[maxDex]);

  Node *h = head_;
  while (h->next != nullptr) {
    double minDist = std::numeric_limits<double>::max();
    Node *curr = h->next;
    Node *minNode = nullptr;
    
    while(curr != nullptr) {
      double currDist = h->data.distanceTo(curr->data);
      if (currDist < minDist) {
          minDist = currDist;
          minNode = curr;
      }
      curr = curr->next;
    }
    if (h->next != minNode && minNode != nullptr) {
      swap(h->next, minNode);
    }
    h = h->next;
  }
}
