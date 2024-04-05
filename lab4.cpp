#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<vector<double> >
divided_difference_table (const vector<double> &x, const vector<double> &y)
{
  int n = x.size ();
  vector<vector<double> > table (n, vector<double> (n));

  for (int i = 0; i < n; ++i)
    {
      table[i][0] = y[i];
    }

  for (int j = 1; j < n; ++j)
    {
      for (int i = 0; i < n - j; ++i)
        {
          table[i][j]
              = (table[i + 1][j - 1] - table[i][j - 1]) / (x[i + j] - x[i]);
        }
    }

  return table;
}

double
newton_interpolation (const vector<double> &x,
                      const vector<vector<double> > &table, double x_value)
{
  int n = x.size ();
  double result = table[0][0];

  for (int i = 1; i < n; ++i)
    {
      double term = table[0][i];
      for (int j = 0; j < i; ++j)
        {
          term *= (x_value - x[j]);
        }
      result += term;
    }

  return result;
}

int
main ()
{
  int numPoints;
  cout << "Enter the number of points: ";
  cin >> numPoints;

  vector<double> x (numPoints), y (numPoints);

  ifstream infile ("in.txt");
  if (!infile)
    {
      cerr << "Error opening file!" << endl;
      return 1;
    }

  for (int i = 0; i < numPoints; i++)
    {
      infile >> x[i];
    }
  for (int i = 0; i < numPoints; i++)
    {
      infile >> y[i];
    }
  infile.close ();

  double targetX;
  cout << "Enter the value of x to evaluate: ";
  cin >> targetX;

  cout << "Data points:" << endl;
  for (int i = 0; i < numPoints; i++)
    {
      cout << "(" << x[i] << ", " << y[i] << ")" << endl;
    }
  vector<vector<double> > table = divided_difference_table (x, y);
  double result = newton_interpolation (x, table, targetX);

  cout << "N(" << targetX << ") = " << result << endl;

  return 0;
}