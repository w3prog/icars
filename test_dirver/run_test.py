import os, sys
import shutil
from subprocess import Popen, PIPE
PATH_TO_PROGRAM="stub.py"
MODE="simple"
IMAGE_DIR='images/%s' % MODE
VALID_DIR='correct_result/%s' % MODE
for img in os.listdir(IMAGE_DIR):
   print("wirking with %s" % img)
   p=Popen("%s %s" % (PATH_TO_PROGRAM, img), stdout=PIPE, shell=True)
   stdoutdata, stderrdata = p.communicate()

   value = stdoutdata.replace(b"\n",b"").replace(b"\r",b"")
   with open("%s//%s.csv" % (VALID_DIR,img.split(".")[0]),'r') as valid, open(value,"r") as result :
        valid_reader = (line for line in valid)
        res_reader = (line for line in result)
        first_line_v=next(valid_reader)
        first_line_r=next(res_reader)
        if (first_line_r != first_line_v):
            print("wrong on %s with filename or count road signs" % img )
            sys.exit()
        count_lines = int(first_line_r.split(";")[-1])
        
        valid_dict=[]
        for i in valid_reader:
            valid_dict.append(i)
        
        res_dict=[]
        for i in res_reader:
            res_dict.append(i)
        print("correct")

if os.path.exists('out'):
        shutil.rmtree('out')