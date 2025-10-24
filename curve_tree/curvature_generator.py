import sys
import math
import numpy as np
import timeit


class Node:
    def __init__(self,data):
        self.children = []
        self.data = data
    def __str__(self):
        return f"{self.data}; {[n.data for n in self.children]}"


with open(sys.argv[1]) as f:
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

with open(sys.argv[2]) as f:
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


root_r = Node([1,0,0,0])
for i in range(len(base)):
    root_r.children.append(Node(base[i]))

def makechildren(root):
    for i in range(len(refl)):
        root.children.append(Node(i))

def reflect(c_1,c_2):
    inner_product = np.array(c_1) @ np.array([[0,-0.5,0,0],[-0.5,0,0,0],[0,0,1,0],[0,0,0,1]]) @ np.array(c_2).transpose()
    result = np.array(c_1)-2*inner_product*np.array(c_2)
    return result

def comp_lists(l_1,l_2,length):
    for i in range(length):
        if l_1[i] != l_2[i]:
            return False
    return True


curvatures = []
circles = []

limit = 1
scale = sys.argv[3]
if scale.find('r') == -1:
    scale = float(scale)
else:
    rat = scale[:scale.find('r')]
    rad = scale[scale.find('r')+1:]
    scale = float(rat)*math.sqrt(int(rad))

def find_reflections(root,level):
    if not comp_lists(root.data,[1,0,0,0],4) and not comp_lists(root.data,[0,0,0,0],4):
        curvatures.append(round(root.data[1]/scale))
        circles.append(root.data)
    if level > limit:
        return
    elif comp_lists(root.data,[0,0,0,0],4):
        return
    elif comp_lists(root.data,[1,0,0,0],4):
        for i in range(len(base)):
            find_reflections(root.children[i],level+1)
    else:
        makechildren(root)
        for i in range(len(refl)):
            root.children[i].data = reflect(root.data,refl[i])
            if comp_lists(root.children[i].data,root.data,4):
                root.children[i] = root
            find_reflections(root.children[i],level+1)


def list_mod(l,n):
    l1 = [x % n for x in l]
    l2 = list(set(l1))
    return l2

def legendre(a,p):
    #if p is prime:
    result = pow(a,((p-1)/2)) % p
    #if not we need to do something else
    #TODO: something else
    return result

start = timeit.default_timer()

find_reflections(root_r,0)

curvatures = list(set(curvatures))
curvatures.sort()

out_c = open('curvature.txt','w')
for i in range(len(curvatures)):
    out_c.write(str(curvatures[i])+'\n')
out_c.close()

out_p = open('circle_packing.txt','w')
for i in range(len(circles)):
    out_p.write(str(round(circles[i][0]*100000000)/100000000)+' ')
    out_p.write(str(round(circles[i][1]*100000000)/100000000)+' ')
    out_p.write(str(round(circles[i][2]*100000000)/100000000)+' ')
    out_p.write(str(round(circles[i][3]*100000000)/100000000)+'\n')
out_p.write('END')
out_p.close()


# print(curvatures)

# print('mod 2: ',list_mod(curvatures,2))
# print('mod 3: ',list_mod(curvatures,3))
# print('mod 4: ',list_mod(curvatures,4))
# print('mod 5: ',list_mod(curvatures,5))
# print('mod 7: ',list_mod(curvatures,7))
# print('mod 8: ',list_mod(curvatures,8))
# print('mod 9: ',list_mod(curvatures,9))
# print('mod 11: ',list_mod(curvatures,11))
# print('mod 13: ',list_mod(curvatures,13))
# print('mod 16: ',list_mod(curvatures,16))
# print('mod 24: ',list_mod(curvatures,24))
# print('mod 25: ',list_mod(curvatures,25))
# print('mod 32: ',list_mod(curvatures,32))


tangent_circles = []

def tan_circ_func_rec(root_1,root_2):
    if comp_lists(root_1.data,[1,0,0,0],4) or comp_lists(root_1.data,[0,0,0,0],4) or comp_lists(root_2.data,[1,0,0,0],4) or comp_lists(root_2.data,[0,0,0,0],4):
        return
    a = round(root_1.data[1]/scale)
    b = round(root_2.data[1]/scale)
    c = [a,b]
    c.sort()
    tangent_circles.append(str(c[0])+','+str(c[1]))
    if not root_1.children or not root_2.children:
        return
    else:
        for i in range(len(refl)):
            if not (root_1 == root_1.children[i] and root_2 == root_2.children[i]):
                tan_circ_func_rec(root_1.children[i],root_2.children[i])


def tan_circ_func(c_1,c_2):
    tan_circ_func_rec(root_r.children[c_1],root_r.children[c_2])

#oct: 0-2,0-3,0-4,0-5,1-2,1-3,1-4,1-5,2-3,2-4,3-5,4-5
if len(base) == 6:
    tan_circ_func(0,2)
    tan_circ_func(0,3)
    tan_circ_func(0,4)
    tan_circ_func(0,5)
    tan_circ_func(1,2)
    tan_circ_func(1,3)
    tan_circ_func(1,4)
    tan_circ_func(1,5)
    tan_circ_func(2,3)
    tan_circ_func(2,4)
    tan_circ_func(3,5)
    tan_circ_func(4,5)
#cube: 0-1,0-2,0-4,1-3,1-5,2-3,2-6,3-7,4-5,4-6,5-7,6-7
elif len(base) == 8:
    tan_circ_func(0,1)
    tan_circ_func(0,2)
    tan_circ_func(0,4)
    tan_circ_func(1,3)
    tan_circ_func(1,5)
    tan_circ_func(2,3)
    tan_circ_func(2,6)
    tan_circ_func(3,7)
    tan_circ_func(4,5)
    tan_circ_func(4,6)
    tan_circ_func(5,7)
    tan_circ_func(6,7)


tangent_circles = list(set(tangent_circles))
tangent_circles.sort()

tangent_circles_1 = [[int(x[:x.find(',')]),int(x[x.find(',')+1:])] for x in tangent_circles]

out_t = open('tangent_circles.txt','w')
for i in range(len(tangent_circles_1)):
    out_t.write(str(tangent_circles_1[i][0])+' '+str(tangent_circles_1[i][1])+'\n')
    out_t.write(str(tangent_circles_1[i][1])+' '+str(tangent_circles_1[i][0])+'\n')
out_t.close()

#print(tangent_circles_1)

tuples = []

def tuple_func_rec(roots,amt):
    for i in range(amt):
        if comp_lists(roots[i].data,[1,0,0,0],4):
            return
    a = []
    for i in range(amt):
        a.append(round(roots[i].data[1]/scale))
    b = ''
    for i in range(amt-1):
        b += str(a[i])+','
    b += str(a[amt-1])
    tuples.append(b)
    for i in range(amt):
        if not roots[i].children:
            return
    for j in range(len(refl)):
        for i in range(amt):
            if roots[i] == roots[i].children[j]:
                continue
        new_roots = []
        for i in range(amt):
            new_roots.append(roots[i].children[j])
        tuple_func_rec(new_roots,amt)

tuple_func_rec(root_r.children,6)

tuples = list(set(tuples))
tuples.sort()

out_t = open('tuples.txt','w')
for i in range(len(tuples)):
    out_t.write(tuples[i]+'\n')
out_t.close()


stop = timeit.default_timer()
print('Time: ', stop - start)