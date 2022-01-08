
#pragma once

#include "context.h"
#include "list.h"
#include "utils.h"

#define GENERATE_CRUD(type)                                           \
  typedef struct type##Node type##Node;                               \
                                                                      \
  struct type##Node                                                   \
  {                                                                   \
    type data;                                                        \
    type##Node *next;                                                 \
  };                                                                  \
                                                                      \
  typedef struct                                                      \
  {                                                                   \
    type data;                                                        \
    CompareFunction compare;                                          \
  } type##Condition;                                                  \
                                                                      \
  void *type##Create(void *value)                                     \
  {                                                                   \
    type##Node *node = CREATE(type##Node, 1);                         \
    node->next = NULL;                                                \
    node->data = *((type *)value);                                    \
                                                                      \
    return node;                                                      \
  }                                                                   \
                                                                      \
  void *type##GetData(void *node)                                     \
  {                                                                   \
    type##Node *n = (type##Node *)node;                               \
    return ((void *)&n->data);                                        \
  }                                                                   \
                                                                      \
  void *type##GetNext(void *node)                                     \
  {                                                                   \
    type##Node *n = (type##Node *)node;                               \
    return ((void *)n->next);                                         \
  }                                                                   \
                                                                      \
  bool type##SetData(void *node, void *value)                         \
  {                                                                   \
    type##Node *a = (type##Node *)node;                               \
    type##Node *b = (type##Node *)value;                              \
                                                                      \
    a->data = b->data;                                                \
                                                                      \
    return true;                                                      \
  }                                                                   \
                                                                      \
  bool type##SetNext(void *node, void *value)                         \
  {                                                                   \
    type##Node *n = (type##Node *)node;                               \
    n->next = (type##Node *)value;                                    \
                                                                      \
    return true;                                                      \
  }                                                                   \
                                                                      \
  bool type##ConditionFunction(void *condition, void *iter)           \
  {                                                                   \
    type##Condition *matcher = (type##Condition *)condition;          \
    type##Node *node = (type##Node *)iter;                            \
    type##Node dummy = {.data = matcher->data, .next = NULL};         \
                                                                      \
    return matcher->compare(&dummy, node);                            \
  }                                                                   \
                                                                      \
  static Context Default##type##Context = {                           \
      .create = type##Create,                                         \
      .getData = type##GetData,                                       \
      .getNext = type##GetNext,                                       \
      .setData = type##SetData,                                       \
      .setNext = type##SetNext,                                       \
      .condition = type##ConditionFunction,                           \
      .onEvent = NULL,                                                \
      .meta = NULL};                                                  \
                                                                      \
  void *on##type##Event(EventType type, void *container, void *data)  \
  {                                                                   \
    switch (type)                                                     \
    {                                                                 \
    case INSERT:                                                      \
    {                                                                 \
      List *list = (List *)container;                                 \
      type##Node *node = (type##Node *)data;                          \
      BasicMetadata *metaData = (BasicMetadata *)list->context->meta; \
                                                                      \
      metaData->counter++;                                            \
      node->data.id = metaData->counter;                              \
                                                                      \
      break;                                                          \
    }                                                                 \
    }                                                                 \
    return NULL;                                                      \
  }                                                                   \
                                                                      \
  void *type##CreateList()                                            \
  {                                                                   \
    List *list = CREATE(List, 1);                                     \
    Context *context = CREATE(Context, 1);                            \
    *context = Default##type##Context;                                \
                                                                      \
    BasicMetadata *meta = CREATE(BasicMetadata, 1);                   \
    meta->counter = 0;                                                \
    context->meta = meta;                                             \
    context->onEvent = on##type##Event;                               \
                                                                      \
    list->head = NULL;                                                \
    list->context = context;                                          \
                                                                      \
    return list;                                                      \
  }
