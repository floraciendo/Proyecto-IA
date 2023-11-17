INSTANCIA = BENG01

all: exec run

exec: 2dspp.cpp
	@g++ 2dspp.cpp -o exec

run:
	@./exec $(INSTANCIA).txt > $(INSTANCIA).out

clean:
	@rm -f exec $(INSTANCIA).out
