clear all                           % clear all variables from matlab workspace
close all
clc                                  % clear all figures
t = -10:0.01:10;
xt1 = heaviside(t+3) -  heaviside(t-4);
plot(t,xt1)
axis ([-4 5 -2 2])
