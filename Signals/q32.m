

clc; 
clear all; 
close all;
 syms t y(t) %symbolic variables 
equation = diff(y,t) + 30*y(t) == 20; %differential eqn. 
condition = y(0) == 0; %initial condition
 y(t) = dsolve(equation,condition) %solution 
t = 0:0.001:1; %time domain
 x = 20*(t>=0); %x(t) = 20u(t) 
digits(6); %precision of data 
y = double(vpa(y(t))); %y(t)
 f = -100:1:100; %frequency domain 
dt = t(2)-t(1); %step size (time) 
for i = 1:length(f) 
X(i) = sum(x.*exp(-2*1i*pi*f(i)*t))*dt; %X(f) 
Y(i) = sum(y.*exp(-2*1i*pi*f(i)*t))*dt; %Y(f)
 H(i) = Y(i)/X(i); %H(f) 
end 
subplot 211; plot(f,abs(H),'b'); grid on; %Magnitude plot
 ylabel("|H(f)|"); 
title("Magnitude Spectrum"); 
subplot 212; plot(f,angle(H),'b');grid on; %Phase plot 
ylabel("{\angle}H(f)"); 
title("Phase Spectrum");
 xlabel("f ->"); 
