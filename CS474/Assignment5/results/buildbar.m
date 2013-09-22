function buildbar(file) 
	A=load(file);
	bar(A,'FaceColor','Red');
	xlabel('Transform Type', 'fontsize', 13, 'fontname', 'Courier New');
	ylabel("Percentage of Zero Value Coefficients", 'fontsize', 15, 'fontname', 'Courier New');
	set(gca,'XLim',[0.25 12.75]);
	set(gca,'XTick',1:1:12);
	set(gca,'XTickLabel',{'Fou-\nrier' 'Haar' 'D4' 'D6' 'D8' 'D10' 'D12' 'D14' 'D16' 'D18' 'D20' 'D22'});
    width = 9;
    height = 9;
    set(gcf,'papersize', [width+.5 height+0.5]);
    set(gcf,'paperposition', [0.25 0.25 width height]);
	print -dpdf -color -FCourier:15 bar.pdf;
end
