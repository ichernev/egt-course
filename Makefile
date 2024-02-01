all: main2

%: %.cpp
	g++ -o $@ $^

clean:
	rm main main2
