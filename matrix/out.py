entrada = open("tempos.txt", "r")

i = 0
for l in entrada:
    if i == 0: 
        print(l)
    elif i == 1: 
        serial = float(l)
        print(serial)
    else:
        print(serial / float(l))
    i += 1
    if i == 11:
        i = 0

