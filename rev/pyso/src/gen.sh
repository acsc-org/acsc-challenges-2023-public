#!/bin/sh

cat header.py > libvalidator.py
python3 ksagen.py >> libvalidator.py
python3 spritzgen.py >> libvalidator.py
python3 comparegen.py >> libvalidator.py
cat footer.py >> libvalidator.py
python3 obfuscate_names.py

python3 libvalidator.py