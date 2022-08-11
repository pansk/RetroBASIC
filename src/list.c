/* list (implementation) for RetroBASIC
   Copyright (C) 2020 Maury Markowitz
    
This file is part of RetroBASIC.

RetroBASIC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

RetroBASIC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RetroBASIC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include <string.h>
#include "list.h"

/*
 * Creates an empty list node. Private method.
 */
list_t* _lst_alloc(void);
list_t* _lst_alloc() {
  list_t *node = (list_t *)malloc(sizeof(list_t));
  if(node == NULL)
    return NULL;
  
  node->data = NULL;
  node->key = NULL;
  node->next = NULL;
  node->prev = NULL;
  
  return node;
}

/*
 * Removes and frees the list itself. The user has to free the items within first!
 */
void lst_free(list_t *list)
{
  if(list == NULL)
    return;

  list_t* tail = list;
  if(tail->next != NULL)
    tail = lst_last(list);
  list_t* temp = tail;
  
  while(tail->prev != NULL) {
    tail = tail->prev;
    free(temp);
    temp = tail;
  }
}

/*
 * Removes and frees the entire list and the user data within.
 */
void lst_free_everything(list_t *list)
{
  if(list == NULL)
    return;
  
  list_t* tail = list;
  if(tail->next != NULL)
    tail = lst_last(list);
  
  list_t* temp;
  while(tail->prev != NULL) {
    temp = tail;
    tail = tail->prev;
    if(temp->data != NULL)
      free(temp->data);
    if(temp->key != NULL)
      free(temp->key);
    free(temp);
  }
  // and then delete the remaining node
  if(tail->data != NULL)
    free(tail->data);
  if(tail->key != NULL)
    free(tail->key);
  tail->next = NULL;
  free(tail);
  // TODO: at this point the original list still has one invalid node in it, and will thus say length 1
}

/*
 * Returns the length of the list.
 */
int lst_length(list_t *list)
{
  int length = 0;

  list_t *head = lst_first(list);
  while(head != NULL) {
    length++;
    head = head->next;
  }
  
  return length;
}

/*
 * Returns the first node.
 */
list_t* lst_first(list_t *list)
{
  if(list == NULL)
    return list;
  
  // walk backwards until we get a NULL prev
  list_t *node = list;
  while(node->prev != NULL)
    node = node->prev;
  
  return node;
}

/*
 * Returns the last node.
 */
list_t* lst_last(list_t *list)
{
  if(list == NULL)
    return list;
  
  // walk forward until we get a NULL next
  list_t *node = list;
  while(node->next != NULL)
    node = node->next;
  
  return node;
}

/*
 * Returns the node for a given data item.
 */
list_t* lst_item(list_t *list, void* data)
{
  if(list == NULL)
    return list;

  list_t* node = lst_first(list);
  if(node == NULL)
    return NULL;
  
  while(node->data != data && node->next != NULL)
    node = node->next;
  
  // only return it if its in the list, if we hit the end return null
  if(node->data == data)
    return node;
  else
    return NULL;
}

/*
 * Returns the node at a given index.
 */
list_t* lst_item_at(list_t *list, int index)
{
  if(list == NULL)
    return list;

  list_t* node = list;
  if(node->prev != NULL)
    node = lst_first(list);
  
  int i = 0;
  while(node->next != NULL && i < index) {
    node = node->next;
    i++;
  }
  
  // only return it if its in the list, if we hit the end return null
  if(i == index)
    return node;
  else
    return NULL;
}
/*
 * Returns the node with a given key.
 */
list_t* lst_item_with_key(list_t *list, char *key)
{
  if(list == NULL)
    return list;

  list_t* node = lst_first(list);
  while(node != NULL && node->key != NULL && strcmp(key, node->key) != 0)
    node = node->next;
  
  return node;
}


/*
 * Returns the index of an item (pointer) or -1 if it's not found.
 */
int lst_position_of(list_t *list, void *data)
{
  if(list == NULL)
    return -1;

  list_t* node = list;
  if(node->prev != NULL)
    node = lst_first(list);

  int pos = 0;
  while(node->data != data && node->next != NULL) {
    node = node->next;
    pos++;
  }
  
  // only return it if its in the list, if we hit the end return null
  if(node->data == data)
    return pos;
  else
    return -1;
}

/*
 * Adds a value at the end of the given list.
 */
list_t* lst_append(list_t* list, void *data)
{
  // always going to produce at least one new node
  list_t *new_list;
  new_list = _lst_alloc();
  new_list->data = data;
  
  // now add it to the end of there were other items already
  list_t *last_existing;
  if(list != NULL) {
    last_existing = list;
    if(last_existing->next != NULL)
      last_existing = lst_last(list);
    
    last_existing->next = new_list;
    new_list->prev = last_existing;
    
    return list;
  }
  else {
    new_list->prev = NULL;
    return new_list;
  }
}

/*
 * Adds a value at the begining of the given list.
 */
list_t* lst_prepend(list_t* list, void *data)
{
  list_t *new_node;
  
  new_node = _lst_alloc();
  new_node->data = data;
  new_node->next = list;
  
  if(list != NULL) {
    new_node->prev = list->prev;
    if (list->prev)
      list->prev->next = new_node;
    list->prev = new_node;
  }
  else
    new_node->prev = NULL;
  
  return new_node;
}

/*
 * Inserts the data at the given index.
 */
