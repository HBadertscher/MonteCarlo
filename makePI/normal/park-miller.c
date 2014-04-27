
double park_miller_rand(int* seed)
{
  const int a = 16807;
  const int m = 2147483647;
  const int q = 127773;
  const int r = 2836;

  int k=*seed/q;
  *seed = a * (*seed - k*q) - k*r;
  if(*seed < 0) *seed += m;
  return ((double)*seed) / (double)m;
}

