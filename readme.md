#### Description

A tree structure, with support for the following operations:

    + ADD_NODE <k>

        Add a new node with an id=last_created_id+1,

        and place it as the rightmost child of the <k> node

    + RIGHTMOST_CHILD <k>

        Return the id of the rightmost child of a node with id <k>

    + DELETE_NODE <k>

        Delete node with id = k (for k != 0), assigning it's children to the parent of k. 

    + DELETE_SUBTREE <k>

        Delete the subtree of node k (k != 0, k included). 

    + SPLIT_NODE <k> <w>

        Add a new node with id=last_created_id+1, and assign the new node as the

        parent of all <k>'s children to the right of <w>, placing the new node as the rightmost child of k. 


The complexity of DELETE_SUBTREE should be linear with respect to the size of the subtree getting removed. 
