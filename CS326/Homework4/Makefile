Report.pdf: Report.tex head.tex body.tex title.tex logo.png
	pdflatex Report.tex
	pdflatex Report.tex
	rm Report.aux Report.log Report.out
	okular Report.pdf

clean:
	rm Report.pdf

.PHONY: clean
