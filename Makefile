#NVCC = /usr/local/cuda/bin/nvcc

make generate: bin/ generate.exe

make reg: bin/ pbfreg.exe 

make omp: bin/ pbfomp.exe

#make gpu: bin/ pbfgpu.exe

bin/: 
	mkdir bin

bin/RandomGenerator.o: src/RandomGenerator.h src/RandomGenerator.cpp
	g++ -c src/RandomGenerator.cpp
	mv RandomGenerator.o bin/

bin/ParseArgs.o: src/ParseArgs.cpp src/ParseArgs.h
	g++ -c src/ParseArgs.cpp
	mv ParseArgs.o bin/

bin/ParseData.o: src/ParseData.cpp src/ParseData.h
	g++ -c src/ParseData.cpp
	mv ParseData.o bin/

bin/Hash.o: src/Hash.cpp src/Hash.h
	g++ -c src/Hash.cpp
	mv Hash.o bin/

bin/PBFStats.o: src/PBFStats.cpp src/PBFStats.h
	g++ -c src/PBFStats.cpp
	mv PBFStats.o bin/

bin/generate.o: src/GenerateData/generate.cpp 
	g++ -c src/GenerateData/generate.cpp
	mv generate.o bin/

bin/pbfreg.o: src/Regular/pbfreg.cpp
	g++ -c src/Regular/pbfreg.cpp
	mv pbfreg.o bin/

bin/pbfomp.o: src/OpenMP/pbfomp.cpp
	g++ -c src/OpenMP/pbfomp.cpp -fopenmp
	mv pbfomp.o bin/

#bin/bloom.o: src/GPU/bloom.cu src/GPU/Bloom.h
#	$(NVCC) -arch=sm_11 -c src/GPU/bloom.cu 
#	mv bloom.o bin/

#bin/pbfgpu.o: src/GPU/pbfgpu.cpp
#	$(NVCC) -c src/GPU/pbfgpu.cpp
#	mv pbfgpu.o bin/

generate.exe: bin/generate.o bin/RandomGenerator.o bin/ParseArgs.o
	g++ bin/generate.o bin/RandomGenerator.o bin/ParseArgs.o -o generate.exe

pbfreg.exe: bin/pbfreg.o bin/ParseArgs.o bin/ParseData.o bin/Hash.o bin/PBFStats.o
	g++ bin/pbfreg.o bin/ParseArgs.o bin/ParseData.o bin/Hash.o bin/PBFStats.o -o pbfreg.exe

pbfomp.exe: bin/pbfomp.o bin/ParseArgs.o bin/ParseData.o bin/Hash.o bin/PBFStats.o
	g++ bin/pbfomp.o bin/ParseArgs.o bin/ParseData.o bin/Hash.o bin/PBFStats.o -o pbfomp.exe -fopenmp

#pbfgpu.exe: bin/pbfgpu.o bin/bloom.o bin/ParseArgs.o bin/ParseData.o bin/PBFStats.o
#	$(NVCC) -arch=sm_11  bin/pbfgpu.o bin/bloom.o  bin/ParseArgs.o bin/ParseData.o bin/PBFStats.o -o pbfgpu.exe

clean:
	rm -rf bin	
