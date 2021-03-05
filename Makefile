
#cracker
crack: crack.c
	gcc crack.c -o crack 
	

dh: dh.c
	gcc dh.c -o dh


#debug
debug: crack.c
	gcc  -o debug crack.c 
#clean
clean:
	rm -f crack
	rm -f dh