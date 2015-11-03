#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==============================================================
/*!
 *     \name   ASSERT macro
 *     \brief  This macros show error if condition is false
 *             This macros works if DEBUG defined!
 *     \param  Condition
 *     \return Abort program or do nothing.
 */
//==============================================================

#define DEBUG

#ifdef DEBUG
    #define ASSERT( cond )                       \
        if ( ! ( cond ) )                        \
        {                                        \
            printf ( "FAIL %s in %s, %s ( %d )", \
            #cond, __PRETTY_FUNCTION__,          \
            __FILE__, __LINE__ );                \
            abort();                             \
        }
#else
    #define ASSERT( cond ) ;
#endif

//================== Prototypes ===================
void DeletingSpaces ();
    char* ReadText ( int MaxSizeText, int* TextLength );
    void AlgorithmDeletingSpacesAndOut ( char* Text, int TextLength, int MaxSizeText );
//******************************

void ReverseWords ();
//******************************

void SumDigitsOnDiagonalSquareMatrix ();
//******************************

void AddToBinaryDigits ();
    int DeterminantGreatestDigit ( char* FirstDigit, int* NumberBit1, \
                                   char* SecondDigit, int* NumberBit2 );
    int SumBinary ( char* FirstDigit, int NumberBit1, \
                    char* SecondDigit, int DifferenceShiftBits );
//******************************

void FindBiggestWhiteSquare ();
    int Min ( int Element1, int Element2 );
//******************************

const int MaxWordLength = 29;

typedef struct
{
    char* Name;
    int Mark;
} Student_t;

void SortStudents ();
    void DeleteDublicates ();
    void AlphabeticSort ( Student_t* Student, const int End );
    void RatingSort ( Student_t* Student, const int End );
//******************************

void DeleteDublicates ();
//******************************

void NumbersFreeOfSquares ();
//******************************

void Weighing ();
    int WeighRecursion ( int Weigh );
//******************************

void FourDigitsNumbers ();
    int IncNumber ( int* FourDigitNumber, int NumberDigit );
//=================================================

int main()
{
    //DeletingSpaces ();
    //ReverseWords ();
    //SumDigitsOnDiagonalSquareMatrix ();
    //AddToBinaryDigits ();
    //FindBiggestWhiteSquare ();
    //SortStudents ();
    //DeleteDublicates ();
    //NumbersFreeOfSquares ();
    //Weighing ();
    //FourDigitsNumbers ();
    return 0;
}

//==============================================================
/*!
 *     TestMIPT 2008, Variant 1, Task C
 *     \brief  This function delete excess spaces between words from inputting data
 *     \param  Text with length less that 100000 symbols
 *     \return Inputting text with deleting excess spaces
 */
//==============================================================

void DeletingSpaces ()
{
    const int MaxSizeText = 100000;
    int TextLength = 0;
    //============ Read text ===============
    char* Text = ReadText ( MaxSizeText, &TextLength );
    ASSERT ( Text );
    //======================================
    AlgorithmDeletingSpacesAndOut ( Text, TextLength, MaxSizeText );
    free ( Text ); Text = NULL;
    //==========================================
}

//===================================================================
char* ReadText ( int MaxSizeText, int* TextLength )
{
    char* Text = ( char* ) calloc ( MaxSizeText, sizeof ( *Text ) );
    (*TextLength) = -1;
    do
    {
        (*TextLength)++;
        ASSERT ( (*TextLength) < MaxSizeText && (*TextLength) >= 0 );
        scanf ( "%c", &Text[*TextLength] );
    }
    while ( Text[*TextLength] != '\n' );
    return Text;
}
//=================================================================
void AlgorithmDeletingSpacesAndOut ( char* Text, int TextLength, int MaxSizeText )
{
    //Determinant space and print other symbols
    for ( int i = 0; i <= TextLength; i++ )
    {
        ASSERT ( i < MaxSizeText && i >= 0);
        //===== Test on space, print symbol if no space ==========
        if ( Text [i] != ' ' )
        {
            printf ( "%c", Text[i] );
        } else
        //===== If read symbol is space, then print one space
        {
            if ( i != 0 && Text[i-1] != ' ' && ( i+1 != TextLength ) )
            {
                printf ( "%c", Text[i] );
            }
        }
    }
}

