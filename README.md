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
PS C:\Window\System32> gcc -L"D:\Programmes\mingwin\lib" arth_nslookup.c -o arth_nslookup -lwsock32

PS C:\Window\System32> .\arth_nslookup.exe -H google.com
> google.com
Address: 216.58.198.206
Response Time: 39 ms

PS C:\Window\System32> .\arth_nslookup.exe -H facebook.com
> facebook.com
Address: 179.60.192.36
Response Time: 44 ms

PS C:\Window\System32> .\arth_nslookup.exe -H maps.google.com
> maps.google.com
Address: 216.58.213.174
Response Time: 40 ms

PS C:\Window\System32> .\arth_nslookup.exe -H a004bucgts.kdn.bitdefender.net
> a004bucgts.kdn.bitdefender.net
Address: 81.161.59.65
Response Time: 46 ms

PS C:\Window\System32> .\arth_nslookup.exe -s 10.158.1.1 -H a004bucgts.kdn.bitdefender.net
> server
Server: 10.158.1.1
> a004bucgts.kdn.bitdefender.net
Address: 81.161.59.65
Response Time: 6 ms

PS C:\Window\System32> .\arth_nslookup.exe -h
C:\Window\System32\arth_nslookup.exe [-s NAME_SERVER] -H DNS_HOST
NAME_SERVER default value: 8.8.8.8

PS C:\Window\System32> .\arth_nslookup.exe -help
C:\Window\System32\arth_nslookup.exe [-s NAME_SERVER] -H DNS_HOST
NAME_SERVER default value: 8.8.8.8
```
