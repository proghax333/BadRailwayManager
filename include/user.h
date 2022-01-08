
#pragma once

#include "context.h"
#include "utils.h"
#include "crud_generator.h"

typedef struct
{
  int id;
  char name[SIZE];
  char email[SIZE];
  char phone[SIZE_SM];
  char password[SIZE];
} User;

GENERATE_CRUD(User);

bool UserCompareId(void *a, void *b)
{
  UserNode *nodeA = (UserNode *)a;
  UserNode *nodeB = (UserNode *)b;

  return nodeA->data.id == nodeB->data.id;
}

bool UserComparePhone(void *a, void *b)
{
  UserNode *nodeA = (UserNode *)a;
  UserNode *nodeB = (UserNode *)b;

  return strcmp(nodeA->data.phone, nodeB->data.phone) == 0;
}
