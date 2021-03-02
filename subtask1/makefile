main : a.out
	clear
	@echo "type:$$./a.out <image_name>"

a.out : subtask.cpp
	g++ subtask.cpp -std=c++11 `pkg-config --cflags --libs opencv`


clean :
	rm a.out
	rm 'cropped.jpg'
	rm 'projected.jpg'	