//==============================================================
/*!
 *     TestMIPT 2007, Variant 4, Task C
 *     \brief  This function reverse all words in inputting text
 *     \param  One string. Length text no more 1000 symbols
 *     \return One string. All words reversed
 */
//==============================================================

void ReverseWords ()
{
    const int MaxSizeWord = 1000;
    char* Word = ( char* ) calloc ( MaxSizeWord, sizeof ( *Word ) );
    ASSERT ( Word );
    while ( 1 )
    {
        int CurrentLengthWord = -1;
        //===== Read word ========
        do
        {
            CurrentLengthWord++;
            ASSERT ( CurrentLengthWord < MaxSizeWord && CurrentLengthWord >= 0 );
            scanf ( "%c", &Word[CurrentLengthWord] );
        } while ( ( Word[CurrentLengthWord] >= 'a' && Word[CurrentLengthWord] <= 'z' ) || \
                  ( Word[CurrentLengthWord] >= 'A' && Word[CurrentLengthWord] <= 'Z' ) );
        //========================
        int i = 0;
        //======= Out reverse word =========
        for ( i = CurrentLengthWord - 1; i >= 0; i-- )
        {
            ASSERT ( i < MaxSizeWord && i >= 0 );
            putchar ( Word[i] );
        }
        //==================================
        //====== Test on \n symbol and print other symbol ========
        ASSERT ( CurrentLengthWord < MaxSizeWord && CurrentLengthWord >= 0 );
        if ( Word[CurrentLengthWord] == '\n')
        {
            break;
        } else
        {
            putchar ( Word[CurrentLengthWord] );
        }
        //========================================================
    }
    free ( Word ); Word = 0;
}

//==============================================================
/*!
 *     TestMIPT 2008, Variant 4, Task C
 *
 *     \brief  This function sum all digits on diagonal squaring matrix
 *     \param  Size matrix and elements of matrix
 *     \return Sum elements on diagonal of matrix
 */
//==============================================================

void SumDigitsOnDiagonalSquareMatrix ()
{
    int N = 0;
    // Read size of matrix and create array with address on string
    scanf ( "%d", &N );
    int** Line = ( int** ) calloc ( N, sizeof ( **Line ) );
    ASSERT ( Line );
    // Read content of matrix and create array column for every string
    for ( int i = 0; i < N;  i++ )
    {
        ASSERT ( i < N && i >= 0 );
        Line[i] = ( int* ) calloc ( N, sizeof ( *Line[i] ) );
        ASSERT ( Line[i] );
        for ( int j = 0; j < N; j++ )
        {
            ASSERT ( j < N && j >= 0 );
            scanf ( "%d", Line [i] + j );
        }
    }
    //================================================================
    int* Sum = ( int* ) calloc ( 2 * N - 1, sizeof ( *Sum ) );
    ASSERT ( Sum );
    int CounterSum = 0;
    //Sum elements on diagonal in first half of matrix
    for ( int i = N - 1; i >= 0; i-- )
    {
        for ( int j = 0; j <= N - i - 1; j++ )
        {
            ASSERT ( i + j < N && i + j >= 0 );
            ASSERT ( CounterSum < 2 * N - 1 && CounterSum >= 0 );
            Sum[CounterSum] += *( Line [i + j] + j);
        }
        CounterSum++;
    }
    //================================================
    //Sum elements on diagonal in second half of matrix
    for ( int i = 1; i <= N - 1; i++ )
    {
        for ( int j = 0; j <= N - i - 1; j++ )
        {
            ASSERT ( i + j < N && i + j >= 0 );
            ASSERT ( CounterSum < 2 * N - 1 && CounterSum >= 0 );
            Sum[CounterSum] += *( Line [j] + i + j);
        }
        CounterSum++;
    }
    //=============== Out Sum ==================
    for ( int i = 0; i < CounterSum;  i++ )
    {
        ASSERT ( i < 2 * N - 1 && i >= 0 );
        printf ( "%d ", Sum[i] );
    }
    //==========================================
    //=============== free all memory ==============
    for ( int i = 0; i < N;  i++ )
    {
        ASSERT ( i < N && i >= 0 );
        free ( Line[i] ); Line [i] = 0;
    }
    free ( Line ); Line = 0;
    //==============================================
}

