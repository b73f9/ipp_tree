#ifndef CUSTOM_TREE_H
#define CUSTOM_TREE_H

typedef struct Tree Tree;

Tree*    createTree();
unsigned getNodeCount(Tree * const tree);
int      getRightmostChildId(Tree * const tree, const unsigned targetNodeId);
// Returns -1 if targetNode doesn't have any children

// NOTE: addNode and splitNode cannot be called
//       more than MAX_NODE_COUNT-1 times on a particular tree.
//       (For the value of MAX_NODE_COUNT see tree.c)

void     addNode(Tree * const tree, const unsigned targetNodeId);
// Adds a new node, as targetNode's rightmost child.

void     splitNode(Tree * const tree, const unsigned targetNodeId,
                   const unsigned nodeToSplitAfterId);
// Adds a new node as targetNode's child (after nodeToSplitAfter)
// and makes targetNode's previous children after
// nodeToSplitAfter the children of the new node.

void     deleteNode(Tree * const tree, const unsigned targetNodeId);
// Deletes targetNode and places it's children in targetNode's place in
// targetNode's parent children list.

void     deleteSubtree(Tree * const tree, const unsigned targetNodeId);
void     freeTree(Tree * const tree);

#endif /* CUSTOM_TREE_H */
