#include "isais.h"

/*------------------------------------*/
/* new */
/*------------------------------------*/
isais *isais_new(ui _n, ui *_s)
{
  int i;
  isais *_a = (isais *)malloc(sizeof(isais));

  /* init string */
  _a->n = _n + 1;
  _a->s = (ui *)calloc(_a->n, sizeof(ui));
  for(i=0; i<_n; i++) _a->s[i] = _s[i];

  /* init # charactors */
  _a->m = 0;
  for(i=0; i<_a->n; i++)
    if(_a->m < _a->s[i]) _a->m = _a->s[i];
  _a->m++;

  /* init suffix array */
  _a->a = (ui *)calloc(_a->n, sizeof(ui));
  for(i=0; i<_a->n; i++) _a->a[i] = _a->n + 1; /* default */

  /* init count of each charactor */
  _a->c = (ui *)calloc(_a->m, sizeof(ui));
  for(i=0; i<_a->n; i++) _a->c[ _a->s[i] ]++;

  /* init bucket pointer */
  _a->p = (ui *)calloc(_a->m, sizeof(ui));

  /* type of each suffix */
  _a->t = (ui *)calloc(_a->n, sizeof(ui));
  for(i=_a->n-2; i>=0; i--){
    if     (_a->s[i] < _a->s[i+1]) _a->t[i] = 0;
    else if(_a->s[i] > _a->s[i+1]) _a->t[i] = 1;
    else                           _a->t[i] = _a->t[i+1];
  }

  /* induced sort */
  isais_fill_sorted_LMS(_a); /* sorted LMS-index -> a */
  isais_fill_Ltype(_a);      /* L-type -> a */
  isais_fill_Stype(_a);      /* S-type -> a */

  /* update pointer for search */
  for(_a->p[0]=0, i=1; i<_a->m; _a->p[i]=_a->p[i-1]+_a->c[i-1], i++);

  return _a;
}
/*------------------------------------*/
/* Is s[_i:] an LMS ? */
/*------------------------------------*/
int isais_is_LMS(isais *_a, int _i)
{
  return _i > 0 && _a->t[_i-1] == 1 && _a->t[_i] == 0;
}
/*------------------------------------*/
/* fill */
/*------------------------------------*/
void isais_fill_sorted_LMS(isais *_a)
{
  int i, j;
  ui n=0, *l; /* sorted LMS */

  /* init list of sorted LMSs */
  for(i=0; i<_a->n; i++) if( isais_is_LMS(_a, i) ) n++;
  l = (ui *)calloc(n, sizeof(ui));

  /* induced sort */
  isais_fill_LMS(_a);
  isais_fill_Ltype(_a);
  isais_fill_Stype(_a);

  /* list of sorted LMSs */
  for(j=0, i=_a->n-1; i>=0; i--)
    if( isais_is_LMS(_a, _a->a[i]) ) l[j++] = _a->a[i];

  /* fill sorted LMS */
  for(i=0; i<_a->n; i++) _a->a[i] = _a->n+1;
  for(_a->p[0]=0, i=1; _a->p[i]=_a->p[i-1]+_a->c[i], i<_a->m; i++); /* reset p */
  for(i=0; i<n; i++) _a->a[ _a->p[_a->s[l[i]]]-- ] = l[i];
  free(l);
}
void isais_fill_LMS(isais *_a)
{
  int i;
  for(_a->p[0]=0, i=1; i<_a->m; _a->p[i]=_a->p[i-1]+_a->c[i], i++); /* rest p */
  for(i=1; i<_a->n; i++)
    if( isais_is_LMS(_a, i) ) _a->a[ _a->p[_a->s[i]]-- ] = i;
}
void isais_fill_Ltype(isais *_a)
{
  int i, j, k;
  for(_a->p[0]=0, i=1; i<_a->m; _a->p[i]=_a->p[i-1]+_a->c[i-1], i++); /* rest p */
  for(i=0; i<_a->n; i++){
    j = _a->a[i];    /* index */
    if(j == _a->n+1 || j-1 < 0 || _a->t[j-1] == 0) continue;
    k = _a->s[j-1];  /* prefix charactor */
    _a->a[ _a->p[k]++ ] = j-1;
  }
}
void isais_fill_Stype(isais *_a)
{
  int i, j, k;
  for(_a->p[0]=0, i=1; i<_a->m; _a->p[i]=_a->p[i-1]+_a->c[i], i++); /* rest p */
  for(i=_a->n-1; i>=0; i--){
    j = _a->a[i];   /* index */
    if(j == _a->n+1 || j-1 < 0 || _a->t[j-1] == 1) continue;
    k = _a->s[j-1]; /* prefix charactor */
    _a->a[ _a->p[k]-- ] = j-1;
  }
}

