all: Frontend Middleend Complier Processor

Frontend:
	cd Frontend && make && cd ..

Middleend:
	cd Middleend && make && cd ..

Complier:
	cd Backend/Complier && make && cd ..

Processor:
	cd Backend/Processor && make && cd .. 

.PHONY: Frontend Middleend Complier Processor

run:
	cd Frontend/ && ./FrontendRes.exe
	cd Middleend/ && ./MiddleendRes.exe	
	cd Backend/Complier/ && ./ComplierRes.exe
	cd Backend/Processor/ && ./ProcessorRes.exe

clean:
	cd Frontend && make clean && cd ..
	cd Middleend && make clean && cd ..	
	cd Backend/Complier && make clean && cd ..
	cd Backend/Processor && make clean && cd ..