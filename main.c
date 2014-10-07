#include "main.h"

#define N 1024
#define W 5
#define MODE 0

void substr(char *buf, char *str, int s, int l)
{
  strcpy(buf, str+s);
  buf[l] = '\0';
};

int main(void)
{

  char str[N] = "hoge hoge hgoe";
  char qry[N] = "hoge hoge hoge";

  switch(MODE){
  case 0: strcpy(str, "201020103"); break;
  case 1: strcpy(str, "mmiissiissiippii"); break;
  case 2: strcpy(str, "abracadabra"); break;
  }
  strcpy(qry, str);

  char buf[N];
  int min;
  int i, j, k;
  ui n, *s, m, *q;
  isais *a;

  /* string -> ary */
  n = strlen(str);
  m = strlen(qry);
  s = (ui *)calloc(n, sizeof(ui));
  q = (ui *)calloc(m, sizeof(ui));
  min = str[0];
  for(i=1; i<n; min = min < str[i] ? min : str[i], i++);
  for(i=0; i<n; min = min < qry[i] ? min : qry[i], i++);
  for(i=0; i<n; s[i] = str[i] - min + 1, i++);
  for(i=0; i<m; q[i] = qry[i] - min + 1, i++);

  /* string & query */
  printf("s = \"%s\" :", str);
  for(i=0; i<n; i++) printf("%5d", s[i]); printf(" : %d\n", n);
  printf("q = \"%s\" :", qry);
  for(i=0; i<m; i++) printf("%5d", q[i]); printf(" : %d\n", m);

  /* suffix array */
  a = isais_new(n, s);
  for(i=0; i<=n; i++){
    j = a->a[i];
    substr(buf, str, j, n-j);
    printf("%3d [%3d] : \"%s\"\n", i, j, buf);
  }

  /* match */
  for(i=0; i<m-W+1; i++){
  substr(buf, qry, i, W);
  j = isais_match(a, W, &q[i]);
  k = isais_search(a, W, &q[i]);
  printf("%3d : match \"%s\" at %3d (%3d)\n", i, buf, j, k);
  }

  /* free */
  free(s);
  free(q);
  isais_free(a);

  return 0;
}
