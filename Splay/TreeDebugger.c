#include "splay.h"

#define MAX_RECURSION_DEPTH 1000
#define TREE_TYPE "%d"
#define LEFT_BRANCH  "l%d"
#define RIGHT_BRANCH "r%d"
#define LEFT_BRANCH_VALUE  curNodeNumber
#define RIGHT_BRANCH_VALUE curNodeNumber
#define MAX_COMMAND_LENGTH 100

void PrintTreeNodes(const Node* root, Node* curNode, FILE* output) {
    static int64_t rankArray[MAX_RECURSION_DEPTH] = {};
    static int32_t curRecursionDepth = 1;
    static int32_t curNodeNumber = 0;
    
    if (curNode == root) {
        curRecursionDepth = 1;
        curNodeNumber     = 0;

        for (uint32_t curIdx = 0; curIdx < MAX_RECURSION_DEPTH; curIdx++) {
            rankArray[curIdx] = 0;
        }
    }

    curNodeNumber++;
    if (rankArray[curRecursionDepth] == 0) {
        rankArray[curRecursionDepth] = curNode - root;
    }
    else {
        fprintf(output, "\t{rank = same; %ld; %ld;}\n", rankArray[curRecursionDepth], curNode - root);
    }

    if (curNode->parent) {
        fprintf(output, "\t\t%ld -> %ld;\n", 
                curNode - root, curNode->parent - root);
    }

    if ((curNode->left != NULL) && (curNode->right != NULL))  {
        fprintf(output, 
            "\t%ld[shape=plaintext, label = <"
            "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING= \"0\" CELLPADDING=\"4\">"
            "<TR>"
                "<TD COLSPAN=\"2\">" TREE_TYPE "</TD>"
            "</TR>"
            "<TR>"
                "<TD COLSPAN=\"2\">%ld</TD>"
            "</TR>"
            "<TR>"
                "<TD PORT = \"l%d\">" LEFT_BRANCH "</TD>"
                "<TD PORT = \"r%d\">" RIGHT_BRANCH "</TD>"
            "</TR>"
            "</TABLE>>];\n",
            curNode - root, curNode->key, curNode->sum,
            curNodeNumber, LEFT_BRANCH_VALUE, 
            curNodeNumber, RIGHT_BRANCH_VALUE);

        curRecursionDepth += 1;

        fprintf(output, "\t\t%ld: <l%d> -> %ld;\n", 
                curNode - root, curNodeNumber, curNode->left  - root);
        fprintf(output, "\t\t%ld: <r%d> -> %ld;\n", 
                curNode - root, curNodeNumber, curNode->right - root);

        PrintTreeNodes(root, curNode->left, output);
        PrintTreeNodes(root, curNode->right, output);
        curRecursionDepth--;
    }
    else if ((curNode->left == NULL) && (curNode->right != NULL)) {
        fprintf(output, 
            "\t%ld[shape=plaintext, label = <"
            "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING= \"0\" CELLPADDING=\"4\">"
            "<TR>"
                "<TD COLSPAN=\"2\">" TREE_TYPE "</TD>"
            "</TR>"
            "<TR>"
                "<TD COLSPAN=\"2\">%ld</TD>"
            "</TR>"
            "<TR>"
                "<TD PORT = \"r%d\">" RIGHT_BRANCH "</TD>"
            "</TR>"
            "</TABLE>>];\n",
            curNode - root, curNode->key, curNode->sum,
            curNodeNumber, RIGHT_BRANCH_VALUE);

        curRecursionDepth += 1;

        fprintf(output, "\t\t%ld: <r%d> -> %ld;\n", 
                curNode - root, curNodeNumber, curNode->right - root);

        PrintTreeNodes(root, curNode->right, output);
        curRecursionDepth--;
    }
    else if ((curNode->left != NULL) && (curNode->right == NULL)) {
        fprintf(output, 
            "\t%ld[shape=plaintext, label = <"
            "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING= \"0\" CELLPADDING=\"4\">"
            "<TR>"
                "<TD COLSPAN=\"2\">" TREE_TYPE "</TD>"
            "</TR>"
            "<TR>"
                "<TD COLSPAN=\"2\">%ld</TD>"
            "</TR>"
            "<TR>"
                "<TD PORT = \"l%d\">" LEFT_BRANCH "</TD>"
            "</TR>"
            "</TABLE>>];\n",
            curNode - root, curNode->key, curNode->sum,
            curNodeNumber, LEFT_BRANCH_VALUE);

        curRecursionDepth += 1;

        fprintf(output, "\t\t%ld: <l%d> -> %ld;\n", 
                curNode - root, curNodeNumber, curNode->left - root);

        PrintTreeNodes(root, curNode->left, output);
        curRecursionDepth--;
    }
    else {
        fprintf(output, 
            "\t%ld[shape = plaintext, label = <"
            "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING= \"0\" CELLPADDING=\"4\">"
            "<TR>"
                "<TD COLSPAN=\"2\">" TREE_TYPE "</TD>"
            "</TR>"
            "<TR>"
                "<TD COLSPAN=\"2\">%ld</TD>"
            "</TR>"
            "<TR>"
                "<TD> 0 </TD>"
                "<TD> 0 </TD>"
            "</TR>"
            "</TABLE>>];\n",
            curNode - root, curNode->key, curNode->sum);
    }
}

void MakeTreeGraph(Node* root, char* name) {
    FILE* output = fopen(name, "w");

    fprintf(output, "digraph G{ \n");
    fprintf(output, "\tnewrank=true;\n");
    PrintTreeNodes(root, root, output);
    fprintf(output, "}\n");
    fclose(output);

    char command[MAX_COMMAND_LENGTH] = "";
    sprintf(command, "dot %s -Tpng -O", name);
    system(command);

    sprintf(command, "rm %s", name);
    system(command);
}
