# Subtask_1
### Camera angle correction and frame cropping ###

  
Guidline to run the executable file:
```
$make
$./a.out <Image_name>		(*<Image_name> to be substituted by the image name
				that is to be processed. Eg: if we want to process
				the empty.jpg image then write
				$./a.out empty	*)
```
After running the above commands, we have to choose `4 POINTS` on the image opened on
the window in **ANTI-CLOCKWISE** order.
Then we will get our **projected image**.
Press `any key` to get the **cropped image**.
Then again press `a key` to close **all the windows**.

The images get saved on the **current directory**.

We can run the following command on the terminal to delete all the images that have
been saved:
```
$make clean
```
