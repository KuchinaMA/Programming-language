all: Complier Processor

Complier:
	cd Complier && make && cd ..

Processor:
	cd Processor && make && cd ..

.PHONY: Complier Processor

run:
	cd Complier/ && ./ComplierRes.exe
	cd Processor/ && ./ProcessorRes.exe

clean:
	cd Complier && make clean && cd ..
	cd Processor && make clean && cd ..