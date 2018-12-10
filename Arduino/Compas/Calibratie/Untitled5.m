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
scatter(x,z,sz,'filled');
scatter(y,z,sz,'filled');
legend('XY','XZ','YZ');
xlim([-10000 10000]);
ylim([-10000 10000]);
%% Calculate offset
x_offset = (max(x)+min(x))/2
y_offset = (max(y)+min(y))/2
z_offset = (max(z)+min(z))/2

%% Corrected data
x_corrected = x - x_offset;
y_corrected = y - y_offset;
z_corrected = z - z_offset;

%% New plot
figure(2)
hold on;
grid on;
scatter(x_corrected,y_corrected,sz,'filled');
scatter(x_corrected,z_corrected,sz,'filled');
scatter(y_corrected,z_corrected,sz,'filled');
legend('XY','XZ','YZ');
xlim([-10000 10000]);
ylim([-10000 10000]);

%%
figure(3)
scatter3(x,y,z,'filled');
figure(4)
scatter3(x_corrected,y_corrected,z_corrected, 'filled');