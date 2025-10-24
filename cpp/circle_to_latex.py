import math
import sys

latex = open("circle_latex.txt","w")

latex.write("\\begin{tikzpicture}\n")

f = open("circle_packing_dual.txt")

# sqrt(2) for cube, sqrt(3) for triangle/hexagon, 1 otherwise
if sys.argv[1] == "cube":
    s = math.sqrt(2)
elif sys.argv[1] == "hexagon" or sys.argv[1] == "hexagon":
    s = math.sqrt(3)
else:
    s = 1

scale = int(sys.argv[2])

if sys.argv[3] == "F":
    flip = True
else:
    flip = False


while False:
    line = f.readline().strip()
    if line == 'END':
        break
    b_bar = line[:line.find(' ')]
    line = line[line.find(' ')+1:]
    b = line[:line.find(' ')]
    line = line[line.find(' ')+1:]
    h_1 = line[:line.find(' ')]
    h_2 = line[line.find(' ')+1:]

    # if round(float(h_1)*float(h_1)+float(h_2)*float(h_2)-1-float(b_bar)*float(b)) != 0:
    #     print("Error: "+b_bar+" "+b+" "+h_1+" "+h_2)

    if not flip:
        if float(b) != 0:
            x = float(h_1) / float(b)*s
            y = float(h_2) / float(b)*s
            r = 1 / float(b)*s
            if True:
                latex.write("\\draw[black, dashed] ("+str(x)+","+str(y)+") circle ("+str(r)+");\n")
        else:
            latex.write("\\draw[black, dashed] ("+str((float(h_1)*float(b_bar)/2-float(h_2)*3)*s)+","+str((float(h_2)*float(b_bar)/2+float(h_1)*3)*s)+") -- ("+str((float(h_1)*float(b_bar)/2+float(h_2)*3)*s)+","+str((float(h_2)*float(b_bar)/2-float(h_1)*3)*s)+");\n")
    else:
        if float(b) != 0:
            x = float(h_1) / float(b)*s
            y = float(h_2) / float(b)*s
            r = 1 / float(b)*s
            if True:
                latex.write("\\draw[black, dashed] ("+str(y*scale)+","+str(x*scale)+") circle ("+str(r)+");\n")
        else:
            latex.write("\\draw[black, dashed] ("+str((float(h_2)*float(b_bar)/2+float(h_1)*4)*s)+","+str((float(h_1)*float(b_bar)/2-float(h_2)*4)*s)+") -- ("+str((float(h_2)*float(b_bar)/2-float(h_1)*4)*s)+","+str((float(h_1)*float(b_bar)/2+float(h_2)*4)*s)+");\n")   

f.close()

d = open("circle_packing.txt")

while True:
    line = d.readline().strip()
    if line == 'END':
        break
    b_bar = line[:line.find(' ')]
    line = line[line.find(' ')+1:]
    b = line[:line.find(' ')]
    line = line[line.find(' ')+1:]
    h_1 = line[:line.find(' ')]
    h_2 = line[line.find(' ')+1:]


    # if round(float(h_1)*float(h_1)+float(h_2)*float(h_2)-1-float(b_bar)*float(b)) != 0:
    #     print("Error: "+b_bar+" "+b+" "+h_1+" "+h_2)

    if not flip:
        if float(b) != 0:
            x = float(h_1) / float(b)*s
            y = float(h_2) / float(b)*s
            r = 1 / float(b)*s
            if r*scale > 0 and r*scale < 0.02:
                continue
            if round(float(b)/s)==0:
                latex.write("\\draw[black, thick] ("+str(x*scale)+","+str(y*scale)+") circle ("+str(r*scale)+")node[scale="+str(r*scale*2)+"]{"+str(int(round(float(b)/s)))+"};\n")
            if float(b) > 0:
                latex.write("\\draw[black, thin] ("+str(x*scale)+","+str(y*scale)+") circle ("+str(r*scale)+")node[scale="+str(r*scale*2)+"]{"+str(int(round(float(b)/s)))+"};\n")
            else:
                latex.write("\\draw[black, thin] ("+str(x*scale)+","+str(y*scale)+") circle ("+str(r*scale)+");\n")
        else:
            latex.write("\\draw[black, thin] ("+str((float(h_1)*float(b_bar)/2-float(h_2)*3)*s)+","+str((float(h_2)*float(b_bar)/2+float(h_1)*3)*s)+") -- ("+str((float(h_1)*float(b_bar)/2+float(h_2)*3)*s)+","+str((float(h_2)*float(b_bar)/2-float(h_1)*3)*s)+");\n")
    else:
        if float(b) != 0:
            x = float(h_1) / float(b)*s
            y = float(h_2) / float(b)*s
            r = 1 / float(b)*s
            if r*scale > 0 and r*scale < 0.03:
                continue
            if round(float(b)/s)==0:
                latex.write("\\draw[black, thick] ("+str(y*scale)+","+str(x*scale)+") circle ("+str(r*scale)+")node[scale="+str(r*scale*2)+"]{"+str(int(round(float(b)/s)))+"};\n")
            elif float(b) > 0:
                latex.write("\\draw[black, thin] ("+str(y*scale)+","+str(x*scale)+") circle ("+str(r*scale)+")node[scale="+str(r*scale*2)+"]{"+str(int(round(float(b)/s)))+"};\n")
            else:
                latex.write("\\draw[black, thin] ("+str(y*scale)+","+str(x*scale)+") circle ("+str(r*scale)+");\n")
        else:
            latex.write("\\draw[black, thick] ("+str((float(h_2)*float(b_bar)/2+float(h_1)*4)*s)+","+str((float(h_1)*float(b_bar)/2-float(h_2)*4)*s)+") -- ("+str((float(h_2)*float(b_bar)/2-float(h_1)*4)*s)+","+str((float(h_1)*float(b_bar)/2+float(h_2)*4)*s)+");\n")   

d.close()

latex.write("\\end{tikzpicture}\n")

latex.close()