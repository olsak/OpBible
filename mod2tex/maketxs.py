#!/usr/bin/python3
import sys

if(len(sys.argv)<2):
    print("missing argument")
    exit(1)
    
filename = sys.argv[1]
buff = []
buffname = None

mainname = filename.split(".")[0]

def writeBuff():
    with open(mainname+"-"+buffname+".txs",'w', encoding = 'utf-8') as f_buff:
        for buff_line in buff:
            f_buff.write(buff_line)
        
books = []
abbvs = []


with open(filename,'r', encoding = 'utf-8') as f:
    for line in f:
        if(line[:6] == "\kniha"):
            if(buffname):
                writeBuff()
            buff = []
            book = line.split('{')[1][:-2]
            buff.append("% source: "+filename+", book: "+book+"\n")
            books.append(book)
        elif(line[:8] == "\zkratka"):
            buffname = line.split('{')[1][:-2]
            abbvs.append(buffname)
        elif(len(line)<2):
            continue
        else:
            buff.append("#"+line[1:-2]+"\n")
    writeBuff()

with open(mainname+"-books.tex", "w", encoding = 'utf-8') as f_books:
    for book,abbv in zip(books,abbvs):
        f_books.write("\\BookTitle "+abbv+" {"+book+"}\n")
        