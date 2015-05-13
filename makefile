all:
	g++ -o svf -O3 SVF.cpp
	g++ -o svf_base -O3 SVF_base.cpp
base:
	g++ -o svf_base -O3 SVF_base.cpp
svf:
	g++ -o svf -O3 SVF.cpp
clean:
	rm -f *.o svf svf_base
