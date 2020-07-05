// Implementation of list module.
// ---------- Add headers here ----------
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


// Each node in a linked list is stored in this structure. The user of the
// module does not have any knowledge of nodes.
struct node { struct node *back; item x; struct node *next; };
typedef struct node node;

// A list is represented using this structure. The left field holds a sentinel
// node which is before the first item node. The end field holds a sentinel node
// which is after the last item node.
struct list { node *left, *current, *right; };

// ---------- Add functions here ----------
// Create a new empty list, with a default item to return in case of error.
list *newList(item e){
  list *l = malloc(sizeof(list));
  l -> left = malloc(sizeof(node));
  l -> right = malloc(sizeof(node));
  l -> current = l -> right;
  node *t = l -> left;
  node *r = l -> right;
  t -> next = r;
  r -> back = t;
  t -> back = NULL;
  r -> next = NULL;
  r -> x = e;
  return l;
}

// Free up the list and all the data in it.
void freeList(list *xs){
  node *c = xs -> left;
    while (c != NULL){
      node *next = c -> next;
      free(c);
      c = next;
    }
    free (xs);
}

// Set the current position before the first item or after the last item,
// to begin a forward or backward traversal.
void startF(list *xs){
  xs -> current = xs -> left -> next;
}

void startB(list *xs){
  xs -> current = xs -> right -> back;
}

// Check whether the current position is at the end, going forward or backward,
// to test whether a traversal has finished.
bool endF(list *xs){
  if (xs -> current == xs -> right) return true;
  else return false;
}

bool endB(list *xs){
  if (xs -> current == xs -> left) return true;
  else return false;
}

// Move the current position one place forwards or backwards, and return true.
// If nextF or nextB is called when at the end of the list when going forward or
// backward, the functions do nothing and return false.
bool nextF(list *xs){
  if (endF(xs) == false){
    node *c = xs -> current;
    xs -> current = c -> next;
    return true;
  }
  else return false;
}

bool nextB(list *xs){
  if (endB(xs) == false){
    node *c = xs -> current;
    xs -> current = c -> back;
    return true;
  }
  else return false;
}

// Get the current item. If getF of getB is called when at the end, going
// forward or backward, the default item is returned.
item getF(list *xs){
  if (endF(xs) == false){
    return xs -> current -> x;
  }
  return -1;
}

item getB(list *xs){
  if (endB(xs) == false){
    return xs -> current -> x;
  }
  return -1;
}

// Set the current item and return true. If setF or setB is called when at the
// end, when going forward or backward, nothing is done and false is returned.
bool setF(list *xs, item x){
  if (endF(xs) == false){
    xs -> current -> x = x;
    return true;
  }
  return false;
}

bool setB(list *xs, item x){
  if (endB(xs) == false){
    xs -> current -> x = x;
    return true;
  }
  return false;
}

// Insert an item before the current position during a traversal.  The traversal
// of the remaining items is not affected.
void insertF(list *xs, item x){
  node *l = xs -> current -> back;
  node *c = xs -> current;
  node *p = malloc(sizeof(node));
  p -> x = x;
  p -> next = c;
  c -> back = p;
  l -> next = p;
  p -> back = l;
}

void insertB(list *xs, item x){
  node *l = xs -> current -> next;
  node *c = xs -> current;
  node *p = malloc(sizeof(node));
  p -> x = x;
  p -> next = l;
  c -> next = p;
  l -> back = p;
  p -> back = c;
}

// Delete the current item and return true. When iterating forward, either nextF
// or deleteF should be called to delete or step past each item. It an the end,
// nothing is done and false is returned. Similarly when going backward
bool deleteF(list *xs){
  node *c = xs -> current;
  node *b = c -> back;
  node *n = c -> next;

  if (endF(xs) == false){
    b -> next = n;
    n -> back = b;
    xs -> current = n;
    free(c);
    return true;
  }
  return false;
}

bool deleteB(list *xs){
  node *c = xs -> current;
  node *b = c -> back;
  node *bb = b -> back;

  if (endB(xs) == false){
    bb -> next = c;
    c -> back = bb;
    free(c);
    return true;
  }
  return false;
}


// Test the list module, using int as the item type. Strings are used as
// 'pictures' to describe lists. The strings "|37", "3|7", "37|" represent a
// list of two items, with the current position at the beginning, middle or end.
#ifdef listTest

// Build a list from a picture, with -1 as the error item.
list *build(char *s) {
    list *xs = malloc(sizeof(list));
    int n = strlen(s) - 1 + 2;
    node *nodes[n];
    for (int i=0; i<n; i++) nodes[i] = malloc(sizeof(node));
    for (int i=0; i<n-1; i++) nodes[i]->next = nodes[i+1];
    for (int i=1; i<n; i++) nodes[i]->back = nodes[i-1];
    xs->left = nodes[0];
    xs->left->x = -1;
    xs->right = nodes[n-1];
    xs->right->x = -1;
    node *p = nodes[1];
    for (int i=0; i<strlen(s); i++) {
        if (s[i] == '|') xs->current = p;
        else {
            p->x = s[i] - '0';
            p = p->next;
        }
    }
    return xs;
}

