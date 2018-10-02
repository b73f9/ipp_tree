#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"

int main(int argc, char **argv) {
    int   k, w;
    bool  verbose = false;
    Tree *tree    = createTree();
    char *command = calloc(31, sizeof(char));
    assert(command != NULL);

    if (argc > 2) {
        printf("ERROR\n");
        return 1;
    }
    if (argc == 2) {
        if (strcmp(argv[1], "-v") == 0) {
            verbose = true;
        } else {
            printf("ERROR\n");
            return 1;
        }
    }

    while (scanf("%30s %d", command, &k) == 2) {
        if (strcmp(command, "RIGHTMOST_CHILD") == 0) {
            printf("%d\n", getRightmostChildId(tree, k));
        }
        else {
            if (strcmp(command, "ADD_NODE") == 0) {
                addNode(tree, k);
            }
            else if (strcmp(command, "DELETE_NODE") == 0) {
                deleteNode(tree, k);
            }
            else if (strcmp(command, "DELETE_SUBTREE") == 0) {
                deleteSubtree(tree, k);
            }
            else if (strcmp(command, "SPLIT_NODE") == 0) {
                scanf("%d", &w);
                splitNode(tree, k, w);
            }
            printf("OK\n");
        }
        if (verbose) {
            fprintf(stderr, "NODES: %d\n", getNodeCount(tree));
        }
    }

    freeTree(tree);
    free(command);

    return 0;
}
