//#define _POSIX_C_SOURCE 200809L /* Pour respecter la norme POSIX.1-2017 */
#include <math.h>         /* Pour sqrt */

int est_premier (unsigned long long int n)
{
  if ((n>1) && (n==2 || n==3 || ((n%2)&&(n%3))))
  {
    unsigned long long int i,y;

    y=sqrt(n)+1;
    i=5;
    while(i<y)
    {
      if (n%i == 0)
        return 0;
      i+=2;
      if (n%i == 0)
        return 0;
      i+=4;
    }
    return 1;
  }
  return 0;
}
