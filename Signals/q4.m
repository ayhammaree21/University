syms t taw 
xt = (10*exp(-0.2*taw)).*(heaviside(taw - 5)- heaviside(taw - 9));
 ht = (10*exp(0.2*(t- taw))).*(heaviside(t- taw)- heaviside(t- taw - 2));
 conv= int(xt*ht,taw,0,30); 
fplot(conv,[0 30]) 
axis ([-1 30 -0.5 80]);
