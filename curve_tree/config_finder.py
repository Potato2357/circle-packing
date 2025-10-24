import sys
import math
import numpy as np

ptype = sys.argv[1]

if ptype == "oct":
    basetxt = "oct.txt"
    dualtxt = "cube.txt"
    scale = 1
    name = "oct"
elif ptype == "cube":
    basetxt = "cube.txt"
    dualtxt = "oct.txt"
    scale = math.sqrt(2)
    name = "cube"
elif ptype == "cubeoct":
    basetxt = "cubeoct.txt"
    dualtxt = "rhodo.txt"
    scale = 1
    name = "cubeoct"
elif ptype == "square":
    basetxt = "square_base.txt"
    dualtxt = "square_dual.txt"
    scale = 1
    name = "square"
elif ptype == "square_ford":
    basetxt = "square_base_a.txt"
    dualtxt = "square_dual_a.txt"
    scale = 1
    name = "square"
elif ptype == "triangle":
    basetxt = "triangle_base.txt"
    dualtxt = "triangle_dual.txt"
    scale = 1
    name = "triangle"
elif ptype == "triangle_ford":
    basetxt = "triangle_base_a.txt"
    dualtxt = "triangle_dual_a.txt"
    scale = 1
    name = "triangle"
elif ptype == "hexagon":
    basetxt = "hexagon_base.txt"
    dualtxt = "hexagon_dual.txt"
    scale = math.sqrt(3)
    name = "hexagon"
elif ptype == "hexagon_ford":
    basetxt = "hexagon_base_a.txt"
    dualtxt = "hexagon_dual_a.txt"
    scale = math.sqrt(3)
    name = "hexagon"
else:
    print("error")
    sys.exit(0)

with open(basetxt) as f:
    width, height = [x for x in next(f).split()]
    base = [[x for x in line.split()] for line in f]

for i in range(len(base)):
    for j in range(len(base[0])):
        if base[i][j].find('r') == -1:
            base[i][j] = float(base[i][j])
        else:
            rat = base[i][j][:base[i][j].find('r')]
            rad = base[i][j][base[i][j].find('r')+1:]
            base[i][j] = float(rat)*math.sqrt(int(rad))

with open(dualtxt) as f:
    width, height = [x for x in next(f).split()]
    refl = [[x for x in line.split()] for line in f]

for i in range(len(refl)):
    for j in range(len(refl[0])):
        if refl[i][j].find('r') == -1:
            refl[i][j] = float(refl[i][j])
        else:
            rat = refl[i][j][:refl[i][j].find('r')]
            rad = refl[i][j][refl[i][j].find('r')+1:]
            refl[i][j] = float(rat)*math.sqrt(int(rad))

def reflect(c_1,c_2):
    inner_product = np.array(c_1) @ np.array([[0,-0.5,0,0],[-0.5,0,0,0],[0,0,1,0],[0,0,0,1]]) @ np.array(c_2).transpose()
    result = np.array(c_1)-2*inner_product*np.array(c_2)
    return result

result_b = []
result_r = []

def reflect_all(ref):
    for i in range(len(base)):
        result_b.append(reflect(base[i],ref))
    for i in range(len(refl)):
        result_r.append(reflect(refl[i],ref))

##############################################

print('Current curatures:')
for i in range(len(base)):
    print(i, round(base[i][1]/scale*10000000)/10000000)

end = False

ans = input('Circle to reflect across: ')

while (end == False):
    reflect_all(base[int(ans)])

    print('Current curatures:')
    for i in range(len(result_b)):
        print(i, round(result_b[i][1]/scale*10000000)/10000000)

    ans = input('Circle to reflect across (\'n\' to end): ')
    if ans == 'n':
        end = True
        break
    else:
        base = result_b
        refl = result_r
        result_b = []
        result_r = []

##############################################

num = input('Config number: ')

