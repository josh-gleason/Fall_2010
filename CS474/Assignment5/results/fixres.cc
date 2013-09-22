#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
  if ( argc <= 2 )
    return 0;

  ifstream fin(argv[1]);
  ofstream fout(argv[2]);
  ofstream fout2;
  
  if ( argc > 2 )
      fout2.open(argv[3]);

  string temp;
  char garb;

  int err, wave, coefs;
  float mse, perc;

  float** mat = new float*[8];

  for ( int i = 0; i < 8; i++ )
    mat[i] = new float[11];

  while ( fin.good() )
  {
    fin >> temp;
    if ( !fin.good() )
      break;

    fin >> err >> temp >> garb >> wave >> temp >> temp
        >> temp >> mse >> temp >> perc >> garb >> garb >> coefs >> temp;

    mat[err / 5 - 1][wave/2-1] = perc;
  }

  for ( int x = 0; x < 8; x++ )
  {
    for ( int y = 0; y < 11; y++ )
      fout << mat[x][y] << ' ';
    fout << endl;
  }

  fin.close();
  fout.close();

  return 0;
}
