all: exec run

exec: main.cpp
	@g++ main.cpp -o exec

run:
	@./exec

clean:
	@rm -f exec
