#include "file_information_node.h"
#ifndef RESULT_H
#define RESULT_H

struct result {
	struct file_information_node *list;
	int total_occurrences;
};

#endif //RESULT_H