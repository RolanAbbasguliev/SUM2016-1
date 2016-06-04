/* FILE NAME: T04PERM.C
 * PROGRAMMER: MM3
 * DATE: 04.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */
#include <stdio.h>

#define N 7
int P[N];

void SavePerm( void );
void Go( int Pos );
void Swap( int *A, int *B );
int Parity( void );

int Parity( void )
{
  int i, j, sum = 0;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      if (P[i] > P[i + j])
        sum++;
  if (sum % 2 == 0)
    return 0;
  else
    return 1;
}
void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

void Go( int Pos )
{
  int i;

  if (Pos == N)
  {
    SavePerm();
    return;
  }    
  else
  {  
    for (i = Pos; i < N; i++)
    {      
      Swap(&P[Pos], &P[i]);       
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);    
    }
  } 
}

void SavePerm( void )
{
  FILE *F;
  int i;

  F = fopen("perm.log", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N - 1; i++)
    fprintf(F, "%2i, ", P[i]);
  fprintf(F, "%2i", P[N - 1]);
  if (Parity())
    fprintf(F, " - even\n");
  else 
    fprintf(F, " - odd\n");
  fclose(F);  
}

void main( void )
{
  int i;

  for (i = 0; i < N; i++)
    P[i] = i + 1;

  Go(0);
}