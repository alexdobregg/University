P1 = crypto
P2 = stocks
P3 = valley
P4 = ridge

GCC = g++

build:
	$(GCC) $(P1).cpp -o $(P1)
	$(GCC) $(P2).cpp -o $(P2)
	$(GCC) $(P3).cpp -o $(P3)
	$(GCC) $(P4).cpp -o $(P4) 

run-p1:	
	./crypto

run-p2: 
	./stocks

run-p3: 
	./valley

run-p4: 
	./ridge

clean:
	rm -f $(P1) $(P2) $(P3) $(P4)	

.PHONY: clean
