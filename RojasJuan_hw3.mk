ODE.pdf : data_ODE.dat RojasJuan_Plots_hw3.py
	python3 RojasJuan_Plots_hw3.py

data_ODE.dat : RojasJuan_ODE.cpp
	g++ RojasJuan_ODE.cpp
	./a.out > data_ODE.dat
