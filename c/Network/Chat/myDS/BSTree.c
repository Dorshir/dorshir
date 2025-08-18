#include <stdio.h>
#include <stdlib.h>
#include "BSTree.h"

#define SWAP_DATA(a, b) do { \
  void* temp = (a)->m_data; \
  (a)->m_data = (b)->m_data; \
  (b)->m_data = temp; \
} while(0)

typedef struct Node Node;
struct Node {
  void* m_data;
  Node* m_left;
  Node* m_right;
  Node* m_father;
};

struct BSTree {
  Node m_sentinel;
};

static LessComparator g_less;

/* Helper functions declarations */
static void DestroyNodesRecursive(Node* _node, void (*_destroyer)(void*));
static Node* CreateNode(void* _data, Node* _father);
static void RemoveLeaf(Node* _node);
static int isFULLTreeRecursive(Node* _node);
static int AreSimilarTreeRecursive(Node* _node1, Node* _node2);
static int CalvTreeLevelRecursive(Node* _node);
static void MirrorTreeRecursive(Node* _node);

/* Main functions */
BSTree* BSTreeCreate(LessComparator _less) {
  BSTree* tree;
  if (!_less) {
      return NULL;
  }
  if (!(tree = (BSTree*)malloc(sizeof(BSTree)))) {
      return NULL;
  }
  g_less = _less;
  tree->m_sentinel.m_data = NULL;
  tree->m_sentinel.m_left = NULL;
  tree->m_sentinel.m_right = NULL;
  tree->m_sentinel.m_father = &tree->m_sentinel;
  return tree;
}

void BSTreeDestroy(BSTree** _tree, void (*_destroyer)(void*)) {
  if (!_tree || !*_tree) {
      return;
  }
  DestroyNodesRecursive((*_tree)->m_sentinel.m_left, _destroyer);
  free(*_tree);
  *_tree = NULL;
}

BSTreeItr BSTreeInsert(BSTree* _tree, void* _item) {
  Node *newNode, *current;
  if (!_tree || !_item) {
      return NULL;
  }
  if (!_tree->m_sentinel.m_left) {
      newNode = CreateNode(_item, &_tree->m_sentinel);
      if (!newNode) {
          return NULL;
      }
      _tree->m_sentinel.m_left = newNode;
      return newNode;
  }
  current = _tree->m_sentinel.m_left;
  while (1) {
      if(!g_less(_item, current->m_data) && !g_less(current->m_data, _item)) {
          return NULL;
      }
      if (g_less(_item, current->m_data)) {
          if (!current->m_left) {
              newNode = CreateNode(_item, current);
              if (!newNode) {
                  return NULL;
              }
              current->m_left = newNode;
              break;
          }
          current = current->m_left;
      } else {
          if (!current->m_right) {
              newNode = CreateNode(_item, current);
              if (!newNode) {
                  return NULL;
              }
              current->m_right = newNode;
              break;
          }
          current = current->m_right;
      }
  }
  while (current != &_tree->m_sentinel && g_less(newNode->m_data, current->m_data)) {
      SWAP_DATA(newNode, current);
      newNode = current;
      current = (Node*)BSTreeItrPrev(current);
  }
  return newNode;
}

BSTreeItr BSTreeItrBegin(const BSTree* _tree) {
    Node* current;
    if (!_tree || !_tree->m_sentinel.m_left) {
        return (BSTreeItr)&_tree->m_sentinel;
    }
    current = _tree->m_sentinel.m_left;
    while (current->m_left) {
        current = current->m_left;
    }
    return current;
}

BSTreeItr BSTreeItrEnd(const BSTree* _tree) {
    return _tree ? (BSTreeItr)&_tree->m_sentinel : NULL;
}

BSTreeItr BSTreeItrNext(BSTreeItr _it) {
    Node* node = (Node*)_it;
    if (!node || node->m_father == node) {
        return NULL;
    }
    if (node->m_right) {
        node = node->m_right;
        while (node->m_left) {
            node = node->m_left;
        }
        return node;
    }
    while (node->m_father && node == node->m_father->m_right) {
        node = node->m_father;
    }
    return node->m_father;
}

BSTreeItr BSTreeItrPrev(BSTreeItr _it){
    Node* node=(Node*)_it;
    if(!node||node->m_father==node){
        return NULL;
    }
    if(node->m_left){
        node=node->m_left;
        while(node->m_right){
            node=node->m_right;
        }
        return node;
    }
    while(node->m_father&&node==node->m_father->m_left){
        node=node->m_father;
    }
    if(node->m_father==NULL || node->m_father->m_father==node){
        return _it;
    }
    return node->m_father;
}


