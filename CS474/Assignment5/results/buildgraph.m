function buildgraph(file) 
	A=load(file);
	bar(A);
	xlabel("Mean Square Error", "fontsize", 15, "fontname", "Courier New");
	ylabel("Percentage of coefficients kept", "fontsize", 15, "fontname", "Courier New");
	set(gca,'XLim', [0.3 8.7]);
	set(gca,'XTick',1:8);
	set(gca,'XTickLabel',5:5:40);
    width = 9;
    height = 9;
    set(gcf,'papersize', [width+.5 height+0.5]);
    set(gcf,'paperposition', [0.25 0.25 width height]);
	legend("Haar    (D-2)","Daubechies-4 ","Daubechies-6 ","Daubechies-8 ","Daubechies-10","Daubechiea-12","Daubechies-14","Daubechies-16","Daubechies-18","Daubechies-20","Daubechies-22");
	print -dpdf -FCourier:15 output.pdf;
end
