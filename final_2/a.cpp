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
  for(int i = 0; i < 26; i++){
    p[0] = (char)(i + 'a');
    password = crypt(p.c_str(), salt);
    printf("%s\n", password);
  }


  puts(password);
  return 0;
}

/*
le arquivo no processo principal
distribui as senhas entre os hosts
*/

