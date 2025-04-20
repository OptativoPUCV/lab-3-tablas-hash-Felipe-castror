#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
    long id = hash(key , map -> capacity);



    while (map ->buckets[id] != NULL && map->buckets[id] -> key != NULL )
    {   
        if (is_equal(map ->buckets[id]->key, key))return;

        id = (id + 1) % map ->capacity;
    }

    map -> buckets[id] = createPair(key , value);

    map -> current = id;

    map -> size++;


}

void enlarge(HashMap * map) 
{
    enlarge_called = 1; //no borrar (testing purposes)

    Pair **old = map->buckets;
    long old_capacity = map->capacity;

    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    
    if (!map->buckets) 
    {
        exit(EXIT_FAILURE); 
    }

    
    map->size = 0;

    
    for (long i = 0; i < old_capacity; i++) 
    {
        if (old[i] != NULL && old[i]->key != NULL) 
        {
            insertMap(map, old[i]->key, old[i]->value);
        }
    }

    // Liberar la memoria de los buckets antiguos
    free(old);
}



HashMap * createMap(long capacity) 
{
    HashMap *map = NULL;
    map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)exit(EXIT_FAILURE);;
  
    map ->buckets = (Pair**)calloc(capacity , sizeof(Pair*));
    if (map ->buckets == NULL)
    {
        free(map);
        exit(EXIT_FAILURE); 
    } 

    map -> size = 0;
    map -> capacity = capacity;
    map -> current = -1;



    return map;
   return NULL;

    
}

void eraseMap(HashMap * map,  char * key) 
{    
    long id = hash(key , map -> capacity);

    while (map ->buckets[id] != NULL)
    {
        if (map ->buckets[id] -> key != NULL  && is_equal(map ->buckets[id]->key, key) )
        {
            map ->buckets[id]->key = NULL;
            map->size --;

            return;

        }
        id = (id + 1) % map ->capacity;

    }

}

Pair * searchMap(HashMap * map,  char * key)
{   

    long id = hash(key, map->capacity);

    while (map ->buckets[id] != NULL)
    {
        if (map ->buckets[id]->key != NULL && is_equal(map->buckets[id]->key , key))
        {
            map -> current = id;
            return map->buckets[id];
        }

        id = (id + 1) % map ->capacity;

        if (id == hash(key, map->capacity)) break;
    }

    return NULL;
}

Pair * firstMap(HashMap * map) 
{
    for (long i = 0; i < map->capacity ; i++)
    {
        if (map->buckets[i] != NULL && map->buckets[i]-> key != NULL)
        {
            map -> current = i;
            return map->buckets[i];
        }
    }

    return NULL;
}

Pair * nextMap(HashMap * map) 
{
    for (long i = map-> current + 1; i <map->capacity; i++)
    {
        if (map ->buckets[i] != NULL && map->buckets[i]->key != NULL)
        {
            map -> current = i;
            return map->buckets[i];
        }
    }

    return NULL;
}
