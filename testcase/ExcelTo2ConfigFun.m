function ExcelTo2ConfigFun(ipFileName,semester )
% this script is used to read course schedule requirements in the excel
% file and output these requirements in standard format as the txt
% configureation file
[sCheck,~,~] =  xlsread(ipFileName,'main');

if(semester == 'au') 
    sCheck = sCheck(:,9);
    semName = '_AU';
else
    sCheck = sCheck(:,10);
    semName = '_Sp';
end


txtFileName1 = strcat(ipFileName(1:end-5),semName,'_config_MWF.txt');
txtFileName2 = strcat(ipFileName(1:end-5),semName,'_config_TR.txt');


%% txt 1

%% time_slots 
[~,txt,~] = xlsread(ipFileName,'time_slots');
txtsz = size(txt);

tf = fopen(txtFileName1,'wt');
fprintf(tf,'##TIME\n\n');
fprintf(tf,'#start\n');
MWF = txt(2:end,1);
TR = txt(2:end,3);
% Labs = txt(2:end,5);
for i = 1:length(MWF)
    if(isempty(MWF{i}))
        break;
    end
    fprintf(tf,strcat("1 ",MWF{i},'\n'));
end

fprintf(tf,'#end\n\n');

%% class rooms
fprintf(tf,'##CLASSROOM\n\n');
[num,~,~] = xlsread(ipFileName,'rooms');

small = num(1);
medium = num(2);
large = num(3);
xlarge = num(4);
len = small + medium + large + xlarge;
numOfFreeRoom = len; % num of rooms that have no name


% standard room with no name
for i = 1:len
    if(i<=small)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(30),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    elseif (i<=small+medium)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(50),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    elseif (i<=small+medium+large)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(70),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    else
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(150),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    end
end

% special room with names, the fix_room
[~,~,raw] = xlsread(ipFileName,'fix_rooms');
raw = raw(2:end,:);
rawsz = size(raw);
fix_room = raw(:,1);
fix_id = raw(:,2);
room_size = raw(:,3);
for i = 1:rawsz(1)
    if(isnan(fix_id{i})) 
        continue;
    end
    fprintf(tf,'#start\n');
    fprintf(tf,strcat('room_name = ',fix_room{i},'\n'));
    fprintf(tf,strcat('room_id = ',num2str(fix_id{i}+numOfFreeRoom),'\nseats = ',num2str(room_size{i}),'\ntype = 0\n'));
    fprintf(tf,'#end\n\n');
end



%% course 
[num,~,raw] = xlsread(ipFileName,'main');
fprintf(tf,'##COURSE\n\n');
numsz = size(num);
department = raw(2:end,3);
time_slot = num(:,4);
course_id = num(:,2);
fix_time = num(:,8);
fix_room = num(:,7);

c_id = unique(course_id);
csz = size(c_id);
for i = 1:csz(1)
    % find the corresponding index
    tpidx = find(course_id==c_id(i));
    tpidx = tpidx(1); 
    % skip the courses from another semester
    if(sCheck(tpidx)==0) 
        continue; 
    end
    
    if(time_slot(tpidx)==135)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('id = ',num2str(c_id(i)),'\n'));
        fprintf(tf,strcat('department = ',department{tpidx},'\n'));
        fprintf(tf,strcat('time = ',num2str(1),'\n'));
        fprintf(tf,strcat('fix_day = {}\n'));
    elseif(time_slot(tpidx)==24) % skip courses for TR
        continue;
    elseif(time_slot(tpidx)==1 || time_slot(tpidx)==3 || time_slot(tpidx)==5 ||...
            time_slot(tpidx)==13 || time_slot(tpidx)== 35 || time_slot(tpidx)==15)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('id = ',num2str(c_id(i)),'\n'));
        fprintf(tf,strcat('department = ',department{tpidx},'\n'));
        tempd = time_slot(tpidx);
        fprintf(tf,strcat('time = ',num2str(1),'\n'));
        if(tempd<10)
            fprintf(tf,strcat('fix_day = {',num2str(tempd),'}\n'));
        else
           atp = floor(tempd/10);
           btp = mod(tempd,10);
           fprintf(tf,strcat('fix_day = {',num2str(atp),',',num2str(btp),'}\n'));
        end
    else
        continue;
    end
    
    if(isnan(fix_time(tpidx)))
        fprintf(tf,strcat('fix_time = ',num2str(-1),'\n'));
    else
        fprintf(tf,strcat('fix_time = ',num2str(fix_time(tpidx)),'\n'));
    end
    
    if(isnan(fix_room(tpidx)))
        fprintf(tf,strcat('fix_room = ',num2str(-1),'\n'));
    else
        fprintf(tf,strcat('fix_room = ',num2str(fix_room(tpidx)+numOfFreeRoom),'\n'));
    end
    fprintf(tf,'#end\n\n');
