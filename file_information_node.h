#ifndef FILE_INFORMATION_NODE_H
#define FILE_INFORMATION_NODE_H

struct file_information_node {
    struct file_information *file_information;
    struct file_information_node *next;
    struct file_information_node *previous;
};

// This method can be called to free everything inside a malloc'ed "free_information_node"
void free_file_information_node(struct file_information_node *list);

// This method can be called to free an entire linked list of "free_information_node".
// @Important: It must be called after consuming all nodes and getting to the last node.
void free_file_information_list_from_top_to_bottom(struct file_information_node *list);

#endif // FILE_INFORMATION_NODE_H
