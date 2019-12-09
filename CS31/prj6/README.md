# CS31-Project-6

1.a.

CORRECT VERSION:

int main()
    {
        int arr[3] = { 5, 10, 15 };
        int* ptr = arr;

        *ptr = 30;          // set arr[0] to 30
        *(ptr + 1) = 20;      // set arr[1] to 20			<--- Brackets added
        ptr += 2;
        ptr[0] = 10;        // set arr[2] to 10			
	ptr = arr;							<— Set pointer to first element

        while (ptr < arr + 3)			//			<—— Start from beginninggining go to end
        {
            
            cout << *ptr << endl;    // print values
            ptr++;					//						<--- Put this after the cout
        }
    }

1.b.?

void findMax(int arr[], int n, int *& pToMax)		// <— Must be reference to pointer
    {
        if (n <= 0) 
            return;      // no items, no maximum!
    
        pToMax = arr;

        for (int i = 1; i < n; i++)
        {
            if (arr[i] > *pToMax)
                 pToMax = arr + i;
        }
    }       

    int main()
    {
        int nums[4] = { 5, 3, 15, 6 };
        int* ptr;

        findMax(nums, 4, ptr);
        cout << "The maximum is at address " << ptr << endl;
        cout << "It's at position " << ptr - nums << endl;
        cout << "Its value is " << *ptr << endl;
    }

1.c.

void computeCube(int n, int* ncubed)
    {
        *ncubed = n * n * n;
    }

    int main()
    {
        int n = 5;		// <— We need a variable
        int* ptr = &n;		// fix the pointer to the address of n
        computeCube(n, ptr);	// change  *ptr
        cout << "Five cubed is " << *ptr << endl;
    }

1.d.

// return true if two C strings are equal
    bool strequal(const char str1[], const char str2[])
    {
        while (*str1 != '\0'  &&  *str2 != '\0')			// <--- '\0' instead of 0; *str1, *str2 instead of str1, str2
        {
            if (*str1 != *str2)  // compare corresponding characters	<--- *str1, *str2 instead of str1, str2
                return false;
            str1++;            // advance to the next character
            str2++;
        }
        return *str1 == *str2;   // both ended at same time?	<--- *str1, *str2 instead of str1, str2
    }

    int main()
    {
        char a[15] = "Zhao";
        char b[15] = "Zhou";

        if (strequal(a,b))
            cout << "They're the same person!\n";
    }

1.e.

#include <iostream>
    using namespace std;

    int* getPtrToArray(int& m)
    {
        int anArray[100];
        for (int j = 0; j < 100; j++)
            anArray[j] = 100-j;
        m = 100;
        return anArray;
    }

    void f()
    {
        int junk[100];
        for (int k = 0; k < 100; k++)
            junk[k] = 123400000 + k;
    }

    int main()
    {
        int n;
        int* ptr = getPtrToArray(n);
        f();
        for (int i = 0; i < 3; i++)
            cout << ptr[i] << ' ';
        for (int i = n-3; i < n; i++)
            cout << ptr[i] << ' ';
        cout << endl;
    }

   The elements of the array anArray are initialized within getPtrToArray. Once the program returns out of getPtrToArray, all the elements initialized within that function are deleted and meaningless. Therefore, when we reference these variables in main() we get junk.

2.

#include <iostream>
    using namespace std;


    int main()
    {
        double* cat;
        double mouse[5];
        cat = mouse + 4;
        *cat = 25;
        *(mouse+3) = 42;
        cat-=3;
        cat[1] = 54;
        cat[0] = 17;
        bool b = (*cat == *(cat+1));
        bool d = (cat == mouse);
    }

3.a.

 double mean(const double* scores, int numScores)
    {
        double tot = 0;
        for(int i = 0; i<numScores; i++){
            tot += *(scores+i);
        }
        return tot/numScores;
    }

3.b.
    const char* findTheChar(const char* str, char chr)
    {
        for (int k = 0; *(str+k) != ‘\0’; k++)
            if (*(str+k) == chr)
                return (str+k);

        return nullptr;
    }

3.c.

    const char* findTheChar(const char* str, char chr)
    {
        while(*str != '\0'){
            if (*str == chr)
                return str;
            str++;
        }
        return nullptr;
    }

4.

	1. maxwell is called on the pointer to the first and third element of array
	2. the pointer ptr is initialized to array (since 5>3)
	3. the value of array[0] is set to -1 through ptr
	4. the pointer now points to array[2]
	5. since &ptr[1] == &array[3] we get array[3] = 9
	6. since *(array+1) = array[1] we get array[1] = 79
	7. *array[5] points to position 5 in the array, ptr points to position 2 in array, therefore &array[5] - ptr = 5-2 = 3, which is printed out
	8. swap1() doesn’t do anything, since a change of pointers is attempted, yet the pointers aren’t passed by reference
	9. swap2() swaps the values the pointers point to
	10. Hence, swap1(&array[0], &array[1]) doesn’t do anything
	11. swap2(array, &array[2]) swaps the values stored in positions 0 and 2 in the array
	12. Following the previous steps we see that the elements of array will be printed out (one per line) as follows:
		4
		79
		-1
		9
		22
		19

5.

    void removeS(char* msg){
        char* temp = msg;
        while(*msg!='\0'){
            while('s'==*msg || ’S’==*msg){
                temp = msg;
                while(*temp!='\0'){
                    *(temp) = *(temp+1);
                    temp++;
                }
            }
            msg++;
        }
    }
