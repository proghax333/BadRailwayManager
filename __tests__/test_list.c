

#include <stdio.h>

#include "../include/context.h"
#include "../include/list.h"
#include "../include/utils.h"

#include "../include/user.h"

void userDisplay(int index, void *node, void *unused)
{
  User *user = (User *)UserGetData(node);
  printf("ID: %d\nName: %s\nPhone: %s\n\n", user->id, user->name, user->phone);
}

int main()
{
  List *usersList = UserCreateList();

  for (int i = 0; i < 10; ++i)
  {
    User user;
    set(user.name, "My name bro!");
    set(user.phone, "My phone!");

    listInsert(usersList, &user);
  }

  UserNode *node;
  UserCondition condition = {
      .compare = UserCompareId};

  printf("Enter id: ");
  condition.data.id = getInt();

  if ((node = listFind(usersList, &condition)))
  {
    printf("Enter new name: ");
    getString(node->data.name, 0);
  }

  listIterate(usersList, userDisplay, NULL);

  listFree(usersList);
  return 0;
}