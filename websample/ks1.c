#define KS1
#include "ks1-a.h"
/* 1 */ 
#define max ( 10000000L ) 
integer bound, i  ;
integer iptd[max + 1], ipt[max + 1]  ;
integer ipnum, ipmax, m  ;
integer j  ;
integer k  ;

#include "ks1.h"
integer 
#ifdef HAVE_PROTOTYPES
zsqrt ( integer n ) 
#else
zsqrt ( n ) 
  integer n ;
#endif
{
  /* 1 */ register integer Result; sqrt_regmem 
  if ( n <= 1 ) 
  Result = n ;
  else {
      register integer for_end; i = 1 ;for_end = max ; if ( i <= for_end) 
  do 
    if ( i * i > n ) 
    {
      Result = i - 1 ;
      goto lab1 ;
    } 
  while ( i++ < for_end ) ;} 
  lab1: ;
  return Result ;
} 
boolean 
#ifdef HAVE_PROTOTYPES
zlqf3 ( integer n ) 
#else
zlqf3 ( n ) 
  integer n ;
#endif
{
  /* 1 */ register boolean Result; lqf3_regmem 
  Result = true ;
  j = n ;
  k = 0 ;
  {register integer for_end; m = 2 ;for_end = ipnum ; if ( m <= for_end) do 
    {
      if ( j < ipt [m ]) 
      {
	if ( k < 3 ) 
	Result = false ;
	goto lab1 ;
      } 
      if ( j % ipt [m ]== 0 ) 
      {
	j = j / ipt [m ];
	k = k + 1 ;
	if ( ( j % ipt [m ]== 0 ) || ( ( n - 1 ) % ( ipt [m ]- 1 ) != 0 ) 
	) 
	{
	  Result = false ;
	  goto lab1 ;
	} 
      } 
    } 
  while ( m++ < for_end ) ;} 
  if ( j > 1 ) 
  {
    k = k + 1 ;
    if ( ( n - 1 ) % m != 0 ) 
    Result = false ;
  } 
  if ( k < 3 ) 
  Result = false ;
  lab1: ;
  return Result ;
} 
void mainbody() {
    
  readln ( bound ) ;
  if ( bound <= 2 ) 
  {
    fprintf (stdout, "%s\n", "nothing to compute." ) ;
    goto lab1 ;
  } 
  else if ( bound > max ) 
  {
    fprintf (stdout, "%s%ld%c\n", "too large bound. I change bound to " , (long)max , '.' ) ;
    bound = max ;
  } 
  m = sqrt ( bound ) + 1 ;
  {register integer for_end; i = 1 ;for_end = bound ; if ( i <= for_end) do 
    iptd [i ]= 0 ;
  while ( i++ < for_end ) ;} 
  iptd [1 ]= 1 ;
  ipnum = 0 ;
  ipmax = m ;
  {register integer for_end; i = 2 ;for_end = sqrt ( m ) + 1 ; if ( i <= 
  for_end) do 
    if ( iptd [i ]== 0 ) 
    {
      ipnum = ipnum + 1 ;
      {register integer for_end; j = 2 ;for_end = m / i ; if ( j <= for_end) 
      do 
	iptd [j * i ]= 1 ;
      while ( j++ < for_end ) ;} 
    } 
  while ( i++ < for_end ) ;} 
  {register integer for_end; i = sqrt ( m ) + 2 ;for_end = m ; if ( i <= 
  for_end) do 
    if ( iptd [i ]== 0 ) 
    ipnum = ipnum + 1 ;
  while ( i++ < for_end ) ;} 
  i = 1 ;
  {register integer for_end; j = 2 ;for_end = m ; if ( j <= for_end) do 
    if ( iptd [j ]== 0 ) 
    {
      ipt [i ]= j ;
      i = i + 1 ;
    } 
  while ( j++ < for_end ) ;} 
  {register integer for_end; i = 3 ;for_end = bound ; if ( i <= for_end) do 
    if ( odd ( i ) ) 
    if ( lqf3 ( i ) ) 
    {
      fprintf (stdout, "%s%ld\n", "" , (long)i ) ;
    } 
  while ( i++ < for_end ) ;} 
  lab1: ;
} 
