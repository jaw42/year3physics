%----------------------------------------------------------------------------------
% Matlab example file simple_plotting.m
%
% This script plots a simple 2D graph of a data file and an analytic function.
%
% Andreas Freise 20.09.2011
%----------------------------------------------------------------------------------

% reset Matlab, i.e. delete all other data and clear all variables
clear all;

% load data file
data=load('test1.dat');

% assign more intuitive variables to data columns 1 and 2
x=data(:,1);
y=data(:,2);

% Now creating the actual plot
figure
plot(x,y,'ro'); % plot data as red circles
hold on; % keep plot active, we want to plot more below
grid on;
xlabel('Some Parameter [some units]');
ylabel('Some Signal [some units]');

% Now add a function trace (requires test_function.m)
% Matlab is a *numeric* package. Thus it can only plot an
% analytic function over an exisiting data vextor (x2 in this case)

x2=linspace(0.4,0.6,50);  % creata a vector with 50 values between 0.4 and 0.6 
y2=test_function(1.0-x2)-0.1;  % apply some analytic expression
plot(x2,y2,'k--'); % plot analytic data as black dashed line

% output filename for eps and png plots
filename='simple_plot';

% To creat a pdf file, plot as eps and then use `epstopdf yourfile.eps' on phymat to creatae the pdf
eval(['print -depsc2 ',filename,'.eps']); 

