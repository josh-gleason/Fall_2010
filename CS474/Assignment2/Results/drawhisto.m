function drawhisto(v,filename)
  len = length(v);
  y = ones(1,len*3); %initialize x and y
  x = ones(1,len*3);

  total = 0;
  for c = [1:len]
    total = total + v(c);
  end
 
  for c = [1:3:len*3]
    y(c)=0;
    y(c+1)=v(fix(c+2)/3)/total;
    y(c+2)=0;
    x(c)=fix((c+2)/3);
    x(c+1)=x(c);
    x(c+2)=x(c);
  end
  
  plot(x,y,'Color','Black');
  axis([0,255,0,max(y)]);

  if ( filename )
    print(filename);
end