end

%% instructor 

fprintf(tf,'##INSTRUCTOR\n\n');
[instructor_id,instructor_txt,instructor_raw] = xlsread(ipFileName,'instructors');
instructor_txt = instructor_txt(2:end,1);
sz = size(instructor_txt);
preference = instructor_raw(2:end,3); % preference time slots of instructors
for i = 1:sz(1)
    fprintf(tf,'#start\n');
    fprintf(tf,strcat('name = ',instructor_txt{i},'\n'));
    fprintf(tf,strcat('id = ',num2str(instructor_id(i)),'\n'));
    
    if(isnan(preference{i}))
        fprintf(tf,'prefertime = {}\n');
    else
        fprintf(tf,strcat('prefertime = {',preference{i},'}\n'));
    end
    fprintf(tf,'#end\n\n');
end


%% class
class_id = num(:,1);
professor_id = num(:,6);
students_num = num(:,5);
fprintf(tf,strcat('##CLASS\n\n'));
c_id = unique(class_id);
csz = size(c_id);
for i = 1:csz(1)
    tpidx = find(class_id==c_id(i));
    tpidx = tpidx(1);
    % skip the classes of another semester
    if(sCheck(tpidx)==0) 
        continue;
    end
    
    if(time_slot(tpidx)== 24 || time_slot(tpidx)== 2 || time_slot(tpidx)== 4)
        continue;
    end
    fprintf(tf,'#start\n');
    fprintf(tf,strcat('id = ',num2str(class_id(tpidx)),'\n'));
    fprintf(tf,strcat('course_id = ',num2str(course_id(tpidx)),'\n'));
    fprintf(tf,strcat('professor_id = ',num2str(professor_id(tpidx)),'\n'));
    fprintf(tf,strcat('students_num = ',num2str(students_num(tpidx)),'\n'));
    fprintf(tf,'type = 0\n#end\n\n');
end




%% txt 2

%% time_slots 
[~,txt,~] = xlsread(ipFileName,'time_slots');
txtsz = size(txt);

tf = fopen(txtFileName2,'wt');
fprintf(tf,'##TIME\n\n');
fprintf(tf,'#start\n');
MWF = txt(2:end,1);
TR = txt(2:end,3);

for i = 1:length(TR)
    if(isempty(TR{i}))
        break;
    end
    fprintf(tf,strcat("2 ",TR{i},'\n'));
end
fprintf(tf,'#end\n\n');

%% class rooms
fprintf(tf,'##CLASSROOM\n\n');
[num,~,~] = xlsread(ipFileName,'rooms');

small = num(1);
medium = num(2);
large = num(3);
xlarge = num(4);
len = small + medium + large + xlarge;
numOfFreeRoom = len; % num of rooms that have no name


% standard room with no name
for i = 1:len
    if(i<=small)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(30),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    elseif (i<=small+medium)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(50),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    elseif (i<=small+medium+large)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(70),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    else
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('room_name = ','DL-',num2str(i),'\n'));
        fprintf(tf,strcat('room_id = ',num2str(i),'\nseats = ',num2str(150),'\ntype = 0\n'));
        fprintf(tf,'#end\n\n');
    end
end

% special room with names, the fix_room
[~,~,raw] = xlsread(ipFileName,'fix_rooms');
raw = raw(2:end,:);
rawsz = size(raw);
fix_room = raw(:,1);
fix_id = raw(:,2);
room_size = raw(:,3);
for i = 1:rawsz(1)
    if(isnan(fix_id{i})) 
        continue;
    end
    fprintf(tf,'#start\n');
    fprintf(tf,strcat('room_name = ',fix_room{i},'\n'));
    fprintf(tf,strcat('room_id = ',num2str(fix_id{i}+numOfFreeRoom),'\nseats = ',num2str(room_size{i}),'\ntype = 0\n'));
    fprintf(tf,'#end\n\n');
end