void* BSTreeItrRemove(BSTreeItr _it) {
    Node* node = (Node*)_it;
    Node* replacement;
    void* data;

    if (!node || node->m_father == node) {
        return NULL;
    }

    data = node->m_data;

    if (node->m_left && node->m_right) {
        replacement = node->m_right;
        while (replacement->m_left) {
            replacement = replacement->m_left;
        }
        SWAP_DATA(node, replacement);
        node = replacement;
    }

    replacement = (node->m_left) ? node->m_left : node->m_right;

    if (replacement) {
        replacement->m_father = node->m_father;
        if (node == node->m_father->m_left) {
            node->m_father->m_left = replacement;
        } else {
            node->m_father->m_right = replacement;
        }
    } else {
        if (node == node->m_father->m_left) {
            node->m_father->m_left = NULL;
        } else {
            node->m_father->m_right = NULL;
        }
    }

    free(node);
    return data;
}

int BSTreeItrEquals(BSTreeItr _a, BSTreeItr _b) {
    return _a == _b;
}


void* BSTreeItrGet(BSTreeItr _it) {
  Node* node = (Node*)_it;
  return (node && node->m_father != node) ? node->m_data : NULL;
}

int isFULLTree(BSTree* _tree) {
   if (!_tree || !_tree->m_sentinel.m_left) {
       return 1;
   }
   return isFULLTreeRecursive(_tree->m_sentinel.m_left);
}

int AreSimilarTree(BSTree* _tree1, BSTree* _tree2)
 {
   if (!_tree1 || !_tree2) {
       return 0;
   }
   return AreSimilarTreeRecursive(_tree1->m_sentinel.m_left, _tree2->m_sentinel.m_left);
}

int CalvTreeLevel(BSTree* _tree) 
{
   if (!_tree || !_tree->m_sentinel.m_left)
   {
       return 0;
   }
   return CalvTreeLevelRecursive(_tree->m_sentinel.m_left);
}

void MirrorTree(BSTree* _tree)
 {
   if (!_tree || !_tree->m_sentinel.m_left) 
   {
       return;
   }
   MirrorTreeRecursive(_tree->m_sentinel.m_left);
}

/* Helper functions implementation */
static void DestroyNodesRecursive(Node* _node, void (*_destroyer)(void*)) {
  if (!_node) {
      return;
  }
  DestroyNodesRecursive(_node->m_left, _destroyer);
  DestroyNodesRecursive(_node->m_right, _destroyer);
  if (_destroyer) {
      _destroyer(_node->m_data);
  }
  free(_node);
}

static Node* CreateNode(void* _data, Node* _father) {
  Node* node = (Node*)malloc(sizeof(Node));
  if (node) {
      node->m_data = _data;
      node->m_left = NULL;
      node->m_right = NULL;
      node->m_father = _father;
  }
  return node;
}

static void RemoveLeaf(Node* _node) 
{
  if (_node == _node->m_father->m_right)
  {
      _node->m_father->m_right = NULL;
  } else {
      _node->m_father->m_left = NULL;
  }
}

static int isFULLTreeRecursive(Node* _node)
 {
   if (!_node)
    {
       return 1;
    }
   if ((!_node->m_left && _node->m_right) || (_node->m_left && !_node->m_right)) 
   {
       return 0;
   }
   return isFULLTreeRecursive(_node->m_left) && isFULLTreeRecursive(_node->m_right);
}

static int AreSimilarTreeRecursive(Node* _node1, Node* _node2) 
{
   if (!_node1 && !_node2) 
   {
       return 1;
   }
   if (!_node1 || !_node2) 
   {
       return 0;
   }
   return AreSimilarTreeRecursive(_node1->m_left, _node2->m_left) && AreSimilarTreeRecursive(_node1->m_right, _node2->m_right);
}

static int CalvTreeLevelRecursive(Node* _node) 
{
   int leftLevel, rightLevel;
   if (!_node) 
   {
       return 0;
   }
   leftLevel = CalvTreeLevelRecursive(_node->m_left);
   rightLevel = CalvTreeLevelRecursive(_node->m_right);
   return (leftLevel > rightLevel ? leftLevel : rightLevel) + 1;
}

static void MirrorTreeRecursive(Node* _node) 
{
   Node* temp;
   if (!_node)
    {
       return;
   }
   temp = _node->m_left;
   _node->m_left = _node->m_right;
   _node->m_right = temp;
   MirrorTreeRecursive(_node->m_left);
   MirrorTreeRecursive(_node->m_right);
}
