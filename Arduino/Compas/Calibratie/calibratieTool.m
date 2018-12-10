%%Get data
clear all;
clc;
close all;

filename = 'calibratie.xlsx';
Data = xlsread(filename);

x = Data(:,1);
y = Data(:,2);
z = Data(:,3);


sz = 10;

figure(1)
hold on;
grid on;
scatter(x,y,sz,'filled');
figure(2)
hold on;
grid on;
plot(z)



