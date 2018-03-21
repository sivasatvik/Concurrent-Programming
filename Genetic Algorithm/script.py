import subprocess
import os
subprocess.call('ls')

# num_proc = [4, 8, 2, 1]
seed = [11]
cities = [20, 50, 100, 250, 500, 750, 1000]
pop = [20]
gen = [5000]

subprocess.call('pwd', shell=True)
# for i in num_proc:
for j in seed:
	for k in cities:
		for l in pop:
			for g in gen:
				command = 'g++ --std=c++17 -pthread lazylist.cpp tsp.cpp main.cpp -o main.o -DSEED='+str(j)+" -DC="+str(k)+" -DPOP="+str(l)+" -DGEN="+str(g)+" -DINF=9999"
				print command
				subprocess.call(command, shell=True)
				subprocess.call("./main.o", shell=True)
os.chdir('../../../plain/')
subprocess.call('pwd', shell=True)
# for i in num_proc:
for j in seed:
	for k in cities:
		for l in pop:
			for g in gen:
				command = 'g++ -g --std=c++17 tsp.cpp main.cpp -o main.o -DSEED='+str(j)+" -DC="+str(k)+" -DPOP="+str(l)+" -DGEN="+str(g)+" -DINF=9999"
				print command
				subprocess.call(command, shell=True)
				subprocess.call("./main.o", shell=True)
