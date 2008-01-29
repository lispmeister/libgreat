/*
 * $Id$
 *
 * TODO come up with a more general way to test things.
 */

#include <stdlib.h>
#include <stdio.h>

int main(void) {
	char *c;
	int i, j;
	const int total = 1000;

	j = 0;
	for(i = 0; i < total; i++) {
		c = malloc(sizeof *c);
		if(!c) {
			j++;
			continue;
		}

		free(c);
	}

	return j;
}

