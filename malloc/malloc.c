#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <errno.h>


static inline size_t word_align(size_t i) {
	return i % sizeof(size_t) ? i + sizeof(size_t) - i % sizeof(size_t) : i;
}


struct chunk {
	struct chunk  *next, *prev;
	size_t        size;
	int           free;
	void          *data; // pointer to data
};


static size_t chunkSize() {
	static size_t s = 0;
	if(!s) { s = word_align(sizeof(struct chunk)); }
	return s;
}

void *base() {
	static struct chunk  *b = NULL;
	if(!b) {
		b = sbrk(chunkSize());
		if(b == (void *)-1) {
			if(errno == ENOMEM) { return NULL; }
			_exit(127);
		}
		b->size = b->free = 0;
		b->next = b->prev = b->data = NULL;
	}

	return b;
}

struct chunk *find(size_t s, struct chunk **heap) {

    struct chunk  *c = base();
    for(; c && (!c->free || c->size < s); *heap = c, c = c->next);

    return c;
}

struct chunk *new_chunk(size_t s, struct chunk *prev) {

	s = word_align(s);
	struct chunk* c = sbrk(s + chunkSize());

	if(c == (void *)-1) {
		if(errno == ENOMEM) { return NULL; }
		_exit(127);
	}

	prev->next = c;

    c->size = s;    c->free = 1;
    c->next = NULL; c->prev = prev;
    c->data = c + 1;

	return c;
}

void splitLeft(size_t s, struct chunk *curr) {

    s = word_align(s);
    size_t size = (int)((char*)curr->next - (char*)curr);
    size_t minSize = s + chunkSize() + word_align(1);

    if(size < minSize) { return; }
    size = size - s - chunkSize();

    struct chunk *next = (struct chunk*)((char*) curr + s);
    next->size = size;       next->free = 1;
    next->next = curr->next; next->prev = curr;
    next->data = next + 1;

    if(curr->next) { curr->next->prev = next; }
    curr->next = next;
}

void *malloc(size_t s) {
	struct chunk *prev = base();
	struct chunk *curr = find(s, &prev);
	
	if(!curr) {
		curr = new_chunk(s, prev);
	}
	curr->free = 0;
    if(!curr->next) { return curr->data; }

    return curr->data;


}

void free(void *p) {
	if(!p || p < base() || p > sbrk(0)) { return; }
	
	struct chunk *c = p - sizeof(struct chunk);
	if(c->data != p) { return ; }
	c->free = 1;


    if(c->prev && c->prev->free) {
        c->prev->next = c->next;
        c->prev->size += c->size + chunkSize();
		if(c->next) { c->next->prev = c->prev; }
        c = c->prev;
    }
    if(c->next && c->next->free) {
		c->next = c->next->next;
		if(c->next) { c->next->prev = c; }
        c->size += c->next->size + chunkSize();
    }
	
	if(!c->next) {
		c->prev->next = NULL;
		sbrk(-(c->size + chunkSize()));
	}
}

void *calloc(size_t n, size_t size) {
	char *p = malloc(n * size);
	for(int i = 0, s = n * size; i < s; ++i) {
		p[i] = 0;
	}
	return (void *)p;
}

void *realloc(void* old, size_t size) {
    if(!old) { return malloc(size); }
    if(old < base() || old > sbrk(0)) { return NULL; }
    struct chunk *c = old - sizeof(struct chunk);

    if(c->data != old) { return old; }

    if(size <= c->size) { return old; }
    else {
        char *p = malloc(size);
        char *o = (char *) old;
        for(size_t i = 0; i < size; ++i) {
            p[i] = o[i];
        }
        free(c);
        return p;
    }
}