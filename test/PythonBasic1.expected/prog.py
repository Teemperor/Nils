R='''%d bottles of beer on the wall,
%o%d'''
b=9
while b>1:
    print(R%(b,b,1))
    b-=1