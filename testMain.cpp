#include <iostream> 

using namespace std; 

static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
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

void printProgress(uint8_t state_t[4][4]);

#define getSBoxValue(num) (sbox[(num)])

//Functions to generate round Keys
uint8_t nextKey(uint8_t RoundKey[4][4]);
void initalKey( char *initalizer, uint8_t newKey[4][4]);

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

static void SubBytes(uint8_t state[4][4])
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      state[j][i] = getSBoxValue(state[j][i]);
    }
  }
}

static void ShiftRows(uint8_t state[4][4])
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp        = state[0][1];
  state[0][1] = state[1][1];
  state[1][1] = state[2][1];
  state[2][1] = state[3][1];
  state[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp        = state[0][2];
  state[0][2] = state[2][2];
  state[2][2] = temp;

  temp        = state[1][2];
  state[1][2] = state[3][2];
  state[3][2] = temp;

  // Rotate third row 3 columns to left
  temp        = state[0][3];
  state[0][3] = state[3][3];
  state[3][3] = state[2][3];
  state[2][3] = state[1][3];
  state[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(uint8_t state[4][4])
{
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for (i = 0; i < 4; ++i)
  {  
    t   = state[i][0];
    Tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3] ;
    Tm  = state[i][0] ^ state[i][1] ; Tm = xtime(Tm);  state[i][0] ^= Tm ^ Tmp ;
    Tm  = state[i][1] ^ state[i][2] ; Tm = xtime(Tm);  state[i][1] ^= Tm ^ Tmp ;
    Tm  = state[i][2] ^ state[i][3] ; Tm = xtime(Tm);  state[i][2] ^= Tm ^ Tmp ;
    Tm  = state[i][3] ^ t ;              Tm = xtime(Tm);  state[i][3] ^= Tm ^ Tmp ;
  }
}


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
void setState(uint8_t state_t[4][4], char password[16])
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

void encrpyt(char password[16], char *ID)
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

    uint8_t   state_t[4][4];
    
    int       i, j, k;
    int       roundNum = 0;

   uint8_t   currentKey[11][4][4]; //Need 11 keys


    //Set password into state
    setState(state_t, password);
    
    cout << "Original Password: \n";
    printProgress(state_t);

    //Get inital key from key ID
    initalKey(ID, currentKey[0]);

    //incremenet the round number
    roundNum++;

    //cout << "inital key" << endl;

    //Initial Round
    addRoundKey(state_t, currentKey[0]);
    cout << "addRoundKey" << endl;
    printProgress(state_t);

    //get next round key
    nextKey(currentKey[0], currentKey[1], roundNum);

    //incremenet the round number
    roundNum++;

    

    //Rounds 1-9
    for(i = 1; i < 10; i++){

      SubBytes(state_t);
    cout << "subBytes" << endl;
    printProgress(state_t);

      ShiftRows(state_t);
    cout << "Shift Rows" << endl;
    printProgress(state_t);

      MixColumns(state_t);
      cout << "Mix Columns" << endl;
    printProgress(state_t);

      addRoundKey(state_t, currentKey[i]);
      cout << "addRoundKey" << endl;
    printProgress(state_t);

      //get next round key
      nextKey(currentKey[i], currentKey[i+1], roundNum);

      //incremenet the round number
      roundNum++;

    }
    

    //Final Round
    SubBytes(state_t);
    ShiftRows(state_t);
    addRoundKey(state_t, currentKey[i]);

    cout << "The new password is: \n";
    printProgress(state_t);

    return;
}

int main() 
{
    char type[50];
    char password[16];

    cout << "Enter a password Name to generate: ";
    cin >> type; 

    cout << endl << "Enter the password to encrypt: "; 
    cin >> password;


    //Encrypt the password
    encrpyt(password, type);


    return 0;
}

/*
97 d7 4c 8c 
5f f8 3 6a 
12 55 ee 8b 
8b 7a ed 12
*/
/*
87 65 1 0 
fc 8 cb 9a 
3 aa 83 1c 
e4 4b 13 4a 
*/

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