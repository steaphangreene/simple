template <class Tp>
static void freadLE(Tp &var, FILE *fl) {
  union { Tp v; char c[sizeof(Tp)]; } data;
  fread(&data.v, 1, sizeof(Tp), fl);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(data.c[i1], data.c[i2]);
    ++i1; --i2;
    }
#endif
  var = data.v;
  };

template <class Tp>
static void freadBE(Tp &var, FILE *fl) {
  union { Tp v; char c[sizeof(Tp)]; } data;
  fread(&data.v, 1, sizeof(Tp), fl);
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(data.c[i1], data.c[i2]);
    ++i1; --i2;
    }
#endif
  var = data.v;
  };
