clc; 
clear all; 
close all;
 syms t y(t) %symbolic variables 
equation = diff(y,t) + 30*y(t) == 20; %differential eqn. 
condition = y(0) == 0; %initial condition
 y(t) = dsolve(equation,condition) %solution 
