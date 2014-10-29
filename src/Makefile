#NVCC = /usr/local/cuda/bin/nvcc

make generate: data/ bin/ generate.exe

make reg: data/ bin/ pbfreg.exe 

#make omp: data/ bin/ bin/pbfomp.exe

#make gpu: data/ bin/ bin/pbfgpu.exe


data/:
	mkdir ../data
	mkdir ../data/pbfResult
	mkdir data/time

bin/: 
	mkdir bin

bin/RandomGenerator.o: RandomGenerator.h RandomGenerator.cpp
	g++ -c RandomGenerator.cpp
	mv RandomGenerator.o bin/

bin/ParseArgs.o: ParseArgs.cpp ParseArgs.h
	g++ -c ParseArgs.cpp
	mv ParseArgs.o bin/

bin/ParseData.o: ParseData.cpp ParseData.h
	g++ -c ParseData.cpp
	mv ParseData.o bin/

bin/Hash.o: Hash.cpp Hash.h
	g++ -c Hash.cpp
	mv Hash.o bin/

bin/PBFStats.o: PBFStats.cpp PBFStats.h
	g++ -c PBFStats.cpp
	mv PBFStats.o bin/

bin/generate.o: GenerateData/generate.cpp 
	g++ -c GenerateData/generate.cpp
	mv generate.o bin/

bin/pbfreg.o: Regular/pbfreg.cpp
	g++ -c Regular/pbfreg.cpp
	mv pbfreg.o bin/

generate.exe: bin/generate.o bin/RandomGenerator.o bin/ParseArgs.o
	g++ bin/generate.o bin/RandomGenerator.o bin/ParseArgs.o -o generate.exe

pbfreg.exe: bin/pbfreg.o bin/ParseArgs.o bin/ParseData.o bin/Hash.o bin/PBFStats.o
	g++ bin/pbfreg.o bin/ParseArgs.o bin/ParseData.o bin/Hash.o bin/PBFStats.o -o pbfreg.exe
	
clean:
	rm -rf bin
	rm -rf data
	rm *.txt	
