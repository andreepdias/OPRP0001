#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

int main(void)
{
  char salt[] = "Fk";
  char *password;

  password = crypt(getpass("Password:"), salt);

  puts(password);
  return 0;
}
