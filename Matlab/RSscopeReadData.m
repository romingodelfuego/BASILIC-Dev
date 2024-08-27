function [data,time] = RSscopeReadData(vs,channel)
vs.flush;
if(~(vs.writeread('FORM?')=="UINT,16"))
    vs.writeread('FORM UINT,16;FORM?');
end
vs.writeline('FORM:BORD LSBF');

ch = ['CHAN' num2str(channel)];

bv0 = str2num(vs.writeread([ch ':DATA:YOR?']));
vvpb = str2num(vs.writeread([ch ':DATA:YINC?']));
t0 = str2num(vs.writeread([ch ':DATA:XOR?']));
delta_t = str2num(vs.writeread([ch ':DATA:XINC?']));

cmd = sprintf("CHAN%d%s", channel, ":DATA?");

vs.writeline(cmd)
aa = vs.read(2);
aa = vs.read(str2num(char(aa(2))));
data = vs.read(str2num(char(aa))/2,"uint16");
data = cast(data,'double');
data = data*vvpb+bv0;

time = (0:length(data)-1)'*delta_t+t0;

end
