RojasJuan_Resultados_hw3.pdf : RojasJuan_Resultados_hw3.tex ODE.pdf ODE_additional.pdf PDE_free.pdf PDE_fixed.pdf PDE_initial.pdf PDE_free_cut.pdf PDE_fixed_cut.pdf
	pdflatex $<

#crear archivos .dat si no existen antes de compilar el script .py
RojasJuan_Plots_hw3.py : ODE.dat PDE_free.dat PDE_fixed.dat PDE_free_cut.dat PDE_fixed_cut.dat

ODE.pdf : RojasJuan_Plots_hw3.py ODE.dat
	python3 $<

ODE.dat : RojasJuan_ODE.cpp
	g++ $<
	./a.out
	rm a.out

PDE_%.pdf : RojasJuan_Plots_hw3.py PDE_%.dat
	python3 $<

PDE_%.dat : RojasJuan_PDE.cpp
	g++ $<
	./a.out
	rm a.out
