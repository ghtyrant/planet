#ifndef PERLINNOISE_H
#define PERLINNOISE_H


class PerlinNoise
{
  public:
    PerlinNoise(double persistence, double frequency, double amplitude, int octaves, int seed);
    virtual ~PerlinNoise();

    double get(double x, double y) const;

  protected:
  private:
    double total(double i, double j) const;
    double getValue(double x, double y) const;
    double interpolate(double x, double y, double a) const;
    double noise(int x, int y) const;

    double persistence_;
    double frequency_;
    double amplitude_;
    int octaves_;
    int seed_;
};

#endif // PERLINNOISE_H
