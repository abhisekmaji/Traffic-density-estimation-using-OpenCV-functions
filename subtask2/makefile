main : a.out
	clear
	@echo "type:$$./a.out <video_name>"

a.out : video.cpp
	g++ video.cpp -std=c++11 `pkg-config --cflags --libs opencv`


clean :
	rm a.out
	rm out.txt