out_b = open('../configs/'+name+'_'+num+'_base.txt','w')
out_b.write(str(len(result_b))+'\n')
for i in range(len(result_b)):
    if basetxt == "oct.txt":
        out_b.write(str(int(round(result_b[i][0])))+' ')
        out_b.write(str(int(round(result_b[i][1])))+' ')
        if result_b[i][2]!= 0:
            out_b.write(str(int(round(result_b[i][2]/math.sqrt(2))))+'r2 ')
        else:
            out_b.write('0 ')
        out_b.write(str(int(round(result_b[i][3])))+' ')
    elif basetxt == "cube.txt":
        if result_b[i][0]!= 0:
            out_b.write(str(int(round(result_b[i][0]/math.sqrt(2))))+'r2 ')
        else:
            out_b.write('0 ')
        if result_b[i][1]!= 0:
            out_b.write(str(int(round(result_b[i][1]/math.sqrt(2))))+'r2 ')
        else:
            out_b.write('0 ')
        out_b.write(str(int(round(result_b[i][2])))+' ')
        if result_b[i][3]!= 0:
            out_b.write(str(int(round(result_b[i][3]/math.sqrt(2))))+'r2 ')
        else:
            out_b.write('0 ')
    elif basetxt == "cubeoct.txt":
        out_b.write(str(int(round(result_b[i][0])))+' ')
        out_b.write(str(int(round(result_b[i][1])))+' ')
        if result_b[i][2]!= 0:
            out_b.write(str(int(round(result_b[i][2]/math.sqrt(6))))+'r6 ')
        else:
            out_b.write('0 ')
        out_b.write(str(int(round(result_b[i][3])))+' ')
    elif basetxt == "square_base.txt" or basetxt == "square_base_a.txt":
        out_b.write(str(int(round(result_b[i][0])))+' ')
        out_b.write(str(int(round(result_b[i][1])))+' ')
        out_b.write(str(int(round(result_b[i][2])))+' ')
        out_b.write(str(int(round(result_b[i][3])))+' ')
    elif basetxt == "triangle_base.txt" or basetxt == "triangle_base_a.txt":
        out_b.write(str(int(round(result_b[i][0])))+' ')
        out_b.write(str(int(round(result_b[i][1])))+' ')
        out_b.write(str(int(round(result_b[i][2])))+' ')
        if result_b[i][3]!= 0:
            out_b.write(str(int(round(result_b[i][3]/math.sqrt(3))))+'r3 ')
        else:
            out_b.write('0 ')
    elif basetxt == "hexagon_base.txt" or basetxt == "hexagon_base_a.txt":
        if result_b[i][0]!= 0:
            out_b.write(str(int(round(result_b[i][0]/math.sqrt(3))))+'r3 ')
        else:
            out_b.write('0 ')
        if result_b[i][1]!= 0:
            out_b.write(str(int(round(result_b[i][1]/math.sqrt(3))))+'r3 ')
        else:
            out_b.write('0 ')
        if result_b[i][2]!= 0:
            out_b.write(str(int(round(result_b[i][2]/math.sqrt(3))))+'r3 ')
        else:
            out_b.write('0 ')
        out_b.write(str(int(round(result_b[i][3])))+' ')
    out_b.write('\n')
out_b.close()

