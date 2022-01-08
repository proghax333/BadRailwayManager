#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 64
#define SIZE_SM 32

typedef struct User
{
  char name[SIZE];
  char email[SIZE];
  char phone[SIZE_SM];
  char password[SIZE];
} User;

typedef struct Train
{
  char name[SIZE];
  char code[SIZE_SM];
} Train;

const char *usersFile = "data/users.dat";
const char *trainsFile = "data/trains.dat";

int readObject(FILE *file, void *mem, int size, int count)
{
  if (!file)
    return 0;

  int rc = fread(mem, size, count, file);

  return rc;
}

int writeObject(FILE *file, void *mem, int size, int count)
{
  if (!file)
    return 0;

  int rc = fwrite(mem, size, count, file);

  return rc;
}

bool createUser(User user)
{
  FILE *file = fopen(usersFile, "a+");
  int rc = writeObject(file, &user, sizeof(User), 1);

  fclose(file);

  return rc;
}

int findUser(User *user)
{
  FILE *file = fopen(usersFile, "rb+");

  User buf;
  int id = 0;
  while (readObject(file, &buf, sizeof(User), 1))
  {
    ++id;

    if (strcmp(user->phone, buf.phone) == 0)
    {
      *user = buf;
      break;
    }
  }

  return id;
}

int updateUser(User *user)
{
  FILE *file = fopen(usersFile, "rb+");

  User buf;
  int id = 0;

  while (readObject(file, &buf, sizeof(User), 1))
  {
    ++id;

    int location = ftell(file);
    printf("location: %d\n", location);

    if (strcmp(user->phone, buf.phone) == 0)
    {
      location -= sizeof(User);
      fseek(file, location, SEEK_SET);

      if (!writeObject(file, user, sizeof(User), 1))
      {
        fclose(file);
        return 0;
      }

      break;
    }
  }

  return 0;
}

int testCreate(void);
int testFind(void);
int testUpdate();

int main()
{
  // testCreate();
  // testFind();
  testUpdate();
  return 0;
}

int testCreate()
{
  User john = {
      "name",
      "email",
      "phone",
      "password"};

  User jack = {
      "name2",
      "email2",
      "phone2",
      "password2"};

  createUser(john);
  createUser(jack);
}

int testFind()
{
  User a = {.phone = "phone2"};
  int id;
  if (id = findUser(&a))
  {
    printf("id = %d & name = %s\n", id, a.name);
  }
}

int testUpdate()
{
  User a = {.phone = "phone2"};
  int id;
  if (id = updateUser(&a))
  {
    printf("id = %d\n", id);
  }
}
