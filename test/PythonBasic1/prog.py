R='%d%o%d bottles of beer on the wall'''
b=99
while b>1:
    print(R%(b,b,b-1))
    b-=1
