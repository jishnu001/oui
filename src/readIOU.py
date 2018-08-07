import sys
import re
import threading

macAddressList = dict()


class myThread (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter
    def run(self):
        readIOU()

def readIOU():
     
    for line in open('../oui.txt','r').readlines():
        if '(hex)' not in line: 
            continue
        f = line.split(' ', 1)
        #print(f[0])
        c = f[1].split('\t\t', 1)
        #print(c[1])
        macAddressList[f[0]] = c[1] 

        #print(c[1])

    #return d


def main(argv):

    thread1 = myThread(1, "Thread-1", 1)
    thread1.start()

    # thread.start_new_thread(readIOU, ('', ''))
     
    mac = argv[1]
    mac = mac.upper()
    mac = mac.replace(':', '-')

    matchObj = re.match("^([0-9A-Fa-f]{2}[:-]){2}([0-9A-Fa-f]{2})", mac[:8], 0)
    
    if matchObj:
        #print(matchObj.group())
        thread1.join()

        oui = mac[:8]

        if oui in macAddressList:
            print(macAddressList[oui])
        else:
            print('Manufacturer not found for MAC address: ' + mac)

    else:
        print('Not a MAC address')
    

if __name__ == "__main__":
    main(sys.argv)
