%C
t=-10:0.01:16;
xt3 = heaviside(t-4) + (t-4).*heaviside(t-4) - 2*(t-7).*heaviside(t-7) + (t - 13).*heaviside(t - 13);
plot(t,xt3)
axis ([0 16 -3 6])
