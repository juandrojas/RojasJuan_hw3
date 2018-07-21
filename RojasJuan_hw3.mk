all : PDE_initial.pdf PDE_fixed.pdf ODE.pdf ODE_additional.pdf

RojasJuan_Plots_hw3.py : data_ODE.dat data_PDE_fixed.dat

ODE.pdf : data_ODE.dat RojasJuan_Plots_hw3.py
	python3 RojasJuan_Plots_hw3.py

data_ODE.dat : RojasJuan_ODE.cpp
	g++ RojasJuan_ODE.cpp
	./a.out > data_ODE.dat

PDE_initial.pdf : data_PDE_fixed.dat RojasJuan_Plots_hw3.py
	python3 RojasJuan_Plots_hw3.py

data_PDE_fixed.dat: RojasJuan_PDE.cpp
	g++ $^
	./a.out

ODE_additional.pdf : data_ODE.dat RojasJuan_Plots_hw3.py
	python3 RojasJuan_Plots_hw3.py
