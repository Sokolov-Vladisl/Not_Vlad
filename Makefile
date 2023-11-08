CC := g++
Sources := LAB1GTSF.cpp
Result := lab
ini := param.txt
Vars := z0 = 1.0 \nv0 = 0.5 \nho = 0.1 \nNmax = 50 \nb = 5 \neps = 1e-6 \na = 0.2
all: comp file
comp:
	$(CC) $(Sources) -o $(Result)
file:
	rm $(ini)
	echo -e '$(Vars)' > $(ini)
	nano $(ini)
clean:
	rm -f comp file
