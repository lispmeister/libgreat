/*
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "random.h"

int main(int argc, char **argv) {
	unsigned int range;

	if(argc != 2) {
		fputs("usage: random_test <number>\n", stderr);
		return EXIT_FAILURE;
	}

	/* XXX Remove once we provide a seed interface */
	srand((unsigned int) time(NULL));

	range = atoi(argv[1]);

	printf("great_random_choice(%u): %u\n",
		range, great_random_choice(range));

	printf("great_random_success(%u): %s\n",
		range, great_random_success() ? "true" : "false");

	return EXIT_SUCCESS;
}