//==============================================================
/*!
 *     TestMIPT 2007, Variant 9, Task C
 *
 *     \brief  This function add two 1000 bit binary digits
 *     \param  The first string  - first digit  \n
 *             The second string - second digit
 *     \return Sum two digits
 */
//==============================================================

const int ZeroCodeASCII = 48;
const int MaxBits = 1000;

void AddToBinaryDigits ()
{
    //======== Read first digit ========
    char* FirstDigit  = ( char* ) calloc ( MaxBits, sizeof ( *FirstDigit ) );
    ASSERT ( FirstDigit );
    FirstDigit[0] = ZeroCodeASCII;
    int NumberBit1 = 0;

    do
    {
        NumberBit1++;
        ASSERT ( NumberBit1 < MaxBits && NumberBit1 >= 0 );
        scanf ( "%c", &FirstDigit[NumberBit1] );
    }
    while ( FirstDigit[NumberBit1] != '\n' );

    //======== Read second digit =======
    char* SecondDigit  = ( char* ) calloc ( MaxBits, sizeof ( *SecondDigit ) );
    ASSERT ( SecondDigit );
    SecondDigit[0] = ZeroCodeASCII;
    int NumberBit2 = 0;

    do
    {
        NumberBit2++;
        ASSERT ( NumberBit2 < MaxBits && NumberBit2 >= 0 );
        scanf ( "%c", &SecondDigit[NumberBit2] );
    }
    while ( SecondDigit[NumberBit2] != '\n' );

    //================= Determinant the greatest digit ====================
    int DifferenceShiftBits = DeterminantGreatestDigit ( FirstDigit, &NumberBit1, \
                                                         SecondDigit, &NumberBit2 );
    //================= Sum two binary bits ====================
    SumBinary ( FirstDigit, NumberBit1, SecondDigit, DifferenceShiftBits );
    //================== Out result ==============================
    int i = 0;
    while ( FirstDigit[i] == '0' )
    {
        ASSERT ( i < MaxBits && i >= 0 );
        i++;
    }
    ASSERT ( i + 1 < MaxBits && i + 1 >= 0 );
    if ( FirstDigit[i + 1] == '\0' ) i--;
    printf ( "%s", FirstDigit + i );
    //============================================================
    free ( FirstDigit ); FirstDigit = 0;
    free ( SecondDigit ); SecondDigit = 0;
    //============================================================
}

//============ This function return Difference between NumberBit1 ========
//=========== and NumberBit2, and swap address if two more than one ======
int DeterminantGreatestDigit ( char* FirstDigit, int* NumberBit1, \
                               char* SecondDigit, int* NumberBit2 )
{
    int Difference = abs ( *NumberBit1 - *NumberBit2 );
    if ( *NumberBit1 < *NumberBit2 )
    {
        for ( int i = 0; i < *NumberBit2; i++ )
        {
            ASSERT ( i < MaxBits && i >= 0 );
            FirstDigit[i] = SecondDigit[i] + FirstDigit[i];
            SecondDigit[i] = FirstDigit[i] - SecondDigit[i];
            FirstDigit[i] = FirstDigit[i] - SecondDigit[i];
        }
        *NumberBit1 = *NumberBit2;
    }
    return Difference;
}
//======================================================================

//==================== This function add two binary numbers ================
int SumBinary ( char* FirstDigit, int NumberBit1, \
                char* SecondDigit, int DifferenceShiftBits )
{
    int MemoryBit = 0;
    for ( int i = NumberBit1 - 1; i >= 0; i-- )
    {
        if ( i - DifferenceShiftBits > 0 )
        {
            ASSERT ( i < MaxBits && i >= 0 );
            ASSERT ( i - DifferenceShiftBits < MaxBits && i - DifferenceShiftBits >= 0 );
            FirstDigit[i] += SecondDigit[i - DifferenceShiftBits] - ZeroCodeASCII;
        }
        if ( MemoryBit == 1 )
        {
            ASSERT ( i < MaxBits && i >= 0 );
            FirstDigit[i] += MemoryBit ;
            MemoryBit = 0;
        }
        ASSERT ( i < MaxBits && i >= 0 );
        if  ( FirstDigit[i] - ZeroCodeASCII >= 2 )
        {
            MemoryBit = 1;
            ASSERT ( i < MaxBits && i >= 0 );
            if  ( FirstDigit[i] - ZeroCodeASCII == 2 )
            {
                FirstDigit[i] = ZeroCodeASCII;
            } else
            {
                FirstDigit[i] = ZeroCodeASCII + 1;
            }
        }
    }
    return 0;
}
//======================================================================

