# dns
Easy Windows dns script to query A type and get response time

## How to compile

```Makefile
arth_nslookup: arth_nslookup.c
	gcc -L"D:\Programmes\mingwin\lib" arth_nslookup.c -o arth_nslookup -lwsock32
```

or look in the <a href="https://github.com/AetherBlack/dns/blob/main/Makefile">Makefile</a>.

## Example

```powershell
PS C:\Window\System32> .\arth_nslookup.exe -H google.com
> google.com
Address: 216.58.198.206
Response Time: 40 ms

PS C:\Window\System32> .\arth_nslookup.exe -s 8.8.8.8 -H google.com
> server       
Server: 8.8.8.8
> google.com
Address: 142.250.74.238
Response Time: 40 ms

PS C:\Window\System32> .\arth_nslookup.exe -s 8.8.4.4 -H google.com
> server       
Server: 8.8.4.4
> google.com
Address: 142.250.74.238
Response Time: 42 ms

PS C:\Window\System32> .\arth_nslookup.exe -s 8.8.4.4 -H maps.google.com
> server       
Server: 8.8.4.4
> maps.google.com
Address: 216.58.213.142
Response Time: 41 ms

PS C:\Window\System32> .\arth_nslookup.exe -h
C:\Window\System32\arth_nslookup.exe [-s NAME_SERVER] -H DNS_HOST
NAME_SERVER default value: 8.8.8.8

PS C:\Window\System32> .\arth_nslookup.exe -help
C:\Window\System32\arth_nslookup.exe [-s NAME_SERVER] -H DNS_HOST
NAME_SERVER default value: 8.8.8.8
```
