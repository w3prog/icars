import sys,os
path="out"
result="correct_result/simple"
if __name__ == "__main__":
    filename = sys.argv[1].split(".")[0]
    if not os.path.exists(path):
        os.mkdir(path)
    out_path="%s/%s.csv" % (path,filename)
    in_path="%s/%s.csv" % (result,filename.split(".")[0])
    with open(in_path,'r') as input, open(out_path,"w") as out:
        reader = (line for line in input )
        for line in reader:
            out.write(line)
    print(out_path)