//==============================================================
/*!
 *     TestMIPT 2007, Variant 3, Task D
 *
 *     \brief  This function detecting max size white square
 *     \param  The first number - size field    \n
 *             The second and n string - squaring field
 *     \return Size side the max square and his \n
 *             coordinate from right bottom vertex
 */
//==============================================================

void FindBiggestWhiteSquare ()
{
    int MapSize = 0;

    //====Structure coordinates and size of  biggest square======
    struct
    {
        int Size;
        int X;
        int Y;
    } Answer;
    //==================== Create arrays =========================
    Answer.Size = 0;

    scanf ( "%d\n", &MapSize );

    char** Map = ( char** ) calloc ( MapSize, sizeof ( **Map ) );
    ASSERT ( Map );

    for ( int i = 0; i < MapSize; i++ )
    {
        ASSERT ( i < MapSize && i >= 0 );
        Map[i] = ( char* ) calloc ( MapSize, sizeof( *Map[i] ) );
        ASSERT ( Map[i] );
    }

    int** MapOfSizes = ( int** ) calloc ( MapSize, sizeof ( **MapOfSizes ) );

    for ( int i = 0; i < MapSize; i++ )
    {
        ASSERT ( i < MapSize && i >= 0 );
        MapOfSizes[i] = ( int* ) calloc ( MapSize, sizeof ( *MapOfSizes[i] ) );
        ASSERT ( MapOfSizes[i] );
    }
    //=========================================================
    //==================  Read from console inputting data ==============
    for ( int X = 0; X < MapSize; X++ )
    {
        for ( int Y = 0; Y < MapSize; Y++ )
        {
            ASSERT ( X < MapSize && Y < MapSize && X >= 0 && Y >= 0 );
            Map[X][Y] = getchar();
        }
        getchar();
    }
    //===================================================================
    for ( int X = 0; X < MapSize; X++ )
    {
        for ( int Y = 0; Y < MapSize; Y++ )
        {
            ASSERT ( X < MapSize && Y < MapSize && X >= 0 && Y >= 0 );

            if ( Map[X][Y] == '#' )
            {
                MapOfSizes[X][Y] = 0;
            }

            if ( ( X == 0 || Y == 0 ) && Map[X][Y] == '.' )
            {
                MapOfSizes[X][Y] = 1;
            }

            if ( X != 0 && Y != 0 && Map[X][Y] == '.' )
            {
                MapOfSizes[X][Y] = 1 + Min ( MapOfSizes[X - 1][Y], \
                                             Min ( MapOfSizes[X][Y - 1], \
                                             MapOfSizes[X - 1][Y - 1] ) );
            }

            if ( MapOfSizes[X][Y] > Answer.Size || ( MapOfSizes[X][Y] == Answer.Size && \
                 Answer.X > X ) )
            {
                Answer.X = X;
                Answer.Y = Y;
                Answer.Size = MapOfSizes[X][Y];
            }
        }
    }
    //================= Clear memory ==================
    for ( int i = 0; i < MapSize; i++ )
    {
        ASSERT ( i < MapSize &&  i >= 0 );
        free ( Map[i] );
        free ( MapOfSizes[i] );
        Map[i] = 0;
        MapOfSizes[i] = 0;
    }

    free ( Map );
    Map = 0;

    free ( MapOfSizes );
    MapOfSizes = 0;
    //=================================================
    //========= Out size and coordinates ==============
    if ( Answer.Size == 0 )
    {
        printf( "%s", "There is not square" );
    }

    if ( Answer.Size != 0 )
    {
        printf( "%d\n", Answer.Size );
        printf( "%d %d", Answer.Y, Answer.X );
    }
    //==================================================
}

// ===== Find minimal element from two variable =========
int Min ( int Element1, int Element2 )
{
    if ( Element1 <= Element2 )
    {
        return Element1;
    }
    return Element2;
}
//==========================================================

