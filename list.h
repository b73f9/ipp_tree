#ifndef CUSTOM_LIST_H
#define CUSTOM_LIST_H

#include <stdbool.h>

typedef struct ListItem ListItem;
// ListItem stores a numerical id in the <0, 1048575> range and
// two pointers (anchors) - beginning and end of it's children list

ListItem* createListItem(const unsigned id);
// Allocates memory for a list item and assigns it an id
// ( id cannot be changed after creation! )

unsigned  getIdFromListItem(ListItem * const targetItem);

ListItem* getChildrenListBeginningFromListItem(ListItem * const targetItem);
ListItem* getChildrenListEndFromListItem(ListItem * const targetItem);

ListItem* nextListItem(ListItem * const targetItem);
ListItem* previousListItem(ListItem * const targetItem);
// If targetItem is the last / first item in a list, returns NULL.

ListItem* nextListItemOrAnchor(ListItem * const targetItem);
ListItem* previousListItemOrAnchor(ListItem * const targetItem);
// If targetItem is the last / first item in a list, returns a
// pointer to it's parent (item it was anchored to).
// Returns a NULL if there is no parent and targetItem is first / last.

bool isListItemLeftAnchored(ListItem * const targetItem);
bool isListItemRightAnchored(ListItem * const targetItem);
// NOTE:
// An anchored item will be first/last in the list, BUT
// first/last item in the list DOESN'T have to be anchored.

void leftAnchorListItem(ListItem * const anchor,
                        ListItem * const targetItem);
void rightAnchorListItem(ListItem * const anchor,
                         ListItem * const targetItem);

void disconnectListItem(ListItem * const targetItem);
// Connects targetItem's neighbours (or neighbour and an anchor) together.
// targetItem's neighbour pointers are set to NULL.
// If targetItem is anchored from both sides
// (targetItem is the only item in the list), parent's anchors are set to NULL.

void connectListItems(ListItem * const leftItem,
                      ListItem * const rightItem);

void insertListFragmentInPlaceOfListItem(ListItem * const targetItem,
                                         ListItem * const listBeginning,
                                         ListItem * const listEnd);

#endif /* CUSTOM_LIST_H */
