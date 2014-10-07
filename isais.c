#include "isais.h"

/*----------------------------------------------------------------------------*/
/* new/free */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* new */
/*------------------------------------*/
isais *isais_new(ui _n, ui *_s)
{
  int i, j;
  isais *_a = (isais *)malloc(sizeof(isais));

  /* string with terminal $ (= 0) */
  _a->n = _n + 1;
  _a->s = (ui *)calloc(_a->n, sizeof(ui));
  for(i=0; i<_n; i++) _a->s[i] = _s[i];

  /* # charactors */
  _a->m = 0;
  for(i=0; i<_a->n; i++)
    if(_a->m < _a->s[i]) _a->m = _a->s[i];
  _a->m++;

  /* type of each charactor (suffix) */
  _a->t = (ui *)calloc(_a->n, sizeof(ui));
  for(i=_a->n-2; i>=0; i--){
    if     (_a->s[i] < _a->s[i+1]) _a->t[i] = 0;
    else if(_a->s[i] > _a->s[i+1]) _a->t[i] = 1;
    else                           _a->t[i] = _a->t[i+1];
  }

  /* LMS-substrings */
  _a->l = (ui *)calloc(_a->n, sizeof(ui));
  for(j=_a->n-1, i=_a->n; i>=0; i--){
    if( isais_is_LMS(_a, i) ){
      _a->l[i] = j;
      j = i;
    }
  }

  /* suffix array */
  _a->a = (ui *)calloc(_a->n, sizeof(ui));
  for(i=0; i<_a->n; i++) _a->a[i] = _a->n + 1; /* default */
  _a->a[0] = _a->n - 1;

  /* count of each charactor */
  _a->c = (ui *)calloc(_a->m, sizeof(ui));
  for(i=0; i<_a->n; i++) _a->c[ _a->s[i] ]++;

  /* bucket pointer */
  _a->p = (ui *)calloc(_a->m, sizeof(ui));

  /* sort */
  isais_fill_sorted_LMS_substring(_a); /* sorted LMS-index -> a */
  isais_fill_sorted_LMS(_a);           /* sorted LMS -> a */
  isais_fill_Ltype(_a);                /* L-type -> a */
  isais_fill_Stype(_a);                /* S-type -> a */

  /* update pointer for search */
  for(_a->p[0]=0, i=1; i<_a->m; _a->p[i]=_a->p[i-1]+_a->c[i-1], i++);

  return _a;
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
  free(_a->l);
  free(_a->c);
  free(_a->p);
  free(_a);
}

/*----------------------------------------------------------------------------*/
/* bucket pointer p */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* set pointer p to head of each bucket */
/*------------------------------------*/
void isais_bucket_head(isais *_a)
{
  int i;
  for(_a->p[0]=0, i=1; i<_a->m; i++) _a->p[i] = _a->p[i-1] + _a->c[i-1];
}
/*------------------------------------*/
/* set pointer p to end of each bucket */
/*------------------------------------*/
void isais_bucket_end(isais *_a)
{
  int i;
  for(_a->p[0]=0, i=1; i<_a->m; i++) _a->p[i] = _a->p[i-1] + _a->c[i];
}

