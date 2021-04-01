The "code" folder contains four more folders named method1, method2, method3 and method4
It also contains an error.cpp file to calculate the error of the queue density calculated in
each method.
In method1, method2 we have the cpp files used to calculate the queue density. Each cpp
file produces a csv file and a text file. The csv file contains the frame and 
queue-density value. The text file contains only the queue density value so that it can
be used by the error.cpp file to calculated the utility(error) value compared to baseline.
To run the cpp files we need to write the following commands.

$make
$./a.out <traffic_video_name> <parameter>

This will give us the runtime and also produce two files as mentioned above. Now if we want
to plot the queue density we can write the following command:

$python3 plot.py

This will produce the plot of the queue density we got using the parameter mentioned by
the user.

Now if we want to calculate the error we need to need to copy the .txt files produced to
current file's parent directory and run the following command on terminal
$make
$./a.out <method_number>
