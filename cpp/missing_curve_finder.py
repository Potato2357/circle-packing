import sys

def read_num(path, maxn):
    nums = []
    with open(path, 'r') as file:
        for line in file:
            n = int(line.strip())
            if n < maxn:
                nums.append(n)
            else:
                break
    return nums

# input: <type> <packing number> <obstructions> <modulus> <N>
packing = '../packings/' + sys.argv[1] + '_' + sys.argv[2] + '_curvatures.txt'
obs1 = sys.argv[3]
mod = sys.argv[4]
maxtest = int(sys.argv[5])

obs = [i for i in obs1.split(',')]

comp = set()
for i in obs:
    ref = '../references/reference_'+i+'_mod_'+mod+'.txt'
    read = read_num(ref,maxtest)
    comp.update(set(read))

l = []
with open(packing, 'r') as file:
    l = file.readlines()
l = [int(n.strip()) for n in l]
pck = set(l)

missing = comp - pck

if sys.argv[6] == 'x':
    has_obs = True
else:
    has_obs = False

if has_obs:
    squares = set()
    read = read_num('../references/squares.txt',maxtest)
    squares.update(set(read))
    # read = read_num('../references/squares_two.txt',maxtest)
    # squares.update(set(read))
    # read = read_num('../references/squares_three.txt',maxtest)
    # squares.update(set(read))
    missing = missing - squares

if len(missing) == 0:
    print('No missing curvatures')
else:
    print("Max obstruction: " + str(max(missing)))
    print("Number of obstructions: " + str(len(missing)))
    print("The third thing: " + str(maxtest/max(missing)))


# print(sorted(missing))