// Destroy a list which was created with the build function and which matches a
// picture.
void destroy(list *xs, char *s) {
    int n = strlen(s) - 1 + 2;
    node *nodes[n];
    nodes[0] = xs->left;
    for (int i=1; i<n; i++) nodes[i] = nodes[i-1]->next;
    for (int i=0; i<n; i++) free(nodes[i]);
    free(xs);
}

// Check that a list matches a picture.
bool match(list *xs, char *s) {
    int n = strlen(s) - 1 + 2;
    node *nodes[n];
    nodes[0] = xs->left;
    for (int i=1; i<n; i++) nodes[i] = nodes[i-1]->next;
    if (nodes[n-1] != xs->right) return false;
    for (int i=1; i<n; i++) {
        if (nodes[i]->back != nodes[i-1]) return false;
    }
    node *p = nodes[1];
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '|') {
            if (p != xs->current) return false;
        }
        else {
            if (p->x != s[i] - '0') return false;
            p = p->next;
        }
    }
    return true;
}

// Use constants to say which function to call.
enum { StartF, EndF, NextF, GetF, SetF, InsertF, DeleteF };
typedef int function;

// Call a given function with a possible integer argument, returning a possible
// integer or boolean result (or -1).
int call(function f, list *xs, int arg) {
    int result = -1;
    switch (f) {
        case StartF: startF(xs); break;
        case EndF: result = endF(xs); break;
        case NextF: result = nextF(xs); break;
        case GetF: result = getF(xs); break;
        case SetF: result = setF(xs, arg); break;
        case InsertF: insertF(xs, arg); break;
        case DeleteF: result = deleteF(xs); break;
        default: assert(false);
    }
    return result;
}

// Check that a given function does the right thing. The 'in' value is passed to
// the function or is -1. The 'out' value is the expected result, or -1.
bool check(function f, int in, char *before, char *after, int out) {
    list *xs = build(before);
    int result = call(f, xs, in);
    bool ok = match(xs, after) && result == out;
    destroy(xs, after);
    return ok;
}

// Test newList, and call freeList. The test for freeList is that the memory
// leak detector in the -fsanitize=address or -fsanitize=leak compiler option
// reports no problems.
void testNewList() {
    list *xs = newList(-1);
    assert(match(xs, "|"));
    freeList(xs);
}

void testStartF() {
    assert(check(StartF, -1, "|", "|", -1));
    assert(check(StartF, -1, "|37", "|37", -1));
    assert(check(StartF, -1, "3|7", "|37", -1));
    assert(check(StartF, -1, "37|", "|37", -1));
}

void testEndF() {
    assert(check(EndF, -1, "|", "|", true));
    assert(check(EndF, -1, "|37", "|37", false));
    assert(check(EndF, -1, "3|7", "3|7", false));
    assert(check(EndF, -1, "37|", "37|", true));
}

void testNextF() {
    assert(check(NextF, -1, "|", "|", false));
    assert(check(NextF, -1, "|37", "3|7", true));
    assert(check(NextF, -1, "3|7", "37|", true));
    assert(check(NextF, -1, "37|", "37|", false));
}

void testGetF() {
    assert(check(GetF, -1, "|", "|", -1));
    assert(check(GetF, -1, "|37", "|37", 3));
    assert(check(GetF, -1, "3|7", "3|7", 7));
    assert(check(GetF, -1, "37|", "37|", -1));
}

void testSetF() {
    assert(check(SetF, 5, "|", "|", false));
    assert(check(SetF, 5, "|37", "|57", true));
    assert(check(SetF, 5, "3|7", "3|5", true));
    assert(check(SetF, 5, "37|", "37|", false));
}

void testInsertF() {
    assert(check(InsertF, 5, "|", "5|", -1));
    assert(check(InsertF, 5, "|37", "5|37", -1));
    assert(check(InsertF, 5, "3|7", "35|7", -1));
    assert(check(InsertF, 5, "37|", "375|", -1));
}

static void testDelete() {
    assert(check(DeleteF, -1, "|", "|", false));
    assert(check(DeleteF, -1, "|37", "|7", true));
    assert(check(DeleteF, -1, "3|7", "3|", true));
    assert(check(DeleteF, -1, "37|", "37|", false));
}

int main() {
    testNewList();
    testStartF();
    testEndF();
    testNextF();
    testGetF();
    testSetF();
    testInsertF();
    testDelete();
    printf("List module OK\n");
    return 0;
}
#endif
