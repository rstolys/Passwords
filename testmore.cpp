#include <iostream> 

using namespace std;


/////////////////////////////////////////////////////////////////////
/// used for mixColumns multiplcation by two 
/// 
/// @param[in]  x           byte to be modified    
/// 
/// @return     uint8_t     modified result of operations to x  
/////////////////////////////////////////////////////////////////////
static uint8_t x2time(uint8_t x)
{
  bool    xOR_1B = false;

  //If the left most bit is 1
  if(x & 0x80) {

    xOR_1B = true;
  }
  
  //bit shift 1 to left 
  x <<= 1;

  if(xOR_1B) {

    //XOR with 0x1b
    x ^= 0x1b;
  }
  
  return x;

}

/////////////////////////////////////////////////////////////////////
/// used for mixColumns multiplcation by three 
/// 
/// @param[in]  x           byte to be modified    
/// 
/// @return     uint8_t     modified result of operations to x  
/////////////////////////////////////////////////////////////////////
static uint8_t x3time(uint8_t x)
{
  int   y;
  
  //Do multiplcation by 2; 
  y = x2time(x);

  //XOR with result and original byte
  x = x ^ y;

  return x;

}


int main() 
{

    uint8_t  test = 0x47;
    uint8_t  test2 = 0xf7;

    printf( "%x\n", x2time(test) );

    printf( "%x\n", x3time(test2) );

    return 0;
}