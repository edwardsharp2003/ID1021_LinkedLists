# Define variables
TEX_FILE = rapport_sorting.tex
PDF_FILE = rapport_sorting.pdf
C_FILE = MergeSort.c
EXECUTABLE = MergeSort



.PHONY: compile plot run all latex clean

# Compile C program
compile:
	clang $(C_FILE) -o $(EXECUTABLE)

# Run compiled exe
run:
	./$(EXECUTABLE)

# Plot data with script
plot:
	gnuplot GnuGraphs.txt

latex:
	latexmk -pdf $(TEX_FILE)

# Clean up
clean:
	rm -f $(EXECUTABLE)
	rm -f *.exe


all: compile run plot
