{3:}program ks1;label 1;{6:}const max=10000000;
{:6}{4:}var bound,i:integer;{:4}{7:}iptd,ipt:array[1..max]of integer;
ipnum,ipmax,m:integer;{:7}{11:}j:integer;{:11}{18:}k:integer;
{:18}{15:}function sqrt(n:integer):integer;label 1;
begin if n<=1 then sqrt:=n else{16:}for i:=1 to max do if i*i>n then
begin sqrt:=i-1;goto 1;end;{:16}1:;end;
{:15}{17:}function lqf3(n:integer):boolean;label 1;begin lqf3:=true;
j:=n;k:=0;
for m:=2 to ipnum do begin{21:}if j<ipt[m]then begin if k<3 then lqf3:=
false;goto 1 end;{:21}if j mod ipt[m]=0 then begin{20:}j:=j div ipt[m];
k:=k+1;
if(j mod ipt[m]=0)or((n-1)mod(ipt[m]-1)<>0)then begin lqf3:=false;
goto 1;end{:20}end end;{22:}if j>1 then begin k:=k+1;
if(n-1)mod m<>0 then lqf3:=false end;if k<3 then lqf3:=false;{:22};
1:end;{:17}begin{5:}readln(bound);
if bound<=2 then begin writeln('nothing to compute.');
goto 1 end else if bound>max then begin writeln(
'too large bound. I change bound to ',max,'.');bound:=max end;
{:5}{8:}{10:}m:=sqrt(bound)+1;for i:=1 to bound do iptd[i]:=0;
iptd[1]:=1;ipnum:=0;ipmax:=m;
{:10}{9:}{12:}for i:=2 to sqrt(m)+1 do if iptd[i]=0 then begin ipnum:=
ipnum+1;for j:=2 to m div i do iptd[j*i]:=1;end;
{:12}{13:}for i:=sqrt(m)+2 to m do if iptd[i]=0 then ipnum:=ipnum+1;
{:13}{:9}{14:}i:=1;for j:=2 to m do if iptd[j]=0 then begin ipt[i]:=j;
i:=i+1 end;
{:14}{:8}for i:=3 to bound do if odd(i)then{19:}if lqf3(i)then begin
writeln('',i);end;{:19}1:;end.{:3}
