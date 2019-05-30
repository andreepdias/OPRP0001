#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

int main(void)
{
  char salt[] = "Fk";
  char *password;

  for(int i = 0; i < 26; i++){
    
    password = crypt((char)(i + 'a'), salt);
    printf("%s\n", password);
  }


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