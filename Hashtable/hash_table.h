/* hash_table.h */
/* Hash Table implementation: header */
 
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

struct pair {
  uint32_t              hkey;
  char                 *key;
  void                 *value;
  struct pair          *next;
};
 
struct htable {
  size_t                size, capacity;
  struct pair         **tab;
};
 
/*
 * hash(data):
 * compute the hash of the nul terminated string data.
 */
uint32_t hash(char *data);
 
/*
 * create_htable(capacity):
 * build a new hash table with initial capacity.
 */
struct htable *create_htable(size_t capacity);
 
/*
 * access_htable(htable, key):
 * returns a pointer to the pair containing the given key
 */
struct pair *access_htable(struct htable *htable, char *key);
 
/*
 * add_htable(htable,key,value):
 * add the pair (key,value) to the hash table
 */
int add_htable(struct htable *htable, char *key, void *value);
 
/*
 * remove_htable(htable, key):
 * removes the pair containing the given key from the hash table
 */
void remove_htable(struct htable *htable, char *key);
 
/*
 * clear_htable(htable):
 * delete all pairs in the table
 */
void clear_htable(struct htable *htable);
 
#endif
