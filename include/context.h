
#pragma once

typedef enum EventType
{
  INSERT,
  UPDATE,
  DELETE
} EventType;

typedef void *(*CreatorFunction)(void *value);
typedef void *(*GetterFunction)(void *node);
typedef bool (*SetterFunction)(void *node, void *newValueNode);
typedef bool (*CompareFunction)(void *a, void *b);
typedef int (*IteratorCallbackFunction)(int index, int *shared, void *node, void *data);

typedef void *(*EventListener)(EventType type, void *container, void *arg);
typedef bool (*ConditionFunction)(void *condition, void *iter);

typedef struct Context
{
  CreatorFunction create;
  GetterFunction getData, getNext;
  SetterFunction setData, setNext;
  ConditionFunction condition;

  EventListener onEvent;
  void *meta;
} Context;

void freeContext(Context *context)
{
  if (context)
  {
    free(context->meta);
    free(context);
  }
}

/* To store list metadata */
typedef struct
{
  int counter;
} BasicMetadata;
