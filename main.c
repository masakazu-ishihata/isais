#include "main.h"

#define N 1024

void substr(char *buf, char *str, int s, int l)
{
  strcpy(buf, str+s);
  buf[l] = '\0';
};

int main(void)
{
  char str[N] = "hoge hoge hoge";
  char qry[N] = "e h";
  char buf[N];
  int min = 0;
  int i, j;
  ui n, *s, m, *q;
  isais *a;

  /* string -> ary */
  n = strlen(str);
  m = strlen(qry);
  s = (ui *)calloc(n, sizeof(ui));
  q = (ui *)calloc(m, sizeof(ui));

  for(min=s[0], i=1; i<n; min = min < s[i] ? min : s[i], i++);
  for(i=0; i<n; s[i] = str[i] - min + 1, i++);
  for(i=0; i<m; q[i] = qry[i] - min + 1, i++);

  printf("s = \"%s\"\n", str);
  printf("q = \"%s\"\n", qry);

  a = isais_new(n, s);
  for(i=0; i<=n; i++){
    j = a->a[i];
    substr(buf, str, j, n-j);
    printf("%3d [%3d] : \"%s\"\n", i, j, buf);
  }

  i = isais_match(a, m, q);
  printf("First appearance of \"%s\" in \"%s\" is at %d.\n", qry, str, i);

  return 0;
}