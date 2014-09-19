#include "main.h"

#define N 1024

void substr(char *buf, char *str, int s, int l)
{
  strcpy(buf, str+s);
  buf[l] = '\0';
};

int main(void)
{
  char str[N] = "successfully accessed";
  char qry[N] = "ss";
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

  /* string & query */
  printf("s = \"%s\" (%d)\n", str, n);
  printf("q = \"%s\" (%d)\n", qry, m);

  /* suffix array */
  a = isais_new(n, s);
  for(i=0; i<=n; i++){
    j = a->a[i];
    substr(buf, str, j, n-j);
    printf("%3d [%3d] : \"%s\"\n", i, j, buf);
  }

  /* match */
  i = isais_match(a, m, q);
  printf("\nq = \"%s\" matches at %d in s = \"%s\".\n", qry, i, str);

  /* free */
  free(s);
  free(q);
  isais_free(a);

  return 0;
}
