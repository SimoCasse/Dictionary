#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

struct dictionary_node
{
    const char *key;
    size_t key_len ;
    struct dictionary_node *next;
    void *value;
};

struct dictionary_table
{
    struct dictionary_node **nodes;
    size_t hashmap_size ;
};

struct dictionary_table *dictionary_table_new (const size_t hashmap_size )
{
    struct dictionary_table *table = malloc (sizeof (struct dictionary_table ));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size ;
    table->nodes = calloc (table->hashmap_size , sizeof (struct dictionary_node *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    return table;
}

struct dictionary_node *dictionary_insert(struct dictionary_table *table, const char *key, const size_t key_len, void *value)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct dictionary_node *head = table->nodes[index];
    if (!head)
    {
        table->nodes[index] = malloc(sizeof(struct dictionary_node));
        if (!table->nodes[index])
        {
            return NULL;
        }
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->next = NULL;
        table->nodes[index]->value = value;
        return table->nodes[index];
    }
    struct dictionary_node *new_item = malloc(sizeof(struct dictionary_node));
    if (!new_item)
    {
        return NULL;
    }
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    new_item->value = value;

    struct dictionary_node *tail = head;
    while (head)
    {
        tail = head;
        head = head->next;
    }
    tail->next = new_item;
}

struct dictionary_node *Search(struct dictionary_table* table, const char* key)
{
  size_t hash = djb33x_hash(key, strlen(key));
  size_t index = hash % table->hashmap_size;

  if(!table->nodes[index])
  {
    return NULL;
  }

  printf("%s", table->nodes[index]->key);

  return table->nodes[index];

}

struct dictionary_node *Pop(struct dictionary_node* node_to_pop)
{
  if(!node_to_pop)
  {
    return NULL;
  }

  free(node_to_pop);

  node_to_pop->key = NULL;
  node_to_pop->key_len = 0;

  return NULL;
}

int main (int argc, char **argv)
{
  struct dictionary_table *table = dictionary_table_new(20);

  int a = 12;
  dictionary_insert(table, "Ciao",4, &a);
  struct dictionary_node *temp = Search(table, "Ciao");
  printf("%d",*(int *)temp->value);

  //dictionary_insert(table, 50, strlen(50));
  //Pop(Search(table,2));
  //Search(table, "Ciao");
}