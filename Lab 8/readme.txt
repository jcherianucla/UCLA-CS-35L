Name: Jahan Kuruvilla Cherian
UID: 104436427
Email: jcherian@ucla.edu
Professor: Eggert
TA: Lauren Samy
File - readme.txt - A file describing the difficulties faced during the
homework and the performance improvement of the multithreaded SRT 
implemenation.

The focus of the multirthreading performance laid in the main.c file
weherein support for only one thread was available and for any number of 
threads not equal to one the program would print out an error.

The task was to allow for greater than one thread, and to do so I realised
that the change had to be made in the nested loops that went through the
width and height of the image. Knowing that pthread_create took in only
one function to create and allows only one argument to be passed into the
function I realised that I had to create one function in which the entire
chunk of code (nested for loops) had to go. The issue however is that it
depended on some variables declared earlier but only one argument could be
passed into the function. I overcame this issue by putting the declaration
of the variables other than the scene variable in the function as well.

The scene variable had to instead be declared as a global variable as it 
had to be used before the multithreading began. pthread_create takes in
a void* argument to the function we wish to multithread and so I decided
to create an array of threadID's and pass in a reference to these id's and
then cast them to integer pointers and dereference them in the function.

I do this because the main issue of the lab was how to multithread the
image rendering and I came to the conclusion if I start each thread's
for loop at the thread number and increment by the total number of threads
then I could multithread by breaking the image into sections for each
thread to render. Incrementing by the total number of threads also overcomes
most race conditions. However one issue continued to persist, and that was
the printing of the color values for each pixel, as there could be race 
conditions in the printing and thus the overall rendering of the image.

To solve this I create a 3 dimensional array of the width, height and color
of the image and store each value for each thread in its respective indices
and then print in the main function to ensure the printing and rendering is
the same as the case with one thread.

The remaining issues were to do with silly compilation errors such as
having to return NULL because I had to create my function as a void* function
as thats the only type of function pthread_create takes in.

Also I had to remember to join all the threads after the threading was complete
so as to avoid confusion between the threads.

In terms of performance improvements, we can see the results of the timings
of rendering each image below, with the first set of times being the case for
a single threaded version.


time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m51.575s
user	0m51.561s
sys		0m0.007s

time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m26.880s
user	0m53.410s
sys		0m0.003s

time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m13.544s
user	0m53.540s
sys		0m0.005s

time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m7.298s
user	0m56.528s
sys		0m0.004s

We notice a significant improvement in the real (execution) timing of the
program because as the number of threads increase, the ray tracing/rendering
of the image is split into concurrent tasks amongst the threads and so 
instead of being serial, we parallelize the task.
