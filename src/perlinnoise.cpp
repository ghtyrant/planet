#include "perlinnoise.h"

PerlinNoise::PerlinNoise(double persistence, double frequency, double amplitude, int octaves, int seed)
  : persistence_(persistence), frequency_(frequency), amplitude_(amplitude), octaves_(octaves), seed_(seed)
{
}

PerlinNoise::~PerlinNoise()
{
}

double PerlinNoise::get(double x, double y) const
{
  return amplitude_ * total(x, y);
}

double PerlinNoise::total(double i, double j) const
{
  //properties of one octave (changing each loop)
  double t = 0.0f;
  double amplitude = 1;
  double freq = frequency_;

  for(int k = 0; k < octaves_; k++)
  {
    t += getValue(j * freq + seed_, i * freq + seed_) * amplitude;
    amplitude *= persistence_;
    freq *= 2;
  }

  return t;
}

double PerlinNoise::getValue(double x, double y) const
{
  int Xint = (int)x;
  int Yint = (int)y;
  double Xfrac = x - Xint;
  double Yfrac = y - Yint;

  //noise values
  double n01 = noise(Xint-1, Yint-1);
  double n02 = noise(Xint+1, Yint-1);
  double n03 = noise(Xint-1, Yint+1);
  double n04 = noise(Xint+1, Yint+1);
  double n05 = noise(Xint-1, Yint);
  double n06 = noise(Xint+1, Yint);
  double n07 = noise(Xint, Yint-1);
  double n08 = noise(Xint, Yint+1);
  double n09 = noise(Xint, Yint);

  double n12 = noise(Xint+2, Yint-1);
  double n14 = noise(Xint+2, Yint+1);
  double n16 = noise(Xint+2, Yint);

  double n23 = noise(Xint-1, Yint+2);
  double n24 = noise(Xint+1, Yint+2);
  double n28 = noise(Xint, Yint+2);

  double n34 = noise(Xint+2, Yint+2);

  //find the noise values of the four corners
  double x0y0 = 0.0625*(n01+n02+n03+n04) + 0.125*(n05+n06+n07+n08) + 0.25*(n09);
  double x1y0 = 0.0625*(n07+n12+n08+n14) + 0.125*(n09+n16+n02+n04) + 0.25*(n06);
  double x0y1 = 0.0625*(n05+n06+n23+n24) + 0.125*(n03+n04+n09+n28) + 0.25*(n08);
  double x1y1 = 0.0625*(n09+n16+n28+n34) + 0.125*(n08+n14+n06+n24) + 0.25*(n04);

  //interpolate between those values according to the x and y fractions
  double v1  = interpolate(x0y0, x1y0, Xfrac); //interpolate in x direction (y)
  double v2  = interpolate(x0y1, x1y1, Xfrac); //interpolate in x direction (y+1)
  double fin = interpolate(v1, v2, Yfrac);  //interpolate in y direction

  return fin;
}

double PerlinNoise::interpolate(double x, double y, double a) const
{
  double negA = 1.0 - a;
  double negASqr = negA * negA;
  double fac1 = 3.0 * (negASqr) - 2.0 * (negASqr * negA);
  double aSqr = a * a;
  double fac2 = 3.0 * aSqr - 2.0 * (aSqr * a);

  return x * fac1 + y * fac2; //add the weighted factors
}

double PerlinNoise::noise(int x, int y) const
{
  int n = x + y * 57;
  n = (n << 13) ^ n;
  int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
  return 1.0 - double(t) * 0.931322574615478515625e-9;
}
