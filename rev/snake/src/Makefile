snake: snake.c.template md5.c md5.h rng.c rng.h patch_addr.py patch_template.py md5.c.template rng.c.template
	python3 patch_template.py
	gcc snake.c md5.c rng.c -o snake -lcurses -no-pie
	strip snake
	python3 patch_addr.py
	python3 patch_code.py
	mkdir -p ../distfiles
	cp snake ../distfiles
