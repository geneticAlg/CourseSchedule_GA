% this script convert the input excel file into 4 configuration txt files
% these config_XX.txt files are consists of config file for 2 semesters 
% autumn & spring, and 2 types of courses (MWF & Tuesday-Thursday)

clear 
close all
clc

ipFileName = 'input_courses_from_Jingyue__simplified.xlsx';
% generate config file for Autumn semester
ExcelTo2ConfigFun(ipFileName,'au'); 
% generate config file for Spring semester
ExcelTo2ConfigFun(ipFileName,'sp');
