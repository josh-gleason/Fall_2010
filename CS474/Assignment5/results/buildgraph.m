function buildgraph(file) 
	A=load(file);
	bar(A);
	xlabel("Mean Square Error", "fontsize", 15, "fontname", "Courier New");
	ylabel("Percentage of coefficients kept", "fontsize", 15, "fontname", "Courier New");
	set(gca,'XLim', [0.3 8.7]);
	set(gca,'XTick',1:8);
	set(gca,'XTickLabel',5:5:40);
	legend("Haar    (D-2)","Daubechies-4 ","Daubechies-6 ","Daubechies-8 ","Daubechies-10","Daubechiea-12","Daubechies-14","Daubechies-16","Daubechies-18","Daubechies-20","Daubechies-22");
	print -dpdf -FCourier:15 output.pdf;
end
