#include <stdio.h>
#include <unistd.h>
#include <string>
#include <crypt.h>
using namespace std;

int main(void)
{
  char salt[] = "Fk";
  char *password;

  string p = "a";
  password = crypt(p.c_str(), salt);
  printf("%s\n", password);
  
}


/*
le arquivo no processo principal
distribui as senhas entre os hosts
*/

