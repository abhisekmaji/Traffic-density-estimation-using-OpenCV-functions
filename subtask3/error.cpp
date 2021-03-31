#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv) {
  // Create and open a text file
  string filename2 = argv[1];
  string filename1 ="base.txt";
  filename2 ="m"+filename2+".txt";
  string myText1;
  string myText2;
  double error= 0.0;
  int count = 0;
  ifstream MyFile1(filename1);
  ifstream MyFile2(filename2);
    while (getline(MyFile1, myText1) && getline(MyFile2, myText2)){
        error+= abs(stod(myText1)-stod(myText2));
        count++;
    }
  MyFile1.close();
  MyFile2.close();
  double mean_error= error/(count*1.0);
  cout<<"absoulte mean error = "<<mean_error<<endl;
}
