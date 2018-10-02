#include <stdlib.h>
#include <assert.h>
#include "list.h"

#define MAX_NODE_COUNT 500 * 1000 + 1

typedef struct Tree Tree;

struct Tree {
    ListItem *nodes[MAX_NODE_COUNT];
    unsigned  nodeCount;
    unsigned  nextNodeId;
};

Tree * createTree() {
    Tree *tree = calloc(1, sizeof(Tree));
    assert(tree != NULL);

    tree->nodeCount  = 1;
    tree->nextNodeId = 1;
    tree->nodes[0] = createListItem(0);

    return tree;
}

static ListItem * addNodeAndReturnAPointer(Tree * const tree,
                                           const unsigned targetNodeId) {
    assert(tree != NULL && targetNodeId < MAX_NODE_COUNT &&
           tree->nextNodeId < MAX_NODE_COUNT);

    ListItem * const targetNode = tree->nodes[targetNodeId];
    assert(targetNode != NULL);
    ListItem * const newNode = createListItem(tree->nextNodeId);
    tree->nodes[tree->nextNodeId] = newNode;
    tree->nextNodeId += 1;
    tree->nodeCount  += 1;

    ListItem * const oldListEnd = getChildrenListEndFromListItem(targetNode);

    if (oldListEnd == NULL) {
        leftAnchorListItem(targetNode, newNode);
        rightAnchorListItem(targetNode, newNode);
    }
    else {
        connectListItems(oldListEnd, newNode);
        rightAnchorListItem(targetNode, newNode);
    }

    return newNode;
}

static void freeNode(Tree * const tree, ListItem * const targetNode) {
    assert(tree != NULL && targetNode != NULL && tree->nodeCount != 0);

    const unsigned targetNodeId = getIdFromListItem(targetNode);
    assert(tree->nodes[targetNodeId] != NULL);

    tree->nodes[targetNodeId] = NULL;
    tree->nodeCount -= 1;

    free(targetNode);
}

int getRightmostChildId(Tree * const tree, const unsigned targetNodeId) {
    assert(tree != NULL && targetNodeId < MAX_NODE_COUNT);

    ListItem * const targetNode = tree->nodes[targetNodeId];
    assert(targetNode != NULL);

    if (getChildrenListEndFromListItem(targetNode) == NULL) {
        assert(getChildrenListBeginningFromListItem(targetNode) == NULL);
        return -1;
    }
    else {
        assert(getChildrenListBeginningFromListItem(targetNode) != NULL);
    }

    return getIdFromListItem(getChildrenListEndFromListItem(targetNode));
}

unsigned getNodeCount(Tree * const tree) {
    assert(tree != NULL);
    return tree->nodeCount;
}

void addNode(Tree * const tree, const unsigned targetNodeId) {
    addNodeAndReturnAPointer(tree, targetNodeId);
}

void deleteNode(Tree * const tree, const unsigned targetNodeId) {
    assert(tree != NULL && targetNodeId != 0 && targetNodeId < MAX_NODE_COUNT);

    ListItem * const targetNode = tree->nodes[targetNodeId];
    assert(targetNode != NULL);

    ListItem * const childrenBeginning = getChildrenListBeginningFromListItem(targetNode);
    ListItem * const childrenEnd       = getChildrenListEndFromListItem(targetNode);

    if (childrenBeginning != NULL) {
        assert(childrenEnd != NULL);
        insertListFragmentInPlaceOfListItem(targetNode, childrenBeginning, childrenEnd);
    }
    else {
        assert(childrenEnd == NULL);
        disconnectListItem(targetNode);
    }

    freeNode(tree, targetNode);
}

void splitNode(Tree * const tree, const unsigned targetNodeId,
               const unsigned nodeToSplitAfterId) {
    assert(tree != NULL && targetNodeId < MAX_NODE_COUNT &&
           nodeToSplitAfterId < MAX_NODE_COUNT);

    ListItem * const targetNode       = tree->nodes[targetNodeId];
    ListItem * const nodeToSplitAfter = tree->nodes[nodeToSplitAfterId];
    assert(targetNode != NULL && nodeToSplitAfter != NULL);

    ListItem * const newNodeChildrenListBeginning = nextListItem(nodeToSplitAfter);
    ListItem * const newNodeChildrenListEnd = getChildrenListEndFromListItem(targetNode);

    rightAnchorListItem(targetNode, nodeToSplitAfter);
    ListItem * const newNode = addNodeAndReturnAPointer(tree, targetNodeId);
    if (newNodeChildrenListBeginning != NULL) {
        leftAnchorListItem(newNode, newNodeChildrenListBeginning);
        rightAnchorListItem(newNode, newNodeChildrenListEnd);
    }
}

void deleteSubtree(Tree * const tree, const unsigned targetNodeId) {
    assert(tree != NULL && targetNodeId != 0 && targetNodeId < MAX_NODE_COUNT);

    ListItem * const targetNode = tree->nodes[targetNodeId];
    assert(targetNode != NULL);

    ListItem * currentNode = getChildrenListBeginningFromListItem(targetNode);
    while (currentNode != targetNode && currentNode != NULL) {
        if (getChildrenListBeginningFromListItem(currentNode) != NULL) {
            currentNode = getChildrenListBeginningFromListItem(currentNode);
        }
        else {
            ListItem * const nextNode = nextListItemOrAnchor(currentNode);
            disconnectListItem(currentNode);
            freeNode(tree, currentNode);
            currentNode = nextNode;
        }
        assert(currentNode != NULL);
    }

    disconnectListItem(targetNode);
    freeNode(tree, targetNode);
}

void freeTree(Tree * const tree) {
    assert(tree != NULL);

    for (unsigned i = 0; i < tree->nextNodeId; ++i) {
        if (tree->nodes[i] != NULL) {
            freeNode(tree, tree->nodes[i]);
        }
    }
    free(tree);
}
