import sys
import re

def readIOU():
    d = dict()
    for line in open('../oui.txt','r').readlines():
        if '(hex)' not in line: 
            continue
        f = line.split(' ', 1)
        #print(f[0])
        c = f[1].split('\t\t', 1)
        #print(c[1])
        d[f[0]] = c[1] 

    return d


def main(argv):
    macAddressList = readIOU()
    mac = argv[1]
    matchObj = re.match("^([0-9A-Fa-f]{2}[:-]){2}([0-9A-Fa-f]{2})", mac, 0)
    if matchObj:
        #print(matchObj.group())
        if mac in macAddressList:
            print(macAddressList[mac])
        else:
            print('Manufacturer not found for MAC address: ' + mac)

    else:
        print('Not a MAC address')
    

if __name__ == "__main__":
    main(sys.argv)
