LATEX = pdflatex

TEX = ${wildcard *.tex}

BIB = ${wildcard *.bib} ${wildcard *.bst}

VISIOPDF = $(wildcard visio/*.pdf)

PAPERNAME = paper

all : ${PAPERNAME}.pdf

${PAPERNAME}.pdf: $(TEX) $(BIB) $(VISIOPDF)
	$(LATEX) ${PAPERNAME}
	bibtex ${PAPERNAME}
	$(LATEX) ${PAPERNAME}
	$(LATEX) ${PAPERNAME}
	open ${PAPERNAME}.pdf &
   
clean: 
	rm -f ${PAPERNAME}.ps ${PAPERNAME}.pdf ${PAPERNAME}.dvi ${PAPERNAME}.bbl ${PAPERNAME}.blg ${PAPERNAME}.log ${PAPERNAME}.aux ${PAPERNAME}.out