out_r = open('../configs/'+name+'_'+num+'_dual.txt','w')
out_r.write(str(len(result_r))+'\n')
for i in range(len(result_r)):
    if basetxt == "cube.txt":
        out_r.write(str(int(round(result_r[i][0])))+' ')
        out_r.write(str(int(round(result_r[i][1])))+' ')
        if result_r[i][2]!= 0:
            out_r.write(str(int(round(result_r[i][2]/math.sqrt(2))))+'r2 ')
        else:
            out_r.write('0 ')
        out_r.write(str(int(round(result_r[i][3])))+' ')
    elif basetxt == "oct.txt":
        if result_r[i][0]!= 0:
            out_r.write(str(int(round(result_r[i][0]/math.sqrt(2))))+'r2 ')
        else:
            out_r.write('0 ')
        if result_r[i][1]!= 0:
            out_r.write(str(int(round(result_r[i][1]/math.sqrt(2))))+'r2 ')
        else:
            out_r.write('0 ')
        out_r.write(str(int(round(result_r[i][2])))+' ')
        if result_r[i][3]!= 0:
            out_r.write(str(int(round(result_r[i][3]/math.sqrt(2))))+'r2 ')
        else:
            out_r.write('0 ')
    elif basetxt == "cubeoct.txt":
        if result_r[i][0]!= 0:
            out_r.write(str(int(round(result_r[i][0]/math.sqrt(6))))+'r6 ')
        else:
            out_r.write('0 ')
        if result_r[i][1]!= 0:
            if round(result_r[i][1]/math.sqrt(6)*2)%2==1:
                out_r.write(str(round(result_r[i][1]/math.sqrt(6)*100)/100)+'r6 ')
            else:
                out_r.write(str(int(round(result_r[i][1]/math.sqrt(6))))+'r6 ')
        else:
            out_r.write('0 ')
        out_r.write(str(int(round(result_r[i][2])))+' ')
        if result_r[i][3]!= 0:
            out_r.write(str(int(round(result_r[i][3]/math.sqrt(6))))+'r6 ')
        else:
            out_r.write('0 ')
    elif basetxt == "square_base.txt" or basetxt == "square_base_a.txt":
        out_r.write(str(int(round(result_r[i][0])))+' ')
        if round(result_r[i][1]*2)%2==1:
            out_r.write(str(round(result_r[i][1]*100)/100)+' ')
        else:
            out_r.write(str(int(round(result_r[i][1])))+' ')
        out_r.write(str(int(round(result_r[i][2])))+' ')
        out_r.write(str(int(round(result_r[i][3])))+' ')
    elif basetxt == "triangle_base.txt" or basetxt == "triangle_base_a.txt":
        if result_r[i][0]!= 0:
            out_r.write(str(int(round(result_r[i][0]/math.sqrt(3))))+'r3 ')
        else:
            out_r.write('0 ')
        if result_r[i][1]!= 0:
            if round(result_r[i][1]/math.sqrt(3)*2)%2==1:
                out_r.write(str(round(result_r[i][1]/math.sqrt(3)*100)/100)+'r3 ')
            else:
                out_r.write(str(int(round(result_r[i][1]/math.sqrt(3))))+'r3 ')
        else:
            out_r.write('0 ')
        if result_r[i][2]!= 0:
            if round(result_r[i][2]/math.sqrt(3)*2)%2==1:
                out_r.write(str(round(result_r[i][2]/math.sqrt(3)*100)/100)+'r3 ')
            else:
                out_r.write(str(int(round(result_r[i][2]/math.sqrt(3))))+'r3 ')
        else:
            out_r.write('0 ')
        if round(result_r[i][3]*2)%2==1:
            out_r.write(str(round(result_r[i][3]*100)/100)+' ')
        else:
            out_r.write(str(int(round(result_r[i][3])))+' ')
    elif basetxt == "hexagon_base.txt" or basetxt == "hexagon_base_a.txt":
        out_r.write(str(int(round(result_r[i][0])))+' ')
        if round(result_r[i][1]*2)%2==1:
            out_r.write(str(round(result_r[i][1]*100)/100)+' ')
        else:
            out_r.write(str(int(round(result_r[i][1])))+' ')
        if round(result_r[i][2]*2)%2==1:
            out_r.write(str(round(result_r[i][2]*100)/100)+' ')
        else:
            out_r.write(str(int(round(result_r[i][2])))+' ')
        if result_r[i][3]!= 0:
            if round(result_r[i][3]/math.sqrt(3)*2)%2==1:
                out_r.write(str(round(result_r[i][3]/math.sqrt(3)*100)/100)+'r3 ')
            else:
                out_r.write(str(int(round(result_r[i][3]/math.sqrt(3))))+'r3 ')
        else:
            out_r.write('0 ')
    out_r.write('\n')
out_r.close()




# def add_circle(x,y,r,c):
#     c = plt.Circle((x, y), r, fill=False, color = c)
#     plt.gca().add_patch(c)

# for i in range(len(result)):
#     x = result[i][2] / result[i][0]
#     y = result[i][3] / result[i][0]
#     r = 1 / result[i][0]
#     add_circle(x,y,r,'k')

# axs = plt.gca()
# axs.axis('equal')
# plt.show()