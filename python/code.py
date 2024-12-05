import sys
import random

def main():
    u = random.randint(1, 10)      
    r = random.randint(0, 10000) 
    a  = [0] * 10000             
    for i in range(10000):       
      for j in range(100000):    
        a[i] += j%u        
      a[i] += r           


if __name__ == "__main__":
    import time 
    start = time.time()
    main()
    duration = time.time() - start
    print(f"Python 用时: {duration}s")