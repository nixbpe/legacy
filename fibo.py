import sys

def fibo(n):
	if n <= 2 :
		return 1
	p1 = 1
	p2 = 1
	for i in range (2,n):
		fibo = p1 + p2
		p2 = p1
		p1 = fibo
	return fibo


print fibo(int(sys.argv[1]))
