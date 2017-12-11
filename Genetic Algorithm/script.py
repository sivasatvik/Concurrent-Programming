import subprocess
import os
subprocess.call('ls')

num_proc = [4, 8, 2, 1]
seed = [7, 11, 31]
cities = [20, 50, 100, 250, 500, 750, 1000]
pop = [5, 10, 20, 40]
gen = [1000, 5000, 10000]

subprocess.call('pwd', shell=True)
for i in num_proc:
	for j in seed:
		for k in cities:
			for l in pop:
				for g in gen:
					command = 'g++ -g --std=c++14 -pthread tsp.cpp main.cpp -o main.o -DN='+str(i)+" -DSEED="+str(j)+" -DC="+str(k)+" -DPOP="+str(l)+" -DGEN="+str(g)
					print command
					subprocess.call(command, shell=True)
					subprocess.call("./main.o", shell=True)
os.chdir('../plain/')
subprocess.call('pwd', shell=True)
for i in num_proc:
	for j in seed:
		for k in cities:
			for l in pop:
				for g in gen:
					command = 'g++ -g --std=c++14 -pthread tsp.cpp main.cpp -o main.o -DN='+str(i)+" -DSEED="+str(j)+" -DC="+str(k)+" -DPOP="+str(l)+" -DGEN="+str(g)
					print command
					subprocess.call(command, shell=True)
					subprocess.call("./main.o", shell=True)
