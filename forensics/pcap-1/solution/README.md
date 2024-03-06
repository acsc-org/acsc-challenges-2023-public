First, run the following command to extract the HID data:

```
tshark -r ./capture.pcapng -Y 'usb.src == "1.12.1" or usb.src == "1.13.1"' -T fields -e usb.src -e usbhid.data | sed 's/1\.12\.1/K/g' | sed 's/1\.13\.1/M/g' |  sed 's/../:&/g2' > hid.dat
```

`1.12.1` is from the keyboard, so replace it with `K` in the output. `1.13.1` is from the mouse, so replace it with `M`.

Then, run `python3 solve.py hid.dat`. The output is as follows:

```
sliddeess.ggooglee.coom
CTF  Intttro PrresseentaatiionHow to be good aat CTFs?A beginneer's  guuiddeA[BACKSPACE]DonotcheaatGguuessiinngisgoodTthiis iis aann  exxaamppplee ooff aa fllaag:ACSC{f0r3ns1cs_is_s0_fum[BACKSPACE]n}If  yoouu ccaan  rreeeaad ttthiss mmessagge, conngraats!1
Buut  tthee flaagg yoou sseee nnow iiss nnoott  thhee acceepteeed fllaag.
Inspeect  tthhee ppaacckeettss mmmmrree ddeeply, ma[BACKSPACE][BACKSPACE]aandd yoouu wwill rrevveaal mmmrree iinfom[BACKSPACE]rrmaatiion  aaboouutt wwhhaatt iss hhaappeeniing.
I'm  wwrrittiinngg tthhhiss  herre, othheerrwisse 11000 peeooppllee  will  dm mmee tto ssay  tthaat  tthe flaagg is nnoott worrkiinng,, oooor [BACKSPACE][BACKSPACE][BACKSPACE]r  thhee cchallengge iiss broken.
Btw,  I don'tt likee forenssiccss too. :)
```

`ACSC{f0r3ns1cs_is_s0_fum[BACKSPACE]n` -> `ACSC{f0r3ns1cs_is_s0_fun`
