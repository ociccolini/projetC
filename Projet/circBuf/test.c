#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circbuf.h"

int main(int argc, char * argv[])
{
	char memoire[8192];

	circbuf buffer;
	
	circbuf_format(memoire, sizeof(memoire));
	buffer = circbuf_init(memoire, sizeof(memoire));
	
	printf("%d\n", circbuf_count(buffer));
	
	circbuf_append(buffer, "coucou");
	
	printf("%d\n", circbuf_count(buffer));
	printf("%s\n", circbuf_get(buffer, 0));
	
	
	
	circbuf_append(buffer, "olivier est un con");
	
	printf("%d\n", circbuf_count(buffer));
	printf("%s\n", circbuf_get(buffer, 0));
	
	
	
	circbuf_append(buffer, "clement est trop gentil");
	
	printf("%d\n", circbuf_count(buffer));
	printf("%s\n", circbuf_get(buffer, 0));
	
	
	
	printf("%s\n", circbuf_get(buffer, 0));
	printf("%s\n", circbuf_get(buffer, 1));
	printf("%s\n", circbuf_get(buffer, 2));
	
	
	circbuf_shut(buffer);
	return 0;
	
}
