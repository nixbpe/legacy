import sys
import math
def isprime(n):
	tmp = int(math.sqrt(n))
	print tmp
	if n == 1:
		return 'No'
	if n == 2: 
		return 'Yes'
	for i in range(2,tmp):
		if n%i == 0 :
			return 'No'
	return 'Yes'
	

print isprime(int(sys.argv[1]))
