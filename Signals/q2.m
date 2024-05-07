clear all 
close all 
clc
 t=0:0.0001:3; 
yt1= sin(200*pi*t);
 figure(1) 
plot(t,yt1) 
axis([0 0.03 -2 2]) 
yt2= cos(500*pi*t); 
figure(2) 
plot(t,yt2) 
axis([0 0.03 -2 2]) 
Yt= yt1.*yt2; 
figure(3) 
plot(t,Yt) 
axis([0 0.03 -2 2])
