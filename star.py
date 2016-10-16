import sys

def star1(n):
	for i in range(n):
		for j in range(i+1):
			print '*',
		print ''

def star2(n):
	for i in range(n):
		for j in range(n-i):
			print '*',
		print ''

def star3(n):
	for i in range(n):
		for j in range(n-i-1):
			print ' ',
		for j in range(i+1):
			print '* ',
		print ''
def star4(n):
	for i in range(n):
		for j in range(i):
			print ' ',
		for j in range(n-i):
			print '* ',
		print ''

def line():
	l = ''
	for i in range(80):
		l+='-'
	print l

star1(10)
line()
star2(10)
line()
star3(10)
line()
star4(10)
