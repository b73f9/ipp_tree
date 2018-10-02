#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define LEFT_ANCHOR_FLAG  (1U << 21)
#define RIGHT_ANCHOR_FLAG (1U << 22)
#define ID_MASK           0xFFFFFu

typedef struct ListItem ListItem;

struct ListItem {
    ListItem *leftNeighbour;
    ListItem *rightNeighbour;
    ListItem *childrenListBeginning;
    ListItem *childrenListEnd;
    unsigned data;
    // Node id stored in the first 20 bits, other bits reserved for flags
};

static void setListItemFlag(ListItem * const targetItem,
                            const unsigned flag, const bool value) {
    assert(targetItem != NULL && (flag & ID_MASK) == 0);

    if (value) {
        targetItem->data |= flag;
    }
    else {
        targetItem->data &= ~flag;
    }
}

void setListItemLeftAnchored(ListItem * const targetItem, const bool value) {
    setListItemFlag(targetItem, LEFT_ANCHOR_FLAG, value);
}

void setListItemRightAnchored(ListItem * const targetItem, const bool value) {
    setListItemFlag(targetItem, RIGHT_ANCHOR_FLAG, value);
}

static void nullifyConnections(ListItem * const targetItem){
    assert(targetItem != NULL);

    targetItem->leftNeighbour  = NULL;
    targetItem->rightNeighbour = NULL;

    setListItemLeftAnchored(targetItem, false);
    setListItemRightAnchored(targetItem, false);
}

ListItem* getChildrenListBeginningFromListItem(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return targetItem->childrenListBeginning;
}

ListItem* getChildrenListEndFromListItem(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return targetItem->childrenListEnd;
}

unsigned getIdFromListItem(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return targetItem->data & ID_MASK;
}

bool isListItemLeftAnchored(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return (targetItem->data & LEFT_ANCHOR_FLAG) != 0;
}

bool isListItemRightAnchored(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return (targetItem->data & RIGHT_ANCHOR_FLAG) != 0;
}

ListItem* nextListItem(ListItem * const targetItem) {
    assert(targetItem != NULL);

    if (isListItemRightAnchored(targetItem)) {
        return NULL;
    }

    return targetItem->rightNeighbour;
}

ListItem* previousListItem(ListItem * const targetItem) {
    assert(targetItem != NULL);

    if (isListItemLeftAnchored(targetItem)) {
        return NULL;
    }

    return targetItem->leftNeighbour;
}

ListItem* nextListItemOrAnchor(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return targetItem->rightNeighbour;
}

ListItem* previousListItemOrAnchor(ListItem * const targetItem) {
    assert(targetItem != NULL);
    return targetItem->leftNeighbour;
}

void leftAnchorListItem(ListItem * const anchor, ListItem * const targetItem) {
    assert(anchor != NULL && targetItem != NULL);

    anchor->childrenListBeginning = targetItem;
    targetItem->leftNeighbour = anchor;
    setListItemLeftAnchored(targetItem, true);
}

void rightAnchorListItem(ListItem * const anchor, ListItem * const targetItem) {
    assert(anchor != NULL && targetItem != NULL);

    anchor->childrenListEnd = targetItem;
    targetItem->rightNeighbour = anchor;
    setListItemRightAnchored(targetItem, true);
}

ListItem* createListItem(const unsigned id) {
    assert((id & ID_MASK) == id);

    ListItem *newListItem = calloc(1, sizeof(ListItem));
    assert(newListItem != NULL);

    newListItem->data = (id & ID_MASK);

    return newListItem;
}

void disconnectListItem(ListItem * const targetItem) {
    assert(targetItem != NULL);

    if (isListItemLeftAnchored(targetItem)) {
        previousListItemOrAnchor(targetItem)->childrenListBeginning = nextListItem(targetItem);

        if (nextListItem(targetItem) != NULL) {
            setListItemLeftAnchored(nextListItem(targetItem), true);
        }
    }
    else if (previousListItem(targetItem) != NULL) {
        previousListItem(targetItem)->rightNeighbour = nextListItemOrAnchor(targetItem);
    }

    if (isListItemRightAnchored(targetItem)) {
        nextListItemOrAnchor(targetItem)->childrenListEnd = previousListItem(targetItem);

        if (previousListItem(targetItem) != NULL) {
            setListItemRightAnchored(previousListItem(targetItem), true);
        }
    }
    else if (nextListItem(targetItem) != NULL) {
        nextListItem(targetItem)->leftNeighbour = previousListItemOrAnchor(targetItem);
    }

    nullifyConnections(targetItem);
}

void connectListItems(ListItem * const leftItem, ListItem * const rightItem) {
    assert(leftItem != NULL && rightItem != NULL);

    setListItemRightAnchored(leftItem, false);
    setListItemLeftAnchored(rightItem, false);

    leftItem->rightNeighbour = rightItem;
    rightItem->leftNeighbour = leftItem;
}

void insertListFragmentInPlaceOfListItem(ListItem * const targetItem,
                                         ListItem * const listBeginning,
                                         ListItem * const listEnd) {
    assert(targetItem != NULL && listBeginning != NULL && listEnd != NULL);

    if (isListItemLeftAnchored(targetItem)) {
        leftAnchorListItem(previousListItemOrAnchor(targetItem),
                           listBeginning);
    }
    else {
        connectListItems(previousListItem(targetItem), listBeginning);
    }

    if (isListItemRightAnchored(targetItem)) {
        rightAnchorListItem(nextListItemOrAnchor(targetItem), listEnd);
    }
    else {
        connectListItems(listEnd, nextListItem(targetItem));
    }

    nullifyConnections(targetItem);
}