%% course 
[num,~,raw] = xlsread(ipFileName,'main');
Instructor_ID_main = raw(:,6);
fprintf(tf,'##COURSE\n\n');
numsz = size(num);
department = raw(2:end,3);
time_slot = num(:,4);
course_id = num(:,2);
fix_time = num(:,8);
fix_room = num(:,7);

c_id = unique(course_id);
csz = size(c_id);
for i = 1:csz(1)
    % find the corresponding index
    tpidx = find(course_id==c_id(i));
    tpidx = tpidx(1);   
    % skip courses of another semester
    if(sCheck(tpidx)==0)
        continue;
    end
    if(time_slot(tpidx)==135)
        continue;
    elseif(time_slot(tpidx)==24)
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('id = ',num2str(c_id(i)),'\n'));
        fprintf(tf,strcat('department = ',department{tpidx},'\n'));
        fprintf(tf,strcat('time = ',num2str(2),'\n'));
        fprintf(tf,strcat('fix_day = {}\n'));
    elseif(time_slot(tpidx)==1 || time_slot(tpidx)==3 || time_slot(tpidx)==5 ||...
            time_slot(tpidx)==13 || time_slot(tpidx)== 35 || time_slot(tpidx)==15)
        continue;
    else
        fprintf(tf,'#start\n');
        fprintf(tf,strcat('id = ',num2str(c_id(i)),'\n'));
        fprintf(tf,strcat('department = ',department{tpidx},'\n'));
        tempd = time_slot(tpidx);
        fprintf(tf,strcat('time = ',num2str(2),'\n'));
        if(tempd<10)
            fprintf(tf,strcat('fix_day = {',num2str(tempd),'}\n'));
        else
           atp = floor(tempd/10);
           btp = mod(tempd,10);
           fprintf(tf,strcat('fix_day = {',num2str(atp),',',num2str(btp),'}\n'));
        end
    end
    
    if(isnan(fix_time(tpidx)))
        fprintf(tf,strcat('fix_time = ',num2str(-1),'\n'));
    else
        fprintf(tf,strcat('fix_time = ',num2str(fix_time(tpidx)),'\n'));
    end
    
    if(isnan(fix_room(tpidx)))
        fprintf(tf,strcat('fix_room = ',num2str(-1),'\n'));
    else
        fprintf(tf,strcat('fix_room = ',num2str(fix_room(tpidx)+numOfFreeRoom),'\n'));
    end
    fprintf(tf,'#end\n\n');
end

%% professor 

fprintf(tf,'##INSTRUCTOR\n\n');
[instructor_id,instructor_txt,instructor_raw] = xlsread(ipFileName,'instructors');

instructor_txt = instructor_txt(2:end,1);
sz = size(instructor_txt);
preference = instructor_raw(2:end,3); % preference time slots of instructors
for i = 1:sz(1)
    fprintf(tf,'#start\n');
    fprintf(tf,strcat('name = ',instructor_txt{i},'\n'));
    fprintf(tf,strcat('id = ',num2str(instructor_id(i)),'\n'));
    if(isnan(preference{i}))
        fprintf(tf,'prefertime = {}\n');
    else
        fprintf(tf,strcat('prefertime = {',preference{i},'}\n'));
    end
    fprintf(tf,'#end\n\n');
end


%% class
class_id = num(:,1);
professor_id = num(:,6);
students_num = num(:,5);
fprintf(tf,strcat('##CLASS\n\n'));
c_id = unique(class_id);
csz = size(c_id);
for i = 1:csz(1)
    tpidx = find(class_id==c_id(i));
    tpidx = tpidx(1);
    % skip classes of another semester
    if(sCheck(tpidx)==0)
        continue;
    end
    if(time_slot(tpidx)== 135 || time_slot(tpidx)== 1 || time_slot(tpidx)== 3 || time_slot(tpidx)== 5 ...
        || time_slot(tpidx)==13 || time_slot(tpidx)== 35 || time_slot(tpidx)== 15)
        continue;
    end
    fprintf(tf,'#start\n');
    fprintf(tf,strcat('id = ',num2str(class_id(tpidx)),'\n'));
    fprintf(tf,strcat('course_id = ',num2str(course_id(tpidx)),'\n'));
    fprintf(tf,strcat('professor_id = ',num2str(professor_id(tpidx)),'\n'));
    fprintf(tf,strcat('students_num = ',num2str(students_num(tpidx)),'\n'));
    fprintf(tf,'type = 0\n#end\n\n');
end

fclose(tf);




end