//==============================================================
/*!
 *     TestMIPT 2008, Variant 9, Task C
 *
 *     \brief  This function sort list of students ( alphabet, rating )
 *     \param  The first number - Number of elements in list \n
 *             The second and n string - elements of list:   \n
 *              - Name student                               \n
 *              - Rating
 *     \return Sort list on two algorithm
 */
//==============================================================

void SortStudents ()
{
    int N = 0;
    scanf ( "%d\n", &N );
    //=======================================================================
    //===================== Create array ====================================
    //=======================================================================
    Student_t* Student = ( Student_t* ) calloc ( N, sizeof ( *Student ) );
    ASSERT ( Student );
    //=======================================================================
    //==================== Read inputting data ==============================
    //=======================================================================
    for ( int i = 0; i < N; i++ )
    {
        ASSERT ( i < N && i >= 0 );
        Student[i].Name = ( char* ) calloc ( MaxWordLength, sizeof ( *Student[i].Name ) );
        ASSERT ( Student[i].Name );
        scanf ( "%s", Student[i].Name );
        scanf ( "%d", &Student[i].Mark );
    }
    //===========================================================
    //================ Out with alphabetic sorting ==============
    //===========================================================
    AlphabeticSort ( Student, N );
    printf ( "\n" );

    for ( int i = 0; i < N; i++ )
    {
        ASSERT ( i < N && i >= 0 );
        printf ( "%s", Student[i].Name );
        printf ( " %d \n", Student[i].Mark );
    }
    printf ( "\n" );
    //===========================================================
    //================ Out with rating sorting ==================
    //===========================================================
    RatingSort ( Student, N );

    for ( int i = 0; i < N; i++ )
    {
        ASSERT ( i < N && i >= 0 );
        printf ( "%s", Student[i].Name );
        printf ( " %d \n", Student[i].Mark );
    }
    //========================================================
    //=================== Free memory ========================
    //========================================================
    for ( int i = 0; i < N; i++ )
    {
        ASSERT ( i < N && i >= 0 );
        free ( Student[i].Name );
        Student[i].Name = 0;
    }

    free ( Student ); Student = 0;
    //========================================================*/
}

//============================================================================
//============= This helping function sort list with alphabet ================
//============================================================================
void AlphabeticSort ( Student_t* Student, const int End )
{
    for ( int i = 0; i < End - 1; i++ )
    {
        for ( int j = i + 1; j < End; j++ )
        {
            if ( strcmp ( Student[i].Name, Student[j].Name ) > 0 )
            {
                ASSERT ( i < End && i >= 0 && j < End && j >= 1 );

                Student_t temp = Student[i];
                Student[i] = Student[j];
                Student[j] = temp;
            }
        }
    }
}
//============================================================================
//============= This helping function sort list with rating ==================
//============================================================================
void RatingSort ( Student_t* Student, const int End )
{
    for ( int i = 0; i < End - 1; i++ )
    {
        for ( int j = i + 1; j < End; j++ )
        {
            if ( Student[i].Mark < Student[j].Mark )
            {
                ASSERT ( i < End && i >= 0 && j < End && j >= 1 );

                Student_t temp = Student[i];
                Student[i] = Student[j];
                Student[j] = temp;
            }
        }
    }
}
//==========================================================================

//==============================================================
/*!
 *     TestMIPT 2008, Variant 9, Task C
 *
 *     \brief  This function delete duplicating numbers
 *     \param  Number of digits
 *     \return Digits without duplicating numbers
 */
//==============================================================

void DeleteDublicates ()
{
    const int MaxNumber = 1000;

    int N = 0;
    scanf ( "%d\n", &N );
    //============= Create array ===========================================
    int* Sequence = ( int* ) calloc ( N, sizeof ( *Sequence ) );
    ASSERT ( Sequence );

    int* AmountNums = ( int* ) calloc ( MaxNumber, sizeof ( *AmountNums ) );
    ASSERT ( AmountNums );
    //======================================================================

    //===============Entering of sequence=======
    for ( int i = 0; i < N; i++ )
    {
        ASSERT ( i >= 0 && i < N );
        scanf ( "%d", &Sequence[i] );
    }
    //==========================================
    //==== Handler and print ===================
    for ( int i = 0; i < N; i++ )
    {
        ASSERT ( i >= 0 && i < N );
        ASSERT ( Sequence[i] >= 0 && Sequence[i] < MaxNumber );
        if ( AmountNums[Sequence[i]] == 0 )
        {
            printf ( "%d ", Sequence[i]);
            AmountNums[Sequence[i]]++;
        }
    }
    //==========================================
    free ( Sequence ); Sequence = 0;
    free ( AmountNums ); AmountNums = 0;
}

