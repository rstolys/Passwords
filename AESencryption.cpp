#include <iostream>
#include <vector>
#include <string>

#include "AESencryption.hpp"

using namespace std;

typedef uint8_t state_t[4][4];


//sbox for subBytes Step
static const uint8_t sbox[256] =
{ //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };


//sboxInv for subBytesInv Step
static const uint8_t sboxInv[256] =
{ //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };



//Rcon for generting Key Schedule
static const uint8_t Rcon[11] =
{ 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
  //element [0] is not used

//Default Key
static const uint8_t defaultKey_gv[16] =
{ 0x2b, 0x28, 0xab, 0x09,
  0x7e, 0xae, 0xf7, 0xcf,
  0x15, 0xd2, 0x15, 0x4f,
  0x16, 0xa6, 0x88, 0x3c };


//Functions to Generation encrpytion
void subBytes(uint8_t state_t[4][4]);
void shiftRows(uint8_t state_t[4][4]);
void mixColumns(uint8_t state_t[4][4]);
void addRoundKey(uint8_t state_t[4][4], uint8_t RoundKey[4][4]);


//Functions for deencrpytion
void subBytesInv(uint8_t state_t[4][4]);
void shiftRowsInv(uint8_t state_t[4][4]);
void mixColumnsInv(uint8_t state_t[4][4]);
void addRoundKeyInv(uint8_t state_t[4][4], uint8_t RoundKey[4][4]);


//Functions to generate round Keys
uint8_t nextKey(uint8_t RoundKey[4][4]);
void initalKey( char *initalizer, uint8_t newKey[4][4]);

//Main functions
void encrpyt(char password[16], char *ID);


/////////////////////////////////////////////////////////////////////
///
/// Functions to Generation encrpytion
///
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/// Implements subBytes Step
///
/// @param[in/out]  state_t     16 byte message to by encrpyted
///
/////////////////////////////////////////////////////////////////////
void subBytes(uint8_t state_t[4][4])
{
    int i,j;

    for(i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++)
        {

            //Determine the new value of the element
            state_t[i][j] = sbox[ state_t[i][j] ];
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////
/// Implements shiftRows Step
///
/// @param[in/out]  state_t     16 byte message to by encrpyted
///
/////////////////////////////////////////////////////////////////////
void shiftRows(uint8_t state_t[4][4])
{
  uint8_t tmp;

    //No changes to row 0

    //Move all row 1 elements left 1 position
    tmp           = state_t[1][0];
    state_t[1][0] = state_t[1][1];
    state_t[1][1] = state_t[1][2];
    state_t[1][2] = state_t[1][3];
    state_t[1][3] = tmp;

    //Move all row 2 elements 2 positions
    tmp           = state_t[2][0];
    state_t[2][0] = state_t[2][2];
    state_t[2][2] = tmp;

    tmp           = state_t[2][1];
    state_t[2][1] = state_t[2][3];
    state_t[2][3] = tmp;

    //Move all row 3 elements right 1 positon
    tmp           = state_t[3][0];
    state_t[3][0] = state_t[3][3];
    state_t[3][3] = state_t[3][2];
    state_t[3][2] = state_t[3][1];
    state_t[3][1] = tmp;

  return;
}


/////////////////////////////////////////////////////////////////////
/// Implements mixColumns Step
///
/// @param[in/out]  state_t     16 byte message to by encrpyted
///
/////////////////////////////////////////////////////////////////////
void mixColumns(uint8_t state_t[4][4])
{
    //Involves a matrix multiplcation of each column with the following matrix
                /******************
                *  2   3   1   1  *
                *  1   2   3   1  *
                *  1   1   2   3  *
                *  3   1   1   2  *
                *******************/

  uint8_t   state_copy[4];    //Copy for state matrix
  uint8_t   state_x2[4];     //Each element multiplied by 2
                              //NOTE: a[n] ^ b[n] is that element multiplied by 3
  uint8_t   high_bit;         //indicates high bit of byte

  int       i, j;

  /*
   * high_bit is 0xff if the high bit of state_t[i] is set, 0 otherwise
   *
   * arithmetic right shift brings in either zeros or ones
   * implicitly removes high bit because state_x2[i] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line
   *
   *
   *
  */

  //For each column
  for(j = 0; j < 4; j++) {

    for (i = 0; i < 4; i++) {

      //Copy current state matrix
      state_copy[i] = state_t[i][j];

      //If high bit is set, make high_bit = 1111 1111 other wise = 0000 0000
      high_bit = state_t[i][j] & 0x80 ? 0xff : 0x00;

      //left shift all bits for multiplication by 2
      state_x2[i] = state_t[i][j] << 1;

      //XOR with 0x1b if high bit is set
      state_x2[i] ^= 0x1b & high_bit;
    }

    //Set each element of the column

    //                2* a0     +      a3       +     a2        +          3 * a1
    state_t[0][j] = state_x2[0] ^ state_copy[3] ^ state_copy[2] ^ (state_x2[1] ^ state_copy[1]);

    //                2* a1     +      a0       +     a3        +          3 * a2
    state_t[1][j] = state_x2[1] ^ state_copy[0] ^ state_copy[3] ^ (state_x2[2] ^ state_copy[2]);

    //                2* a2     +      a1       +     a0        +          3 * a3
    state_t[2][j]= state_x2[2] ^ state_copy[1] ^ state_copy[0] ^ (state_x2[3] ^ state_copy[3]);

    //                2* a3     +      a2       +     a1        +          3 * a0
    state_t[3][j] = state_x2[3] ^ state_copy[2] ^ state_copy[1] ^ (state_x2[0] ^ state_copy[0]);

    //Reset high bit
    high_bit = 0x00;

  }
}

/////////////////////////////////////////////////////////////////////
/// Implements addRoundKey Step
///
/// @param[in]      RoundKey    Key to modify message
/// @param[in/out]  state_t     16 byte message to by encrpyted
///
/////////////////////////////////////////////////////////////////////
void addRoundKey(uint8_t state_t[4][4], uint8_t RoundKey[4][4])
{
  int i,j;

  for(i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++)
      {

          //Determine the new value of the element -> exclusive or each element
          state_t[i][j] ^= RoundKey[i][j];
      }
  }

  return;
}


/////////////////////////////////////////////////////////////////////
///
/// Functions for deencrpytion
///
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/// Implements subBytesInv Step
///
/// @param[in/out]  state_t     16 byte message to by decrpyted
///
/////////////////////////////////////////////////////////////////////
void subBytesInv(uint8_t state_t[4][4])
{
  int i,j;

  for(i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++)
      {

          //Determine the new value of the element
          state_t[i][j] = sboxInv[ state_t[i][j] ];
      }
  }

  return;
}

/////////////////////////////////////////////////////////////////////
/// Implements shiftRowsInv Step
///
/// @param[in/out]  state_t     16 byte message to by decrpyted
///
/////////////////////////////////////////////////////////////////////
void shiftRowsInv(uint8_t state_t[4][4])
{
  uint8_t tmp;

  //No changes to row 0

  //Move all row 1 elements right 1 position
  tmp           = state_t[1][3];
  state_t[1][3] = state_t[1][2];
  state_t[1][2] = state_t[1][1];
  state_t[1][1] = state_t[1][0];
  state_t[1][0] = tmp;

  //Move all row 2 elements 2 positions
  tmp           = state_t[2][0];
  state_t[2][0] = state_t[2][2];
  state_t[2][2] = tmp;

  tmp           = state_t[2][1];
  state_t[2][1] = state_t[2][3];
  state_t[2][3] = tmp;

  //Move all row 3 elements left 1 positon
  tmp           = state_t[3][0];
  state_t[3][0] = state_t[3][1];
  state_t[3][1] = state_t[3][2];
  state_t[3][2] = state_t[3][3];
  state_t[3][3] = tmp;


}

/////////////////////////////////////////////////////////////////////
/// Implements mixColumns Step
///
/// @param[in/out]  state_t     16 byte message to by decrpyted
///
/////////////////////////////////////////////////////////////////////
void mixColumnsInv(uint8_t state_t[4][4])
{
   //Involves a matrix multiplcation of each column with the following matrix
                /******************
                *  0e  0b  0d  09  *
                *  09  0e  0b  0d  *
                *  0d  09  0e  0b  *
                *  0b  0d  09  0e  *
                *******************/
  int i, j;

  uint8_t   state_copy[4];    //Copy for state matrix

  //For each column
  for(j = 0; j < 4; j++) {

    //For each byte in each column
    for(i = 0; i < 4; i++) {

      //Copy current state matrix
      state_copy[i] = state_t[i][j];

    }

    //Set all the elements of the column

    state_t[0][j] = x14time(state_copy[0]) ^ x11time(state_copy[1]) ^ x13time(state_copy[2]) ^ x9time(state_copy[3]);

    state_t[1][j] = x9time(state_copy[0]) ^ x14time(state_copy[1]) ^ x11time(state_copy[2]) ^ x13time(state_copy[3]);

    state_t[2][j] = x13time(state_copy[0]) ^ x9time(state_copy[1]) ^ x14time(state_copy[2]) ^ x11time(state_copy[3]);

    state_t[3][j] = x11time(state_copy[0]) ^ x13time(state_copy[1]) ^ x9time(state_copy[2]) ^ x14time(state_copy[3]);

  }
}

/////////////////////////////////////////////////////////////////////
///  used for mixColumns and mixColumnsInv multiplcation by two
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
/// used for mixColumns and mixColumnsInv multiplcation by three
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

/////////////////////////////////////////////////////////////////////
/// used for mixColumns and mixColumnsInv multiplcation by nine
///
/// @param[in]  x           byte to be modified
///
/// @return     uint8_t     modified result of operations to x
/////////////////////////////////////////////////////////////////////
static uint8_t x9time(uint8_t x)
{
  // 𝑥×9=(((𝑥×2)×2)×2)+𝑥

  int   i;

  //Copy value
  int   y = x;

  //Do multiplcation by 2, 3 times
  for(i = 0; i < 3; i++) {

    //Multiply by 2
    y = x2time(y);
  }

  //XOR with result and original byte
  x = x ^ y;

  return x;
}

/////////////////////////////////////////////////////////////////////
/// used for mixColumns and mixColumnsInv multiplcation by eleven
///
/// @param[in]  x           byte to be modified
///
/// @return     uint8_t     modified result of operations to x
/////////////////////////////////////////////////////////////////////
static uint8_t x11time(uint8_t x)
{
  // 𝑥×11=((((𝑥×2)×2)+𝑥)×2)+𝑥

  int   i;

  //Copy value
  int   y = x;

  //Do multiplcation by 2, 2 times
  y = x2time(y);
  y = x2time(y);

  //XOR with original value
  y = y ^ x;

  //Multiply by 2 again
  y = x2time(y);

  //XOR with result and original byte
  x = x ^ y;

  return x;
}

/////////////////////////////////////////////////////////////////////
/// used for mixColumns and mixColumnsInv multiplcation by thirteen
///
/// @param[in]  x           byte to be modified
///
/// @return     uint8_t     modified result of operations to x
/////////////////////////////////////////////////////////////////////
static uint8_t x13time(uint8_t x)
{
  // 𝑥×13=((((𝑥×2)+𝑥)×2)×2)+𝑥

  int   i;

  //Copy value
  int   y = x;

  //Multiply by 2
  y = x2time(y);

  //XOR with original byte
  y = y ^ x;

  //Do multiplcation by 2, 2 times
  y = x2time(y);
  y = x2time(y);

  //XOR with result and original byte
  x = x ^ y;

  return x;
}


/////////////////////////////////////////////////////////////////////
/// used for mixColumns and mixColumnsInv multiplcation by fourteen
///
/// @param[in]  x           byte to be modified
///
/// @return     uint8_t     modified result of operations to x
/////////////////////////////////////////////////////////////////////
static uint8_t x14time(uint8_t x)
{
  // 𝑥×14=((((𝑥×2)+𝑥)×2)+𝑥)×2

  int   i;

  //Copy value
  int   y = x;

  //Multiply by 2
  y = x2time(y);

  //XOR with original byte
  y = y ^ x;

  //Do multiplcation by 2, 2 times
  y = x2time(y);

  //XOR with result and original byte
  y = y ^ x;

  //Multiply by 2
  x = x2time(y);

  return x;
}

/////////////////////////////////////////////////////////////////////
/// Implements addRoundKey Step
///
/// @param[in]      RoundKey    Key to modify message
/// @param[in/out]  state_t     16 byte message to by decrpyted
///
/// NOTE: This function is the same as the encryption since it only
///       involves the XOR operation
/////////////////////////////////////////////////////////////////////
void addRoundKeyInv(uint8_t state_t[4][4], uint8_t RoundKey[4][4])
{
  int i,j;

  for(i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++)
      {

          //Determine the new value of the element -> exclusive or each element
          state_t[i][j] ^= RoundKey[i][j];
      }
  }

  return;
}



/////////////////////////////////////////////////////////////////////
///
/// Functions to generate round Keys
///
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
/// Generates next round key
///
/// @param[in]  RoundKey    Last Round Key
/// @param[in]  roundNum    round number of operation
/// @param[out] newKey      Next Round Key
///
/////////////////////////////////////////////////////////////////////
void nextKey( uint8_t currentKey[4][4], uint8_t newKey[4][4], int roundNum)
{
  unsigned i, j, k;
  uint8_t temp[4]; // Used for the column/row operations


  //rotWord () -> shifts all elements up one location
  temp[0] = currentKey[1][3];
  temp[1] = currentKey[2][3];
  temp[2] = currentKey[3][3];
  temp[3] = currentKey[0][3];


  //subBytes() -> will input all values into sBox()
  for(i = 0; i < 4; i++) {

    temp[i] = sbox[ temp[i] ];
  }

  //First element gets XOR with Rcon (round constant)
  temp[0] ^= Rcon[roundNum];



  //Generate newKey
  for(i = 0; i < 4; i++) {

    //XOR with element in previous key
    newKey[i][0]  = temp[i] ^ currentKey[i][0];
    newKey[i][1]  = newKey[i][0] ^ currentKey[i][1];
    newKey[i][2]  = newKey[i][1] ^ currentKey[i][2];
    newKey[i][3]  = newKey[i][2] ^ currentKey[i][3];

  }

  return;
}

/////////////////////////////////////////////////////////////////////
/// Generates next inital key
///
/// @param[in]  initalizer  name for key use
/// @param[out] newKey      inital Key
///
/////////////////////////////////////////////////////////////////////
void initalKey( char *initalizer, uint8_t newKey[4][4])
{
  int       length;
  uint8_t   initKey[16];

  int       i, j;

  //Determine the length of the initalizer
  length = strlen( initalizer );

  //For the first 16 elements or the entire initalizer
  for(i = 0; i < 16 && i < length; i++) {

    //Copy each element into initKey
    initKey[i] = initalizer[i] & 0xff;
  }

  //If the key is not full
  if(16 > i) {

    //Until the initKey if full
    for( ; i < 16; i++) {

      //Enter in preset elements
      initKey[i] = defaultKey_gv[i] & 0xff;
    }
  }

  // Copy initKey into inital Key
  for(i = 0; i < 16; i++) {

    newKey[ i / 4 ][ i % 4 ] = initKey[i];
  }

  //inital key is set
  return;
}


/////////////////////////////////////////////////////////////////////
/// Generates inital password in plaintext
///
/// @param[out]   state_t    inital state of Password
/// @param[in]   password   plaintest Password
///
/////////////////////////////////////////////////////////////////////
void setState(uint8_t state_t[4][4], char password[17])
{
  int       length;
  uint8_t   initState[16];

  int       i, j;

  //Determine the length of the initalizer
  length = strlen( password );

  //For the first 16 elements or the entire initalizer
  for(i = 0; i < 16 && i < length; i++) {

    //Copy each element into initKey
    initState[i] = password[i] & 0xff;
  }


  //If the key is not full
  if(16 > i) {

    //Until the initKey if full
    for( ; i < 16; i++) {

      //Enter in preset elements
      initState[i] = 0x00;
    }
  }

  // Copy initState into state variable
  for(i = 0; i < 16; i++) {

    state_t[ i / 4 ][ i % 4 ] = initState[i];
  }

  //inital password
  return;
}

/////////////////////////////////////////////////////////////////////
/// Generates state matrix from encrypted password
///
/// @param[out]   state_t    inital state of Password
/// @param[in]    password   plaintest Password
///
/////////////////////////////////////////////////////////////////////
void resetState(uint8_t state_t[4][4], char password[17])
{

  int i;

  //For the first 16 elements or the entire initalizer
  for(i = 0; i < 16 ; i++) {

    //Copy each element into initKey
    state_t[ i / 4 ][ i % 4 ] = password[i] & 0xff;
  }

  //inital password
  return;
}

/////////////////////////////////////////////////////////////////////
/// main function to encrypt the password
///
/// @param[in/out]  password    inital and encrptyed password
/// @param[int]     ID          character array to id password
///
/////////////////////////////////////////////////////////////////////
void printProgress(uint8_t state_t[4][4])
{
  int i, j;

  for(i = 0; i < 4; i++) {
    for(j = 0; j < 4; j++) {

      printf("%x ", state_t[i][j] & 0xff);
    }
    cout << endl;
  }

  cout << endl;

}


/////////////////////////////////////////////////////////////////////
/// Creates new char array from state matrix
///
/// @param[out]  password    to be encrptyed password
/// @param[int]     ID          character array to id password
///
/////////////////////////////////////////////////////////////////////
void new_password (char password[17], uint8_t state_t[4][4])
{
  int   i, j;
  int   index = 0;

  for (i = 0; i < 4; i++)
  {

    for (j = 0; j < 4; j++)
    {

      password[index] = state_t[i][j] & 0xff;
      index++;
    }
  }

  //Add null terminator
  password[16] = '\0';

}


/////////////////////////////////////////////////////////////////////
/// main function to encrypt the password
///
/// @param[in/out]  password    inital and encrptyed password
/// @param[int]     ID          character array to id password
/// @param[out]     state_t     state matrix for password
///
/////////////////////////////////////////////////////////////////////
void encrpyt(char password[16], char *ID, uint8_t state_t[4][4])
{

    //Steps for encrption
    /*
     * addRoundKey -> 1 time
     *
     * Repeat 9 times
     *  -   subBytes
     *  -   shiftRows
     *  -   mixColumns
     *  -   addRoundKey
     *
     * subBytes
     * shiftRows
     * addRoundKey
     *
     */


    int       i, j, k;
    int       roundNum = 0;

    uint8_t   currentKey[11][4][4]; //Need 11 keys

    //Set password into state
    setState(state_t, password);

    //cout << "Original Password: \n";
    //printProgress(state_t);

    //Get inital key from key ID
    initalKey(ID, currentKey[0]);

    //incremenet the round number
    roundNum++;

    //Initial Round
    addRoundKey(state_t, currentKey[0]);

    //get next round key
    nextKey(currentKey[0], currentKey[1], roundNum);

    //incremenet the round number
    roundNum++;

    //Rounds 1-9
    for(i = 1; i < 10; i++){

      subBytes(state_t);
      shiftRows(state_t);
      mixColumns(state_t);
      addRoundKey(state_t, currentKey[i]);

      //get next round key
      nextKey(currentKey[i], currentKey[i+1], roundNum);

      //incremenet the round number
      roundNum++;
    }


    //Final Round
    subBytes(state_t);
    shiftRows(state_t);
    addRoundKey(state_t, currentKey[i]);


    //cout << "\nThe new password is: \n";
    //printProgress(state_t);

    return;
}


/////////////////////////////////////////////////////////////////////
/// main function to decrypt the password
///
/// @param[in]      ID          character array to id password
/// @param[in/out]  state_t     state matrix for password
///
/////////////////////////////////////////////////////////////////////
void decrpyt( char *ID, uint8_t state_t[4][4])
{

  //Steps for encrption
    /*
     * addRoundKey -> 1 time
     *
     * Repeat 9 times
     *  -   shiftRowsInv
     *  -   subBytesInv
     *  -   addRoundKeyInv
     *  -   mixColumnsInv
     *
     * shiftRowsInv
     * subBytesInv
     * addRoundKey
     *
     */


  int       i, j, k;
  int       roundNum = 0;

  uint8_t   currentKey[11][4][4]; //Need 11 keys

  //Determine all of the keys to use
  initalKey(ID, currentKey[0]);

  //Find each of the keys
  for(i = 1; i < 11; i++) {

    //Get next key
    nextKey(currentKey[i-1], currentKey[i], i);
  }

  //Add round key
  addRoundKey(state_t, currentKey[10]);

  //For 10 rounds
  for(i = 9; i > 0; i--) {

    shiftRowsInv(state_t);
    subBytesInv(state_t);
    addRoundKeyInv(state_t, currentKey[i]);
    mixColumnsInv(state_t);

  }

  shiftRowsInv(state_t);
  subBytesInv(state_t);
  addRoundKeyInv(state_t, currentKey[0]);


  //cout << "\nThe password was: \n";
  //printProgress(state_t);

  return;
}

/*
int main()
{
    char type[50];
    char password[16];

    uint8_t   state_t[4][4];

    cout << "Enter a password Name to generate: ";
    cin >> type;

    cout << endl << "Enter the password to encrypt: ";
    cin >> password;



    //Encrypt the password
    encrpyt(password, type, state_t);

    //Encrypt the password
    decrpyt(password, type, state_t);

    return 0;
}
*/
