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

/*
le arquivo no processo principal
distribui as senhas entre os hosts
*/

FkfcaxTSAzB4g

300


3