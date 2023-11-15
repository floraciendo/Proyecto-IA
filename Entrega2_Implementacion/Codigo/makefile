INSTANCIA = BENG01
SEMILLA = 202073522

all: exec run

exec: 2dspp.cpp
	@g++ 2dspp.cpp -o exec

run:
	@./exec $(INSTANCIA).txt $(SEMILLA) > $(INSTANCIA)_$(SEMILLA).out

clean:
	@rm -f exec $(INSTANCIA)_$(SEMILLA).out