/*------------------------------------*/
/* free */
/*------------------------------------*/
void isais_free(void *_p)
{
  isais *_a = (isais *)_p;
  free(_a->s);
  free(_a->a);
  free(_a->t);
  free(_a->c);
  free(_a->p);
  free(_a);
}

/*------------------------------------*/
/* show */
/*------------------------------------*/
void isais_show_s(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", i);
  fprintf(_fp, "\n");
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", _a->s[i]);
  fprintf(_fp, "\n");
}
void isais_show_a(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->n; i++){
    if(_a->a[i] == _a->n + 1)
      fprintf(_fp, "    *");
    else
      fprintf(_fp, "%5d", _a->a[i]);
  }
  fprintf(_fp, "\n");
}
void isais_show_t(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", _a->t[i]);
  fprintf(_fp, "\n");
}
void isais_show_c(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->m; i++) fprintf(_fp, "%5d", _a->c[i]);
  fprintf(_fp, "\n");
}
void isais_show(FILE *_fp, isais *_a)
{
  int i, j;
  fprintf(_fp, "n = %d\n", _a->n);
  fprintf(_fp, "m = %d\n", _a->m);
  for(i=0; i<_a->n; i++){
    fprintf(_fp, "[%5d] %5d :", i, _a->a[i]);
    for(j=_a->a[i]; j<_a->n; j++){
      fprintf(_fp, "%5d", _a->s[j]);
    }
    fprintf(_fp, "\n");
  }
}

/*------------------------------------*/
/* search */
/*------------------------------------*/
/* find smallest i s.t. s[i:len(q)] == q */
int isais_match(isais *_a, ui _n, ui *_s)
{
  int i, j, min = -1;

  for(i=isais_search(_a, _n, _s); i<_a->n; i++){
    j = _a->a[i];
    if(_a->n-i < _n) break;
    if(arycmp(_n, _s, _n , &(_a->s[j])) == 0 && (min == -1 || min > j))
      min = j;
  }
  return min;
}
/* find i s.t. s[a[i-1]:] < q <= s[a[i]:] */
int isais_search(isais *_a, ui _n, ui *_s)
{
  if(_n < 1) return 0;
  if(_s[0] >= _a->m) return _a->n;

  //  int s = _a->p[ _s[0] ];
  //  int t = _a->p[ _s[0] ] + _a->c[ _s[0] ];
  int s = 0;
  int t = _a->n-1;
  int m, i, c;

  /* binary search */
  while(s < t){
    m = (s + t) / 2;
    i = _a->a[m];
    c = arycmp(_n, _s, _a->n-i, &(_a->s[i]));

    /* final comparison */
    if(s + 1 == t){
      switch(c){
      case -1: return s;
      case +1: return t;
      }
    }

    /* others */
    switch(c){
    case -1: t = m; break;
    case +1: s = m; break;
    }
  }

  /* s == t must hold */
  return s;
}

/*------------------------------------*/
/* util */
/*------------------------------------*/
int arycmp(ui n, ui *a, ui m, ui *b)
{
  int i, c;

  for(i=0; i<min(n, m); i++){
    if((c = cmp(a[i], b[i])) == 0) continue;
    return c;
  }
  return cmp(n, m);
}
