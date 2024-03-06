#!/usr/bin/env python3

import os
from flask import Flask, request, send_file

ALLOW_EXTS = [
   'gif', 
   'svg', 
   'jpg', 
   'jpeg', 
   'png', 
   'svg'
]

app = Flask(__name__)

def is_good_ext(filename):
   ext  = os.path.splitext(filename)[1].lstrip('.')
   return ext in ALLOW_EXTS

@app.route('/upload', methods = ['POST'])
def upload():
   if request.method == 'POST':
      file = request.files['file']
      name = os.path.basename(file.filename)
      
      if not is_good_ext(name):
         return 'wat ext?'
      else:
         file.save('/data/' + name)
         return 'all good!'
   else:
      return 'wat?'
		
@app.route('/download', methods = ['GET'])
def download():
   file = request.args.get('file')
   name = os.path.basename(file)

   if not is_good_ext(name):
      return 'wat ext?'
   elif not os.path.exists('/data/' + name):
      return 'not found'
   else:
      return send_file('/data/' + name, as_attachment=True)

@app.route('/', methods = ['GET'])
def index():
   return 'wat?'

if __name__ == '__main__':
   app.run(debug=False, host="0.0.0.0")