
#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "context.h"
#include "utils.h"

typedef struct List
{
  void *head;
  Context *context;
} List;

void *listInsert(List *list, void *value)
{
  if (!list)
    return NULL;

  Context *context = list->context;
  void *node = context->create(value);

  void *head = list->head;
  if (!head)
  {
    list->head = node;
    // trigger insert event
    if (context->onEvent)
      context->onEvent(
          INSERT,
          (void *)list,
          (void *)node);

    return node;
  }

  void *iter = head;
  void *p;

  while ((p = context->getNext(iter)) != NULL)
  {
    iter = p;
  }
  context->setNext(iter, node);

  // trigger insert event
  if (context->onEvent)
    context->onEvent(
        INSERT,
        (void *)list,
        (void *)node);

  return node;
}

void *listFind(List *list, void *condition)
{
  void *head = list->head;
  if (!list)
    return NULL;

  Context *context = list->context;
  void *iter = head;

  while (iter != NULL)
  {
    if (context->condition(condition, iter))
    {
      return iter;
    }

    iter = context->getNext(iter);
  }

  return NULL;
}

void *listUpdate(List *list, void *node, void *condition)
{
  void *head = list->head;
  if (!list)
    return NULL;

  Context *context = list->context;
  void *iter = head;

  while (iter != NULL)
  {
    if (context->condition(condition, iter))
    {
      context->setData(iter, node);

      // trigger update event
      if (context->onEvent)
        context->onEvent(
            UPDATE,
            (void *)list,
            (void *)node);

      return iter;
    }

    iter = context->getNext(iter);
  }

  return NULL;
}

bool listDelete(List *list, void *condition)
{
  if (!list)
    return false;

  Context *context = list->context;
  void *head = list->head;

  // list is empty
  if (!head)
    return false;

  // delete head element
  if (context->condition(condition, head))
  {
    void *next = context->getNext(head);
    list->head = next;

    // trigger delete event
    if (context->onEvent)
      context->onEvent(
          DELETE,
          (void *)list,
          (void *)head);

    free(head);

    return true;
  }

  // delete in between
  void *iter = head, *next;
  while ((next = context->getNext(iter)))
  {
    if (context->condition(condition, next))
    {
      void *temp = context->getNext(next);
      context->setNext(iter, temp);

      // trigger delete event
      if (context->onEvent)
        context->onEvent(
            DELETE,
            (void *)list,
            (void *)next);

      free(next);
      return true;
    }

    iter = next;
  }

  return false;
}

void listIterate(List *list, IteratorCallbackFunction callback, void *data)
{
  if (!list)
    return;

  Context *context = list->context;
  void *iter = list->head;
  int index = 0;
  int shared = 0;

  while (iter != NULL)
  {
    int result = callback(index, &shared, iter, data);
    if (result == EXIT)
    {
      break;
    }

    iter = context->getNext(iter);
    ++index;
  }
}

void listFreeHelper(Context *context, void *node)
{
  if (node)
  {
    listFreeHelper(context, context->getNext(node));
    free(node);
  }
}

bool listFree(List *list)
{
  if (!list)
    return false;

  listFreeHelper(list->context, list->head);
  freeContext(list->context);
  free(list);

  return true;
}