//==============================================================
/*!
 *     TestMIPT 2007, Variant 6, Task C
 *
 *     \brief  This function find numbers without divider, \n
 *             which square number more than 1
 *     \param  Number digits and sequence
 *     \return Digits without without divider e.t.c.
 */
//==============================================================

void NumbersFreeOfSquares ()
 {
	const int MaxSize = 200;
	const int MaxDivider = 65536;
    //========== Create array =======================
	long int* Numbers = ( long int* ) calloc ( MaxSize, sizeof( *Numbers ) );
	ASSERT ( Numbers );

	int N = 0;
    //===============================================
    //======== Read inputting data ==================
	scanf ( "%d", &N );

	for ( int i = 0; i < N; i++ )
    {
        scanf ( "%ld", &Numbers[i] );
    }
    //===============================================
    //===== Handler and print =======================
	for (int i = 0; i < N; i++ )
    {
		for ( int j = 2; j < MaxDivider; j++)
		{
		    ASSERT ( i >= 0 && i < N );
			if ( Numbers[i] % ( j * j ) == 0 )
            {
                break;
            }

			if ( j == MaxDivider - 1 )
            {
                printf( "%ld ", Numbers[i] );
            }
		}
	}
	//=================================================
	free ( Numbers ); Numbers = 0;
}

//==============================================================
/*!
 *     TestMIPT 2007, Variant 1, Task C
 *
 *     \brief  This function weight number \n
 *     \param  Number
 *     \return YES if number can weight    \n
 *             NO  if number can't weight
 */
//==============================================================

void Weighing ()
{
	int Weigh = 0;
	scanf( "%d", &Weigh );

	if ( WeighRecursion ( Weigh ) == 1 )
    {
        printf ( "YES" );
    } else
    {
        printf ( "NO" );
    }
}

//========== Expansion number in square 4 ==========
int WeighRecursion ( int Weigh )
{
	if ( Weigh / 4 != 0 )
    {
        return ( Weigh % 4 == 2 ) ? 0 : WeighRecursion ( Weigh / 4 );
	} else
    {
        switch ( Weigh )
        {
		case 0:
        case 2:
        case 3:

		    return 1;
            break;

		case 1:

		    return 1;
            break;

        default:

            break;
        }
    }
    return 0;
}
//==================================================

//==============================================================
/*!
 *     TestMIPT 2008, Variant 7, Task C
 *
 *     \brief  This function find number, sum which equal inputting number
 *     \param  Number
 *     \return Numbers, sum which equal inputting number
 */
//==============================================================

void FourDigitsNumbers ()
{
	const int FirstFourDigitNumber = 1000;
	const int LastFourDigitNumber = 9999;
	int FourDigitNumber[4] = { 1, 0, 0, 0 };
	int Sum = 0, i = 0, Counter = 0, SumDigit = 0;

	scanf ( "%d", &Sum );

	for ( i = FirstFourDigitNumber; i <= LastFourDigitNumber; i++ )
    {
        SumDigit = FourDigitNumber[0] + FourDigitNumber[1] + \
                   FourDigitNumber[2] + FourDigitNumber[3];
        IncNumber ( FourDigitNumber, 4 );
        if ( SumDigit == Sum )
        {
            Counter++;
            printf ( "%d ", i );
        }
    }
	printf ( "\n%d ", Counter );
}

//======= This function increase number, divided into digit ============
int IncNumber ( int* FourDigitNumber, int NumberDigit )
{
    NumberDigit -= 1;
    ASSERT ( NumberDigit >= 0 && NumberDigit < 4 );
    FourDigitNumber[NumberDigit]++;
    if ( FourDigitNumber[NumberDigit] == 10 )
    {
        FourDigitNumber[NumberDigit] = 0;
        if ( NumberDigit != 0 )
        {
            IncNumber ( FourDigitNumber, NumberDigit );
        }
    }
    return 0;
}
//==============================================================
