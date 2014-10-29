import subprocess,sys,time,optparse

def main(runtime,hashStart,hashEnd,hashInc,distinct,allStart,allEnd,allInc,numThreads):	
	subprocess.call(["rm","-rf","./data/"])
	subprocess.call(["mkdir","./data/"])
	subprocess.call(["mkdir","./data/pbfResult"])
	subprocess.call(["mkdir","./data/time"])
	f = open("data/time/all_time.txt","w+")
	line_title = "numdistinctKeys,numallKeys,numHash,Runtime,NumThread,ReguarTime,OpenMPTime,GPUTime"
	f.write(line_title+"\n")
	#vary the number of total keys
	for numallKeys in range(allStart,allEnd+allInc,allInc):
		#Remove previous input data 
		subprocess.call(["rm","./data/total_keys.txt"])
		subprocess.call(["rm","./data/distinct_keys.txt"])
		subprocess.call(["rm","./data/freq.txt"])
		#Generate input data.
		subprocess.call(["./generate.exe","--distinct",str(distinct),"--total",str(numallKeys)])
		averFreq = numallKeys/distinct	
		#vary number of hash functions
		for numHash in range(hashStart,hashEnd+hashInc,hashInc):
			for i in range(1,runtime+1):
				#Setup the basic arguments.				
				basicArgs = ["-n",str(numallKeys),"-k",str(numHash),"-f",str(averFreq)]				
				# CPU Regular 
				regArgs = ["./pbfreg.exe","--pbfOutput","./data/pbfResult/reg_n_"+str(numallKeys)+"_k_"+str(numHash)+"_r_"+str(i)]
				regRun = []
				regRun[len(regRun):] = regArgs
				regRun[len(regRun):] = basicArgs
				startReg = time.time()
				regProcess = subprocess.call(regRun)
				endReg = time.time()
				regTime = endReg - startReg

				# CPU OpenMP 
				ompArgs = ["./pbfomp.exe","--pbfOutput","./data/pbfResult/omp_n_"+str(numallKeys)+"_k_"+str(numHash)+"_r_"+str(i)]
				ompRun = []
				ompRun[len(ompRun):] = ompArgs
				ompRun[len(ompRun):] = basicArgs
				ompRun[len(ompRun):] = ["-t",str(numThreads)]
				startOmp = time.time()
				ompProcess = subprocess.call(ompRun)
				endOmp = time.time()
				ompTime = endOmp - startOmp

				# GPU run
				pbfArgs = ["./pbfgpu.exe","--pbfOutput","./data/pbfResult/gpu_n_"+str(numallKeys)+"_k_"+str(numHash)+"_r_"+str(i)]
				pbfRun = []
				pbfRun[len(pbfRun):] = pbfArgs
				pbfRun[len(pbfRun):] = basicArgs	
				startPbf = time.time()		
				pbfProcess = subprocess.call(pbfRun)
				endPbf = time.time()
				gpuTime = endGpu - startGpu
				#gpuTime = 0
				
				line = str(distinct)+","+str(numallKeys)+","+str(numHash)+","+str(i)+","+str(regTime)+","+str(ompTime)+","+str(gpuTime)
				#print line+"\n" 
				f.write(line+"\n")
				line = "Update: numallKeys = "+str(numallKeys)+", numHash = "+str(numHash)+", runtime = "+str(i)
				print line
	f.close()

if __name__=="__main__":
	runtime = 2
	hashStart = 200
	hashEnd = 300
	hashInc = 100
	distinct = 50
	allStart = 1500
	allEnd = 2500
	allInc= 1000
	numThreads = 4
	parser = optparse.OptionParser()
	parser.add_option("--runtime")
	parser.add_option("--hashStart")
	parser.add_option("--hashEnd")
	parser.add_option("--hashInc")
	parser.add_option("--distinct")
	parser.add_option("--allStart")
	parser.add_option("--allEnd")
	parser.add_option("--allInc")
	parser.add_option("--numThreads")	
	(options,args) = parser.parse_args()
	if(options.runtime is not None):
		runtime = int(options.runtime)
	if(options.hashStart is not None):
		hashStart = int(options.hashStart)
	if(options.hashEnd is not None):
		hashEnd = int(options.hashEnd)
	if(options.hashInc is not None):
		hashInc = int(options.hashInc)
	if(options.distinct is not None):
		distinc = int(options.distinct)	
	if(options.allStart is not None):
		allStart = int(options.allStart)
	if(options.allEnd is not None):
		allEnd = int(options.allEnd)
	if(options.allInc is not None):
		allInc = int(options.allInc)
	if(options.numThreads is not None):
		numThreads = int(options.numThreads)	
	main(runtime=runtime,hashStart= hashStart,hashEnd=hashEnd,hashInc=hashInc,distinct=distinct,
allStart=allStart,allEnd=allEnd,allInc=allInc,numThreads=numThreads)
