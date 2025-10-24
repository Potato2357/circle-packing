from matplotlib.font_manager import FontProperties
import matplotlib.pyplot as plt
from matplotlib.textpath import TextPath
from matplotlib.patches import PathPatch
import math


fp = FontProperties(family='YuGothic')

def add_circle(x,y,r,c):
    c = plt.Circle((x, y), r, fill=False, color = c)
    plt.gca().add_patch(c)

def add_v_line(x,c):
    plt.plot([x,x],[0,3], color = c)

def add_h_line(y,c):
    plt.plot([-2,2],[y,y], color = c)

def add_d_line(d,h_1,h_2,c):
    plt.plot([h_1*d-h_2*4,h_1*d+h_2*4],[h_2*d+h_1*4,h_2*d-h_1*4], color = c)

def add_circle_fill(x,y,r,c):
    c = plt.Circle((x, y), r, fill=True, color = c)
    plt.gca().add_patch(c)


f = open('circle_packing_dual.txt')

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

    if round(float(h_1)*float(h_1)+float(h_2)*float(h_2)-1-float(b_bar)*float(b)) != 0:
        print('Error: '+b_bar+' '+b+' '+h_1+' '+h_2)

    if float(b) != 0:
        x = float(h_1) / float(b)
        y = float(h_2) / float(b)
        r = 1 / float(b)
        add_circle(x,y,r,'r')
    else:
        add_d_line(float(b_bar)/2,float(h_1),float(h_2),'r')
f.close()

d = open('circle_packing.txt')

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


    if round(float(h_1)*float(h_1)+float(h_2)*float(h_2)-1-float(b_bar)*float(b)) != 0:
        print('Error: '+b_bar+' '+b+' '+h_1+' '+h_2)

    if float(b) != 0:
        x = float(h_1) / float(b)
        y = float(h_2) / float(b)
        r = 1 / float(b)
        if True:
            add_circle(x,y,r,'k')
            if r>0.001:
                #remember to change this
                # tp = TextPath((x-r/3,y-r/4), str(round(float(b))), size=r*0.7, prop=fp)
                # tp = TextPath((x-r/3,y-r/4), str(round(float(b)/math.sqrt(2))), size=r*0.7, prop=fp)
                tp = TextPath((x-r/3,y-r/4), str(round(float(b)/math.sqrt(3))), size=r*0.7, prop=fp)
                # tp = TextPath((x-r/3,y-r/4), str(round(float(b)/math.sqrt(6)*10)/10), size=r*0.7, prop=fp)
                plt.gca().add_patch(PathPatch(tp, color='black'))
    else:
        add_d_line(float(b_bar)/2,float(h_1),float(h_2),'k')
d.close()

axs = plt.gca()
axs.axis('equal')
plt.show()