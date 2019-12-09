I downloaded and unzipped the srt file onto my computer's desktop. Then
using secure copy I sent it to my SEASNet account.
I ran make clean check on original:
    time ./srt 1-test.ppm >1-test.ppm.tmp

    real    0m53.570s
    user    0m53.562s
    sys 0m0.001s
    mv 1-test.ppm.tmp 1-test.ppm
    time ./srt 2-test.ppm >2-test.ppm.tmp
    ./srt: Multithreading is not supported yet.

    real    0m0.002s
    user    0m0.000s
    sys 0m0.001s
    make: *** [Makefile:36: 2-test.ppm] Error 1

So, the multithread case returns an Error with exit code 1 bacause 
multithreading is not supported, but it does output a time for the single
thread case (defined by command line variable 1-test.ppm 
instead of 2-test.ppm used in multithreading).

First, I went into main.c

I included the necessary header pthread.h

I modified Makefile by changing
    $(CC) $(CFLAGS) -o $@ $(srt_objects) $(LDLIBS)
to
    $(CC) $(CFLAGS) -lpthread -o $@ $(srt_objects) $(LDLIBS)
so that it links to the thread library.

In main.c:
    I set some variables to have global scope since they are used both
    in the thread function and main
        float scaled_color[width][height][3];      // Need this global
        int nthreads;                   // Need this global too
        scene_t scene;                  // Need scene here

    Also, scaled_color has been changed to a three dimensional array.
    This will be justified later.

I notice that there are 4 nested for loops inside main. The calculations
within the top most for loop can be trivially paralelized. This is what I do.
It is done by specifying which thread takes care of which pixel column.
For example, if we run the program with 4 threads then:
    Thread 1 calculates for pixel column 0, 4, 8, 12...
    Thread 2 calculates for pixel column 1, 5, 9, 13...
    Thread 3 calculates for pixel column 2, 6, 10, 14...
    Thread 4 calculates for pixel column 3, 7, 11, 15...
This way, we gurantee that no two threads will step on eachothers toes,
i.e. do calculations on the same pixel.

There is however one problem:
    at the bottom of the for loop nest we see that information about
    pixel is recorded into a file using printf. It seems risque to 
    do this in the thread function since the order of writing the pixels
    into the file is very important. Since this writing isn't very
    computationaly intensive, I decided to do it within main. This
    just required one quick fix (changing scaled_color to a three
    dimensional array). This is necessary in order to preserve the
    information about each pixel in order to be able to write in file
    at the bottom of main later.