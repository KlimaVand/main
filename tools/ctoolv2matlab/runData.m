function out= runData(filename)
fid = fopen(filename);

tline = fgetl(fid);
t=regexp(tline,'\t','split');

out(1,1)=t(2);
i=2;
while ischar(tline);
    disp(tline);
    tline = fgetl(fid);
    if(tline~=-1)
        t=regexp(tline,'\t','split');
        out(i,1)=t(2);
        i=i+1;
    end
end

fclose(fid);