/*----------------------------------------------------------------------------*/
/* propositional functions */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/*  Is s[_i] an LMS charactor?        */
/*------------------------------------*/
int isais_is_LMS(isais *_a, int _i)
{
  return 0 < _i && _a->t[_i-1] == 1 && _a->t[_i] == 0;
}
/*------------------------------------*/
/* Is s[_i] an L-teype charactor? */
/*------------------------------------*/
int isais_is_Ltype(isais *_a, int _i)
{
  return 0 <= _i && _i < _a->n && _a->t[_i] == 1;
}
/*------------------------------------*/
/* Is s[_i] an S-teype charactor? */
/*------------------------------------*/
int isais_is_Stype(isais *_a, int _i)
{
  return 0 <= _i && _i < _a->n && _a->t[_i] == 0;
}
/*------------------------------------*/
/* compare two LMS-substrings */
/*------------------------------------*/
int isais_comp_LMS_substring(isais *_a, int _i, int _j)
{
  int i, l = _a->l[_i]-_i;

  /* different length */
  if(_a->l[_i]-_i != _a->l[_j]-_j) return -1;

  /* same length */
  for(i=0; i<l; i++){
    if(_a->s[_i+i] != _a->s[_j+i]) return _a->s[_i+i] < _a->s[_j+i];
    if(_a->t[_i+i] != _a->t[_j+i]) return _a->t[_i+i] < _a->t[_j+i];
  }
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fill */
/*----------------------------------------------------------------------------*/
/*------------------------------------*/
/* Fill LMS-substrings with dictionary order */
/*  by induced sorting */
/*------------------------------------*/
void isais_fill_sorted_LMS_substring(isais *_a)
{
  isais_fill_LMS(_a);
  isais_fill_Ltype(_a);
  isais_fill_Stype(_a);
}
/*------------------------------------*/
/* Fill LMSs with oritinal order */
/*------------------------------------*/
void isais_fill_LMS(isais *_a)
{
  int i;

  /* set p to bucket end */
  isais_bucket_end(_a);

  /* fill LMSs from end to head */
  for(i=1; i<_a->n; i++)
    if(_a->l[i] > 0)
      _a->a[ _a->p[_a->s[i]]-- ] = i;
}
/*------------------------------------*/
/* Fill LMSs with dictionary order */
/*------------------------------------*/
void isais_fill_sorted_LMS(isais *_a)
{
  int i, j;
  ui n = 0; /* # of LMS-substrings */
  ui m = 0; /* # of unique LMS-substrings */
  ui *p;    /* array of LMS-substrings with oritinal (positional) order */
  ui *q;    /* array of LMS-substrings with dictionary order */
  ui *d;    /* dictionary d[i] = ID of LMS-substring s[i]... */
  ui *s;    /* shorten string */
  isais *a; /* Suffix array for recursion */

  /* n : count # of LMS substrings */
  for(i=0; i<_a->n; i++) if(_a->l[i] > 0) n++;

  /* p : array of LMS-substings with positional order */
  p = (ui *)calloc(n, sizeof(ui));
  for(i=0, j=0; i<_a->n; i++)
    if(_a->l[i] > 0) p[j++] = i;

  /* q : array of LMS-substrings with dictionary order */
  q = (ui *)calloc(n, sizeof(ui));
  for(i=0, j=0; i<_a->n; i++){
    if(_a->a[i] == _a->n + 1) continue;        /* skip if default value */
    if(_a->l[_a->a[i]] > 0) q[j++] = _a->a[i];
  }

  /* d : dictionary from LMS ID to new ID */
  d = (ui *)calloc(_a->n, sizeof(ui));
  d[_a->n-1] = 1;
  for(m=1, i=1; i<n; i++){
    if(isais_comp_LMS_substring(_a, q[i-1], q[i]) != 0) m++;
    d[ q[i] ] = m;
  }

  /* s : shorten string */
  s = (ui *)calloc(n, sizeof(ui));
  for(i=0; i<n; i++) s[i] = d[ p[i] ];

  /* recursion : sort LMSs */
  if(m < n){
    a = isais_new(n, s);
    for(i=0; i<n; i++) q[i] = p[ a->a[i+1] ];
    isais_free(a);
  }

  /* reset suffix array */
  for(i=0; i<_a->n; i++) _a->a[i] = _a->n+1;

  /* set p to bucket end */
  isais_bucket_end(_a);

  /* fill sorted LMSs from end to head */
  for(i=0; i<n; i++){
    j = n-i-1;
    _a->a[ _a->p[_a->s[ q[j] ]]-- ] = q[j];
  }

  free(p);
  free(q);
  free(d);
  free(s);
}
/*------------------------------------*/
/* Fill L-types */
/*------------------------------------*/
void isais_fill_Ltype(isais *_a)
{
  int i, j;

  /* set p to bucket head */
  isais_bucket_head(_a);

  /* fill L-types from head to end */
  for(i=0; i<_a->n; i++){
    j = _a->a[i];
    if( isais_is_Ltype(_a, j-1) ) _a->a[ _a->p[_a->s[j-1]]++ ] = j-1;
  }
}
/*------------------------------------*/
/* Fill S-types */
/*------------------------------------*/
void isais_fill_Stype(isais *_a)
{
  int i, j;

  /* set p to bucket end */
  isais_bucket_end(_a);

  /* fill S-types from end to head */
  for(i=_a->n-1; i>=0; i--){
    j = _a->a[i];
    if( isais_is_Stype(_a, j-1) ) _a->a[ _a->p[_a->s[j-1]]-- ] = j-1;
  }
}

/*----------------------------------------------------------------------------*/
/* show */
/*----------------------------------------------------------------------------*/
void isais_show(FILE *_fp, isais *_a)
{
  int i, j;
  fprintf(_fp, "String Length = %d\n", _a->n);
  fprintf(_fp, "# charactors  = %d\n", _a->m);
  for(i=0; i<_a->n; i++){
    fprintf(_fp, "[%5d] %5d :", i, _a->a[i]);
    for(j=_a->a[i]; j<_a->n; j++){
      fprintf(_fp, "%5d", _a->s[j]);
    }
    fprintf(_fp, "\n");
  }
}
void isais_show_s(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", i);
  fprintf(_fp, "\n");
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", _a->s[i]);
  fprintf(_fp, " # Original String\n");
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
  fprintf(_fp, " # Suffix Array\n");
}
void isais_show_t(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", _a->t[i]);
  fprintf(_fp, " # Types of charactors\n");
}
void isais_show_l(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->n; i++) fprintf(_fp, "%5d", _a->l[i]);
  fprintf(_fp, " # LMS-substrings\n");
}
void isais_show_c(FILE *_fp, isais *_a)
{
  int i;
  for(i=0; i<_a->m; i++) fprintf(_fp, "%5d", _a->c[i]);
  fprintf(_fp, " # count\n");
}

/*------------------------------------*/
/* search */
/*------------------------------------*/
/* find smallest i s.t. s[i:len(q)] == q */
int isais_match(isais *_a, ui _n, ui *_s)
{
  int i, j, min = -1;
  int n = _a->n;

  if(_a->n < _n) return -1;
  for(i=isais_search(_a, _n, _s); i<n; i++){
    j = _a->a[i];
    if(n-j < _n) break;
    if(arycmp(_n, _s, _n, &(_a->s[j])) != 0) break;
    if(min == -1 || min > j) min = j;
  }
  return min;
}
/* find i s.t. s[a[i-1]:] < q <= s[a[i]:] */
int isais_search(isais *_a, ui _n, ui *_s)
{
  if(_n < 1) return 0;
  if(_s[0] >= _a->m) return _a->n;

  int s = _a->p[ _s[0] ];
  int t = _a->p[ _s[0] ] + _a->c[ _s[0] ];
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
