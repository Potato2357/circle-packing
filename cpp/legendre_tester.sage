f = open("tangencies.txt")

has_zero = False
has_neg_one = False
has_one = False

out_t = open('tangencies_kronecker.txt','w')

while True:
    line = f.readline().strip()
    if line == '':
        break
    a = float(line[:line.find(' ')])
    b = float(line[line.find(' ')+1:])
    if a == 0 or b == 0:
        continue


    if a%4==1:
        chi = kronecker(a+b,a)
    elif a%4==3:
        chi = kronecker(-a-b,a)
    else:
        chi = 1000

    if chi == 0:
        has_zero = True
    elif chi == 1:
        has_one = True
    elif chi == -1:
        has_neg_one = True
    if chi != 0:
        out_t.write('{:<15s} {:<10s}'.format(str(int(a))+' '+str(int(b)),str(chi))+'\n')
    


out_t.close()

if has_zero:
    print("Some of the tangent circles have kronecker symbol 0")
if has_neg_one:
    print("Some of the tangent circles have kronecker symbol -1")
if has_one:
    print("Some of the tangent circles have kronecker symbol 1")