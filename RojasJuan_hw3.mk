all : PDE_initial.pdf ODE.pdf

ODE.pdf : data_ODE.dat RojasJuan_Plots_hw3.py
	python3 RojasJuan_Plots_hw3.py

data_ODE.dat : RojasJuan_ODE.cpp
	g++ RojasJuan_ODE.cpp
	./a.out > data_ODE.dat

PDE_initial.pdf : data_PDE_fixed.dat RojasJuan_Plots_hw3.py
	python3 RojasJuan_Plots_hw3.py

data_PDE_fixed.dat: RojasJuan_PDE.cpp
	g++ $^
	./a.out > data_PDE_fixed.dat
