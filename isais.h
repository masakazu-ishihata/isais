#ifndef _ISAIS_H_
#define _ISAIS_H_

/*------------------------------------*/
/* include */
/*------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------*/
/* type */
/*------------------------------------*/
typedef unsigned int ui;

/*------------------------------------*/
/* util */
/*------------------------------------*/
#define min(a, b) (a < b ? a : b)
#define cmp(a, b) (a < b ? -1 : (a == b ? 0 : +1))
int arycmp(ui n, ui *a, ui m, ui *b);

/*------------------------------------*/
/* isais */
/*------------------------------------*/
typedef struct ISAIS
{
  ui  n;    /* string length */
  ui  m;    /* # of charactors */
  ui *s;    /* string */
  ui *t;    /* type of suffix */
  ui *l;    /* LMS-substring : l[i] = j where i is LMS ID and j is the next LMS ID */
  ui *a;    /* suffix array */
  ui *c;    /* count of each charactor */
  ui *p;    /* pointer of each bucket */
} isais;

/* new/free */
isais *isais_new(ui _n, ui *_s);
void isais_free(void *_p);

/* bucket */
void isais_bucket_head(isais *_a);
void isais_bucket_end(isais *_a);

/* propositional functions */
int isais_is_LMS(isais *_a, int _i);
int isais_is_Ltype(isais *_a, int _i);
int isais_is_Stype(isais *_a, int _i);
int isais_comp_LMS_substring(isais *_a, int _i, int _j);

/* fill */
void isais_fill_sorted_LMS_substring(isais *_a);
void isais_fill_sorted_LMS(isais *_a);
void isais_fill_LMS(isais *_a);
void isais_fill_Ltype(isais *_a);
void isais_fill_Stype(isais *_a);


/* show */
void isais_show_s(FILE *_fp, isais *_a);
void isais_show_a(FILE *_fp, isais *_a);
void isais_show_t(FILE *_fp, isais *_a);
void isais_show_l(FILE *_fp, isais *_a);
void isais_show_c(FILE *_fp, isais *_a);
void isais_show(FILE *_fp, isais *_a);

/* search */
int isais_search(isais *_a, ui _n, ui *_s);
int isais_match(isais *_a, ui _n, ui *_s);

#endif
