#include <assert.h>
#include <string.h>
#include "circbuf.h"

int main()
{
	char mem[24];	
	circbuf a;
	
	// create and manipulate a circular buffer using mem array
	circbuf_format(mem, sizeof(mem));
	a=circbuf_init(mem, sizeof(mem));
	assert(circbuf_count(a)==0);

	assert(
		circbuf_append(a, "phrase1")==0
		&& circbuf_count(a)==1
		&& !strcmp(circbuf_get(a, 0), "phrase1"));
	
	assert(
		circbuf_append(a, "toto")==0
		&& circbuf_count(a)==2
		&& !strcmp(circbuf_get(a, 0), "toto")
		&& !strcmp(circbuf_get(a, 1), "phrase1"));
		
	assert(
		circbuf_append(a, "titi")==1
		&& circbuf_count(a)==2 
		&& !strcmp(circbuf_get(a, 0), "titi")
		&& !strcmp(circbuf_get(a, 1), "toto"));
			
	circbuf_shut(a);

	// reads an existing circular buffer
	circbuf b=circbuf_init(mem, sizeof(mem));

	assert(
		circbuf_count(b)==2	
		&& !strcmp(circbuf_get(b, 0), "titi")
		&& !strcmp(circbuf_get(b, 1), "toto"));

	circbuf_shut(b);
	return 0;
}
