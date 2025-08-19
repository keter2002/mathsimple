'''
Fibonacci sequence formula.
'''

def fibo(n):
    return (((1+(5**(1/2)))**n) - ((1-(5**(1/2)))**n)) / ((2**n)*(5**(1/2)))
