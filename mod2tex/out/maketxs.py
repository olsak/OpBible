#!/usr/bin/python3
import sys
import os

if(len(sys.argv)<2):
    print("missing argument")
    exit(1)
    
filename = sys.argv[1]
buff = []
buffname = None

mainname,_ = os.path.splitext(filename)
# filename.split(".")[0]
mainname = os.path.basename(mainname)

def writeBuff():
    with open(mainname+"-"+buffname+".txs",'w', encoding = 'utf-8') as f_buff:
        for buff_line in buff:
            f_buff.write(buff_line)
        
books = []
abbvs = []
inside_fence = False

with open(filename,'r', encoding = 'utf-8') as f:
    for iline,line in enumerate(f):
        if(line[:6] == "\kniha"):
            assert inside_fence == False, iline+1
            if(buffname):
                writeBuff()
            buff = []
            book = line.split('{')[1][:-2]
            buff.append("% source: "+filename+", book: "+book+"\n")
            books.append(book)
        elif(line[:8] == "\zkratka"):
            assert inside_fence == False, iline+1
            buffname = line.split('{')[1][:-2]
            abbvs.append(buffname)
        else:
            line = line.strip()
            if line == "": continue
            if line.startswith("<"):
                assert inside_fence == False, iline+1
                inside_fence = True
                line = "#"+line[1:]
            assert inside_fence == True, iline+1
            if line.endswith(">"):
                inside_fence = False
                line = line[:-1]+"\n"
            else: line = line+' '
            buff.append(line)
    assert inside_fence == False
    writeBuff()

with open(mainname+"-books.tex", "w", encoding = 'utf-8') as f_books:
    for book,abbv in zip(books,abbvs):
        f_books.write("\\BookTitle "+abbv+" "+abbv+" {"+book+"}\n")
        