list_t* lst_insert_at(list_t *list, void *data, int position)
{
  list_t *new_node;
  list_t *tmp_node;
  
  // if we insert before the start, that means the end, zero is the start.
  if (position < 0)
    return lst_append(list, data);
  else if (position == 0)
    return lst_prepend(list, data);
  
  tmp_node = lst_item_at(list, position);
  if(tmp_node == NULL)
    return lst_append(list, data);
  
  new_node = _lst_alloc();
  new_node->data = data;
  new_node->prev = tmp_node->prev;
  new_node->prev->next = new_node;
  new_node->next = tmp_node;
  new_node->prev = new_node;

  return list;
}

/*
 * Uses the "key" string to find the proper location to insert new data.
 */
list_t* lst_insert_sorted(list_t *list, void *data, char *key)
{
  // try to build a new node and fail out otherwise
  list_t *new_node = _lst_alloc();
  if(new_node == NULL)
    return NULL;
  new_node->data = data;
  new_node->key = key;

  // if the list is empty we always insert at the front
  if(list == NULL)
    return new_node;
  
  // get the head of the list and roll forward until we find the right location
  list_t *node_after = lst_first(list);
  while(node_after != NULL && node_after->key != NULL && strcmp(key, node_after->key) > 0)
    node_after = node_after->next;
  // FIXME: what happens if we need to be after the last node in the list?
  
  // get the previous node too, which may not exist if we're at the start
  list_t *node_before = NULL;
  if(node_after != NULL)
    node_before = node_after->prev;
  
  // and link it in, note that we might be at the end
  if(node_after != NULL) {
    node_after->prev = new_node;
    new_node->next = node_after;
  }
  if(node_before != NULL) {
    node_before->next = new_node;
    new_node->prev = node_before;
  }
  
  return new_node;
}

/*
 * Copies a list into a new list and returns the new one.
 */
list_t* lst_copy(list_t *list)
{
  list_t *new_node = NULL;

  // anything to copy?
  if(list == NULL)
    return NULL;
  
  // check that we're at the head, or move there
  if(list->prev != NULL)
    list = lst_first(list);
  
  // got memory?
  list_t *new_list = _lst_alloc();
  if(new_list == NULL)
    return NULL;
  new_list->data = list->data;

  // do the copy
  while(list->next != NULL) {
    new_node = _lst_alloc();
    new_node->data = list->data;
    new_node->key = list->key;
    lst_append(new_list, new_node);
    list = list->next;
  }
  
  return new_list;
}

/*
 * Concats two lists
 */
list_t* lst_concat(list_t *first_list, list_t *second_list)
{
  list_t *tail = lst_last(first_list);
  list_t *head = lst_first(second_list);
  if(tail != NULL) {
    tail->next = head;
  }
  if(head != NULL) {
    head->prev = tail;
  }
  return first_list;
}

/*
 * Removes and frees the node pointing to a given @p data
 */
list_t* lst_remove(list_t *list, void* data)
{
  // get the existing node for that item
  list_t* current_node = lst_item(list, data);
  if(current_node == NULL)
    return NULL;

  // get the previous and next nodes, either of which may be null
  list_t* prev_node = current_node->prev;
  list_t* next_node = current_node->next;
  
  // link the list back together
  if(prev_node != NULL)
    prev_node->next = next_node; // which may be null, which is fine
  if(next_node != NULL)
    next_node->prev = prev_node;

  free(current_node);
  return list;
}

/*
 * Removes and frees the node at the given index and returns the pointer to the associated data
 */
void* lst_remove_at(list_t *list, int position)
{
  // get the existing item at that index and fail out if it doesn't exist
  list_t* current_node = lst_item_at(list, position);
  if(current_node == NULL)
    return NULL;

  // get the previous and next nodes, either of which may be null
  list_t* prev_node = current_node->prev;
  list_t* next_node = current_node->next;
  
  // link the list back together
  if(prev_node != NULL)
    prev_node->next = next_node; // which may be null, which is fine
  if(next_node != NULL)
    next_node->prev = prev_node;

  void *data = current_node->data;
  free(current_node);
  return data;
}

/*
 * Removes and frees the node with the given key and returns the pointer to the associated data
 */
void* lst_remove_key(list_t *list, char *key)
{
  // get the existing item at that index and fail out if it doesn't exist
  list_t* current_node = lst_item_with_key(list, key);
  if(current_node == NULL)
    return NULL;

  // get the previous and next nodes, either of which may be null
  list_t* prev_node = current_node->prev;
  list_t* next_node = current_node->next;
  
  // link the list back together
  if(prev_node != NULL)
    prev_node->next = next_node; // which may be null, which is fine
  if(next_node != NULL)
    next_node->prev = prev_node;

  void *data = current_node->data;
  free(current_node);
  return data;
}

/*
 * Calls the provided function on each of the elements in the list.
 */
list_t* lst_foreach(list_t *list, void (*function)(void *key, void *data, void *user_data), void *user_data)
{
  list_t *next;
  while(list != NULL) {
      next = list->next;
      (*function)(list->key, list->data, user_data);
      list = next;
    }
  return list;
}

/*
 * Alias for prepend, for API purposes
 */
list_t* lst_push(list_t *list, void *data)
{
  return lst_prepend(list, data);
}

/*
 * Alias for remove_at(0), for API purposes
 */
void* lst_pop(list_t *list)
{
  return lst_remove_at(list, 0);
}